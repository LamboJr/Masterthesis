

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;
use work.Constants_and_Declarations.all;

entity ModuleTop is 
      Port(--sysclk : in STD_LOGIC;
        --o_TX_Serial : out STD_LOGIC;
        io_Serial : inout STD_LOGIC;
        i_MS_SLV : in std_logic;
        i_Enable : in std_logic;
        resetbutton : in std_logic;
        tradebutton : in std_logic;
        switch3  : in std_logic;
        switch2 : in std_logic;
        switch1 : in std_logic;
        switch0 : in std_logic;
        o_debug : out std_logic;
        LED : out std_logic);
end ModuleTop;

 architecture behav of ModuleTop is
--signal for transmitting signal from PL to PS
signal PL_to_PS_buffer : std_logic_vector(31 downto 0) := (others => '0');
--signal for transmitting control signal from PS to PL
signal PS_to_PL_buffer : std_logic_vector(31 downto 0) := (others => '0');
--signals for connecting the BRAM to the RTL and the ZYNQ processor
signal bram_porta_0_addr : std_logic_vector(12 downto 0);
signal clk : std_logic;
signal bram_porta_0_din : std_logic_vector( 31 downto 0);
signal bram_porta_0_dout : std_logic_vector( 31 downto 0);
signal bram_porta_0_en : std_logic;
signal rst : std_logic;
signal bram_porta_0_we : std_logic_vector(3 downto 0);

--Bram Port b
signal bram_portb_0_addr : std_logic_vector(10 downto 0) := (others => '0');
signal bram_portb_0_din : std_logic_vector(31 downto 0) := (others => '0');
signal bram_portb_0_we : std_logic := '0';

-- Test Bench uses a 125 MHz Clock
--Board Freq is changeable but its alos set to 125 MHz. Until there is a reason to change it
-- Want to interface to 115200 baud UART
-- 125000000 / 115200 = 1085 Clocks Per Bit.
constant c_CLKS_PER_BIT : integer := 1085;
    
--signals for connecting the UART TX and RX components
signal w_RX_DV     : std_logic;
signal w_RX_WORD   : std_logic_vector(15 downto 0);
signal r_RX_SERIAL : std_logic; 
signal w_TX_Active : std_logic;
signal r_TX_DV     : std_logic                    := '0';
signal r_TX_WORD   : std_logic_vector(15 downto 0) := (others => '0');
signal w_TX_SERIAL : std_logic;
signal w_TX_DONE   : std_logic;

--Ringbuffer signals

constant RAM_WIDTH : natural := 32;
constant RAM_DEPTH : natural := 2048;

signal wr_en : std_logic;
signal wr_en_PL : std_logic;
signal wr_en_PS : std_logic;
signal wr_data_PL : std_logic_vector(RAM_WIDTH-1 downto 0);
signal wr_data_PS : std_logic_vector(RAM_WIDTH-1 downto 0);
signal wr_data : std_logic_vector(RAM_WIDTH - 1 downto 0);

signal rd_en : std_logic;
signal rd_en_PL :std_logic;
signal rd_en_PS : std_logic;

signal rd_valid : std_logic;
signal rd_data : std_logic_vector(RAM_WIDTH - 1 downto 0);

signal empty : std_logic;
signal empty_next : std_logic;
signal full : std_logic;
signal full_next : std_logic;

signal fill_count : integer range RAM_DEPTH -1 downto 0;

-- intern signals
signal temp : std_logic_vector(31 downto 0) := x"00000000";

signal ModeMonitor : std_logic;
signal ModeTrading : std_logic;


--TradeHandler signals
signal w_response_valid : std_logic;
signal r_generate_response : std_logic;
signal w_data_output : std_logic_vector(15 downto 0) := x"FFF8";  --Signal for data which gets send to UART TX Component to transmit
signal r_data_input :std_logic_vector(15 downto 0) := x"FFF8";   ---signal for newly received data via the UART RX module

signal send_delay_counter : integer := 0;
    
signal w_debug : std_logic := '0';

constant send_delay : integer := 1000;
---------
--new experimental

type t_framingState is (s_idle,s_Processing,s_resetCounter,s_Holding);
signal framingState : t_framingState := s_idle;

signal frameCounter : integer := 1;

signal BlockRequestActive : std_logic;
signal BlockInitActive : std_logic;
signal BlockDatalength : integer;
signal Blockrequesttype : t_BlockRequestType;

signal DataInputBuffer : t_BufferSize110 := (others => x"0003");

signal team_spot : natural range 0 to TEAM_SIZE := 0;
signal team_buffer_index : natural range 0 to (PKMN_BUFFER_DEPTH ) := 50;

--buffer counter for indexing position of buffer
signal buffer_index : natural range 0 to 110 := 0;

signal Team : Team_structure := (Zapdos_buffer,Milotic_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer);
signal team_index : natural range 0 to TEAM_SIZE -1 := 0;

signal Team_new : Team_structure := (Zapdos_buffer,Milotic_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer);
signal Team_valid : std_logic;

