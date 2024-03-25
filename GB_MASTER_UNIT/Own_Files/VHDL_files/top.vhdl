

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
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
        --o_debug : out std_logic;
        LED : out std_logic);
end ModuleTop;

architecture behav of ModuleTop is
 
--32 Bit signal for transmitting data from PL to PS
signal PL_to_PS_buffer : std_logic_vector(31 downto 0) := (others => '0');

--32 Bit signal for transmitting data from PS to PL
signal PS_to_PL_buffer : std_logic_vector(31 downto 0) := (others => '0');

--signals for connecting the Ringbuffer(s) to the RTL and the ZYNQ processor
signal BRAM_Controller_Addr : std_logic_vector(12 downto 0);
signal clk : std_logic;
signal BRAM_Controller_Data_in : std_logic_vector( 31 downto 0);
signal BRAM_Controller_Data_out : std_logic_vector( 31 downto 0);
signal BRAM_Controller_en : std_logic;
signal rst : std_logic;
signal BRAM_Controller_wr_en : std_logic_vector(3 downto 0);


-- Test Bench uses a 125 MHz Clock
--Board Freq is changeable but is also set to 125 MHz. Until there is a reason to change it
-- Want to interface to 115200 baud UART
-- 125000000 / 115200 = 1085 Clocks Per Bit.
constant c_CLKS_PER_BIT : integer := 1085;
    
--signals for connecting the UART TX and RX components
signal w_RX_DV     : std_logic;  --Data Valid (Output data)
signal w_RX_WORD   : std_logic_vector(15 downto 0);  --Data Word
signal r_RX_SERIAL : std_logic;  --Serial Bit Stream

signal w_TX_Active : std_logic;
signal r_TX_DV     : std_logic                 := '0'; -- Data Valid (Input data)
signal r_TX_WORD   : std_logic_vector(15 downto 0) := (others => '0');   --Data Word
signal w_TX_SERIAL : std_logic;  -- Serial Bit Stream
signal w_TX_DONE   : std_logic;  -- Transmission Done

--Ringbuffer signals abd constant ----

constant RAM_WIDTH : natural := 32;
constant RAM_DEPTH : natural := 2048;

constant Zero_address : std_logic_vector(12 downto 0) := b"0000000000000";

----------------------Ringbuffer PS to Pl--------------------------
signal wr_en_PS_to_PL : std_logic;
signal wr_data_PS_to_PL : std_logic_vector(RAM_WIDTH-1 downto 0);
signal rd_en_PS_to_PL : std_logic;
signal rd_valid_PS_to_PL : std_logic;
signal rd_data_PS_to_PL : std_logic_vector(RAM_WIDTH - 1 downto 0);
signal empty_PS_to_PL : std_logic;
signal empty_next_PS_to_PL : std_logic;
signal full_PS_to_PL : std_logic;
signal full_next_PS_to_PL : std_logic;
signal fill_count_PS_to_PL : integer range RAM_DEPTH -1 downto 0;

----------------------Ringbuffer PL to PS---------------------------
signal wr_en_PL_to_PS : std_logic;
signal wr_data_PL_to_PS : std_logic_vector(RAM_WIDTH-1 downto 0);
signal rd_en_PL_to_PS :std_logic;
signal rd_valid_PL_to_PS : std_logic;
signal rd_data_PL_to_PS : std_logic_vector(RAM_WIDTH - 1 downto 0);
signal empty_PL_to_PS : std_logic;
signal empty_next_PL_to_PS : std_logic;
signal full_PL_to_PS : std_logic;
signal full_next_PL_to_PS : std_logic;
signal fill_count_PL_to_PS : integer range RAM_DEPTH -1 downto 0;

-- intern signals
signal temp : std_logic_vector(31 downto 0) := x"00000000";

-------------signals to indicate the Monitoring or Trading Mode
signal ModeMonitor : std_logic;
signal ModeTrading : std_logic;

---------------------sending delay signals-----------------------
signal send_delay_counter : integer := 0;
constant send_delay : integer := 3000;

----optionla to use debug signal
signal w_debug : std_logic := '0';

------Framecounter signal generated in HW and transported to SOftware
signal frameCounter : integer := 1;



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
      --i_TX_Word   => w_data_output,
      i_TX_Word   => r_TX_WORD,
      o_TX_Active => w_TX_Active,
      --o_TX_Serial => o_TX_Serial,
      o_TX_Serial => w_TX_Serial,
      o_TX_Done   => w_TX_DONE
      ); 

--zynq system block
zynq_ps_interface_inst: entity work.GB_UNIT_design_wrapper
    port map (
        BRAM_PORTA_0_addr => BRAM_Controller_Addr,
        BRAM_PORTA_0_clk => clk,
        BRAM_PORTA_0_din => BRAM_Controller_data_out,
        BRAM_PORTA_0_dout => BRAM_Controller_data_in,
        BRAM_PORTA_0_en => BRAM_Controller_en,
        BRAM_PORTA_0_rst => rst,
        BRAM_PORTA_0_we => BRAM_Controller_wr_en,
        PL_to_PS_buffer => PL_to_PS_buffer,
        PS_to_PL_buffer => PS_to_PL_buffer);
        

  --ringbuffer PL to PS
ringbuffer_PL_to_PS_inst : entity work.ringbuffer
generic map (RAM_DEPTH => RAM_DEPTH,RAM_WIDTH => RAM_WIDTH)
port map(
clk => clk,
rst => rst,
wr_en => wr_en_PL_to_PS,
wr_data => wr_data_PL_to_PS,
rd_en => rd_en_PL_to_PS,
rd_valid => rd_valid_PL_to_PS,
rd_data => rd_data_PL_to_PS,
empty => empty_PL_to_PS,
empty_next => empty_next_PL_to_PS,
full => full_PL_to_PS,
full_next => full_next_PL_to_PS,
fill_count => fill_count_PL_to_PS
);
--ringbuffer Ps to PL
ringbuffer_PS_to_PL_inst : entity work.ringbuffer
generic map (RAM_DEPTH => RAM_DEPTH,RAM_WIDTH => RAM_WIDTH)
port map(
clk => clk,
rst => rst,
wr_en => wr_en_PS_to_PL,
wr_data => wr_data_PS_to_PL,
rd_en => rd_en_PS_to_PL,
rd_valid => rd_valid_PS_to_PL,
rd_data => rd_data_PS_to_PL,
empty => empty_PS_to_PL,
empty_next => empty_next_PS_to_PL,
full => full_PS_to_PL,
full_next => full_next_PS_to_PL,
fill_count => fill_count_PS_to_PL
);   
    
--FramecounterGenerator_Inst : entity work.FramecounterGenerator
--port map (
--    clk => clk,
--    reset => rst,
--    enable => i_enable,
--    FrameCounter => Framecounter
--);
         
---process only is used in ModeTrading
-- This process detects a falling edge on the MS_SLV flag 
-- It then start a counter.
--After 3000 clock cycles the process initializes the transmission of a data paket via the UART Transmitter   
Transmitter : process(clk)
begin
if rising_edge(clk) then
    if resetbutton = '1' then
        r_TX_DV <= '0';
        send_delay_counter <= 0;
    else
        if ModeTrading = '1' then
            if i_MS_SLV = '0' then
                if send_delay_counter < send_delay-2 then
                    send_delay_counter <= send_delay_counter + 1;
                    r_TX_DV <= '0';
                elsif send_delay_counter = send_delay -2 then   
                    -- Read new data paket from the Ringbuffer PS to PL 
                    rd_en_PS_to_PL <= '1';
                    r_TX_DV <= '0';
                    send_delay_counter <= send_delay_counter + 1;
                elsif send_delay_counter = send_delay -1 then    
                    r_TX_DV <= '0';
                    send_delay_counter <= send_delay_counter + 1;
                    rd_en_PS_to_PL <= '0';
                elsif send_delay_counter = send_delay then
                    --Start Transmisson via the UART Transmitter
                    r_TX_DV <= '1';
                    rd_en_PS_to_PL <= '0';
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