signal Team2PokemonBuffer : t_TwoPokemonbuffer := (EmptyBuffer110,EmptyBuffer110,EmptyBuffer110);
signal TrainerDataSize110 : t_BufferSize110 := (others => x"0000");

signal BlockRequestPokemonIndex : natural range 0 to 2;

signal TradeSpotNumber : std_logic_vector(2 downto 0);

begin

--UART Receiver
    UART_RX_INST : entity work.uart_rx
    generic map (
      g_CLKS_PER_BIT => c_CLKS_PER_BIT
      )
    port map (
      sysclk       => clk,
      i_RX_Serial => r_RX_Serial,
      o_RX_DV     => w_RX_DV,
      o_RX_Word   => w_RX_WORD
      );
 --UART Transmitter      
    UART_TX_INST : entity work.uart_tx 
    generic map ( g_CLKS_PER_BIT => c_CLKS_PER_BIT
    )
    port map (
      sysclk       => clk,
      i_TX_DV     => r_TX_DV,
      i_TX_Word   => w_data_output,
      o_TX_Active => w_TX_Active,
      --o_TX_Serial => o_TX_Serial,
      o_TX_Serial => w_TX_Serial,
      o_TX_Done   => w_TX_DONE
      ); 




--zynq system block
zynq_ps_interface_inst: entity work.GB_UNIT_design_wrapper
    port map (
        BRAM_PORTA_0_addr => bram_porta_0_addr,
        BRAM_PORTA_0_clk => clk,
        BRAM_PORTA_0_din => bram_porta_0_din,
        BRAM_PORTA_0_dout => bram_porta_0_dout,
        BRAM_PORTA_0_en => bram_porta_0_en,
        BRAM_PORTA_0_rst => rst,
        BRAM_PORTA_0_we => bram_porta_0_we,
        PL_to_PS_buffer => PL_to_PS_buffer,
        PS_to_PL_buffer => PS_to_PL_buffer);
        

  
ringbuffer_inst : entity work.ringbuffer
generic map (RAM_DEPTH => RAM_DEPTH,RAM_WIDTH => RAM_WIDTH)
port map(
clk => clk,
rst => rst,
wr_en => wr_en,
wr_data => wr_data,
rd_en => rd_en,
rd_valid => rd_valid,
rd_data => rd_data,
empty => empty,
empty_next => empty_next,
full => full,
full_next => full_next,
fill_count => fill_count
);

TradeHandler_inst : entity work.TradeHandler
port map(
    clk => clk,
    o_data_output => w_data_output,
    i_data_input => r_data_input,
    o_response_valid => w_response_valid,
    i_generate_response => r_generate_response,
    reset => rst,
    tradebutton => tradebutton,
    o_debug => w_debug,
    frameCounter => frameCounter,
    BlockrequestType => Blockrequesttype,
    BlockDataLength => BlockDataLength,
    BlockrequestActive => BlockRequestActive,
    BlockInitActive => BlockInitActive,
    DataInputBuffer => DataInputbuffer,
    BlockRequestPokemonIndex => BlockRequestPokemonIndex,
    TradeSpotNumber => TradespotNumber
);

UpdateBuffer : process(clk)
begin
    if rising_edge(clk) then
        if BlockRequestActive = '1'  and BlockInitActive = '1' then
            case BlockRequestType is
                when BLOCK_REQ_SIZE_200 =>
                    DataInputBuffer <= Team2Pokemonbuffer(BlockRequestPokemonIndex);
                        
                when BLOCK_REQ_SIZE_100 =>
                    DataInputBuffer <= TrainerDataSize110;

                when others=>
                     
                end case;
        end if;
    end if;
end process;

Mode_select : process(clk)
begin
    if rising_edge(clk) then
         case PS_to_PL_buffer(7 downto 4) is
            when x"1" =>
                    team_spot <= 0;
            when x"2" =>
                    team_spot  <= 1;
            when x"3" =>
                    team_spot  <= 2;
            when x"4" =>
                    team_spot  <= 3;
            when x"5" =>
                    team_spot  <= 4;
            when x"6" =>
                    team_spot  <= 5;
            when x"7" =>
                --Reset Counter for new Pokemon
                --Team_valid <= '1';
            when x"8" =>
                --Finished configuring the important data
                --indicates transfer of the buffer
            when others =>
            end case;
        
    end if;
    end process;
    
    Fill_Team2PokemonBuffer : process(clk)
    begin
    if rising_edge(clk) then
        if (PS_to_PL_buffer(7 downto 4) = 8) and Team_valid = '1' then
            if buffer_index < 50 then
                Team2Pokemonbuffer(0)(buffer_index) <= Team(0)(buffer_index);
                Team2Pokemonbuffer(0)(buffer_index+50) <= Team(1)(buffer_index);
                Team2Pokemonbuffer(1)(buffer_index) <= Team(2)(buffer_index);
                Team2Pokemonbuffer(1)(buffer_index+50) <= Team(3)(buffer_index);
                Team2Pokemonbuffer(2)(buffer_index) <= Team(4)(buffer_index);
                Team2Pokemonbuffer(2)(buffer_index+50) <= Team(5)(buffer_index);
                TrainerDataSize110(buffer_index) <= Trainer_buffer(buffer_index);
                buffer_index <= buffer_index +1;
            end if;
        else    
            buffer_index <= 0;
        end if;
    end if;
    end process;
    
    
    ReceiveTeam :process(clk)
    begin
    if rising_edge(clk) then
        if resetbutton = '1' then
            Team_new <= (others => No_pokemon_buffer);
            Team_valid <= '0';
            team_buffer_index <= 50;
        else 
           
            if team_buffer_index < 50 then
                Team_valid <= '0';
                rd_en_PL <= '1';
                if rd_valid = '1' then
                    Team_new(team_spot)(team_buffer_index)(15 downto 0 ) <= rd_data(15 downto 0);
                    team_buffer_index <= team_buffer_index + 1;
                end if;
            else
                rd_en_PL <= '0';
                Team_valid <= '1';
                if PS_to_PL_buffer(7 downto 4) = x"7" then
                    team_buffer_index <= 0;
                end if;
            end if;
        end if;
    end if;
    end process;



sample: process(clk)
begin
if rising_edge(clk) then
    if  w_RX_DV = '1' and i_enable = '0' and i_MS_SLV = '1' then
        r_generate_response <= '1';
        r_data_input <= w_RX_WORD;
    else
        r_generate_response <= '0';
    end if;
end if;
end process;




Transmitter : process(clk)
begin
if rising_edge(clk) then
    if resetbutton = '1' then
        r_TX_DV <= '0';
        send_delay_counter <= 0;
     
    else
        if ModeTrading = '1' then
            if i_MS_SLV = '0' then
                if send_delay_counter < send_delay then
                    send_delay_counter <= send_delay_counter + 1;
                    r_TX_DV <= '0';
                elsif send_delay_counter = send_delay then
                    r_TX_DV <= '1';
                    send_delay_counter <= send_delay_counter + 1;
                else
                    r_TX_DV <= '0';                          
                end if;
            else
                send_delay_counter <= 0;
                r_TX_DV <='0';
            end if;
        end if;
    end if;
end if;
end process;    

--monitoring :  process (clk)
-- begin
--    if rising_edge(clk) then
--       if rst = '1' then
--           wr_data <= x"00000000";
--           wr_en <= '0'; 
--       else
--            if ModeMonitor = '1' then
--                --check if new data frame was received and the communication is enabled 
--                if w_RX_DV ='1' and i_enable = '0'   then
--                --i_MS_SLV indicates if the GB Master or the GB Slave is sending data
--                -- This section of the code writes data to the ringbuffer if both the master and slave data frame are received
--                    if i_MS_SLV = '0' then
--                            wr_data_PL(15 downto 0) <= w_RX_WORD;
--                            wr_en_PL <= '1';
--                    else
--                            wr_data(31 downto 16) <= w_RX_WORD;
--                            wr_en_PL <= '0';
--                    end if;
--                else
--                        wr_en_PL <= '0';
--                end if;
--            else
----                if w_response_valid = '1' then
----                    wr_data(31 downto 16) <= w_RX_WORD;
----                    wr_data(7 downto 0) <= std_logic_vector(to_unsigned(FrameCounter,8));
----                    wr_en <= '1';
----                else
----                    wr_en <= '0';
----                end if;                 
                
--            end if;
--       end if;    
--    end if;
-- end process;
 
    Team <= Team_new when Team_valid = '1' else Team;
 
    --send switch state to SOftware
    PL_to_PS_buffer(0) <= switch0;
    PL_to_PS_buffer(1) <= switch1;
    PL_to_PS_buffer(2) <= switch2;
    PL_to_PS_buffer(3) <= switch3;
    
    TradespotNumber <= switch2 & switch1 & switch0;
    
    --conversion from the BRAM control signels to the ringbuffer control signals 
    temp(0) <= empty;
    --bram dout is set to rd_data only if data is requested at address 0 at Software level, else the empty flag is writen to dout 
    bram_porta_0_dout <= rd_Data when bram_porta_0_addr = x"00000000" else temp;
    rd_en_PS <= (not(bram_porta_0_we(0)) and bram_porta_0_en) when bram_porta_0_addr = x"00000000" else '0';
    
    rd_en <= rd_en_PS or rd_en_PL;
    
    wr_data <= bram_porta_0_din; 
    wr_en <= bram_porta_0_we(0) and bram_porta_0_en;
    
    --wr_data <= wr_data_PL when ModeTrading = '0' else wr_data_PS;
    --wr_en <= wr_en_PS or wr_en_PL;
    
    ModeMonitor <= PS_to_PL_buffer(0);
    ModeTrading <= not(PS_to_PL_buffer(0));
    
     io_Serial <= w_TX_Serial when (i_MS_SLV ='0' and ModeTrading ='1') else 'Z';
     r_RX_Serial <= io_Serial when (i_MS_SLV ='1' or  ModeMonitor = '1') else 'Z';

    --r_RX_Serial <= io_Serial;
    
    o_debug <= w_debug;
end behav;