--UpdateTransmitter : process(clk)
--begin
--if rising_edge(clk) then
--    --rd_en_PS_to_PL <= '1';
--    if rd_valid_PS_to_PL = '1' then
--        r_TX_Word <= rd_data_PS_to_PL(15 downto 0);
--    end if;
--end if;
--end process;  



sampling :  process (clk)
 begin
    if rising_edge(clk) then
       if rst = '1' then
           wr_data_PL_to_PS <= x"00000000";
           wr_en_PL_to_PS <= '0'; 
       else
            if ModeMonitor = '1' then
                --check if new data frame was received and the communication is enabled 
                if w_RX_DV ='1' and i_enable = '0'   then
                --i_MS_SLV indicates if the GB Master or the GB Slave is sending data
                -- This section of the code writes data to the ringbuffer if both the master and slave data frame are received
                    if i_MS_SLV = '0' then
                            wr_data_PL_to_ps(15 downto 0) <= w_RX_WORD;
                            wr_en_PL_To_ps <= '1';
                    else
                            wr_data_PL_to_PS(31 downto 16) <= w_RX_WORD;
                            wr_en_PL_to_ps <= '0';
                    end if;
                else
                        wr_en_PL_to_ps <= '0';
                end if;
            else
                --ModeTrading
                --Only strores the received master paket int the RIngbuffer Pl to PS
                 if  w_RX_DV = '1' and i_enable = '0' and i_MS_SLV = '1' then
                    wr_data_PL_to_ps(15 downto 0) <= w_RX_WORD;
                    wr_data_PL_to_PS(31 downto 16) <= x"0000";
                    wr_en_PL_To_ps <= '1';
                 else
                    wr_en_PL_To_ps <= '0';
                 end if;             
            
            end if;
       end if;    
    end if;
 end process;
 
 
    --send switch state to SOftware
    PL_to_PS_buffer(0) <= switch0;
    PL_to_PS_buffer(1) <= switch1;
    PL_to_PS_buffer(2) <= switch2;
    PL_to_PS_buffer(3) <= switch3;
    PL_to_PS_buffer(4) <= resetbutton;
    PL_to_PS_buffer(5) <= tradebutton;
--    PL_to_PS_buffer(31 downto 28) <= std_logic_vector(to_unsigned(Framecounter,4));
    
    --conversion from the BRAM control signels to the ringbuffer control signals 
    ---temp(7 downto 4) <= std_logic_vector(to_unsigned(Framecounter,4));
    temp(0) <= empty_PL_TO_PS;
    --Ringbuffer Dout is set to rd_data only if data is requested at address 0 at Software level, else the empty flag is writen to dout 
    BRAM_Controller_data_in <= rd_Data_PL_TO_PS when BRAM_Controller_addr = Zero_address else temp;
    rd_en_PL_TO_PS <= (not(BRAM_Controller_wr_en(0)) and BRAM_Controller_en) when BRAM_Controller_addr = Zero_address else '0';
    
    --Data send from Ps to Pl into Ringbuffer and corrsponding signals
    wr_data_PS_to_Pl <= BRAM_Controller_data_out; 
    wr_en_Ps_to_Pl <= BRAM_Controller_wr_en(0) and BRAM_Controller_en;
    
    ---Update data for transmitter if there is a valid read from the ringbuffer PS-to-PL
    r_TX_Word <= rd_data_PS_to_PL(15 downto 0) when (rd_valid_PS_to_PL = '1') else r_TX_Word;
    
    ----------Setting the Monitoring/tRading Mode respectivly
    ModeMonitor <= PS_to_PL_buffer(0);
    ModeTrading <= not(PS_to_PL_buffer(0));
    
    ---------- Switch between Trading mode and monitoring mode. The Data signal pin has to have the right configuration as input or output
     io_Serial <= w_TX_Serial when (i_MS_SLV ='0' and ModeTrading ='1') else 'Z';
     r_RX_Serial <= io_Serial when (i_MS_SLV ='1' or  ModeMonitor = '1') else 'Z';

    --r_RX_Serial <= io_Serial;
    
    LED <= '1';
    --o_debug <= w_debug;
end behav;
