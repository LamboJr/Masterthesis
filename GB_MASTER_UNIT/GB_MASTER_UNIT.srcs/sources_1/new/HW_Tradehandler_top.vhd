

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;


entity HWTop is 
      Port(--sysclk : in STD_LOGIC;
        --o_TX_Serial : out STD_LOGIC;
        clk : in std_logic;
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
end HWTop;

architecture behav of HWTop is


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


signal ModeMonitor : std_logic;
signal ModeTrading : std_logic := '1';


--TradeHandler signals
signal w_response_valid : std_logic;
signal r_generate_response : std_logic;
signal w_data_output : std_logic_vector(15 downto 0) := x"FFF8";  --Signal for data which gets send to UART TX Component to transmit
signal r_data_input :std_logic_vector(15 downto 0) := x"FFF8";   ---signal for newly received data via the UART RX module

signal send_delay_counter : integer := 0;
    
signal w_debug : std_logic := '0';

constant send_delay : integer := 1000;

signal reset : std_logic := '0';
signal timingCounter : integer := 0;
constant TimerThresholdpackage : integer :=100000; --time between two packages in 9er frame
constant TimerThresholdEndStart : integer := 700000; --time between end package and start package of normal 9er frames
constant TimerThresholdSyncPhase : integer := 1500000; -- time between two packages during sync Phase;

signal frameCounter : integer := 1;
constant FRAME_LENGTH : integer := 9;

type t_framingState is (s_idle,s_Processing,s_Holding);
signal framingState : t_framingState := s_idle;


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

TradeHandler_inst : entity work.TradeHandler
port map(
    clk => clk,
    i_enable => i_enable,
    o_data_output => w_data_output,
    i_data_input => r_data_input,
    o_response_valid => w_response_valid,
    i_generate_response => r_generate_response,
    reset => reset,
    tradebutton => tradebutton,
    o_debug => open
);


frameing : process(clk)
begin
    if rising_edge(clk) then
        case FramingState is
        when s_Idle => 
            if i_enable = '0' then
                Framingstate <= s_processing;
            end if;
            timingcounter <= timingcounter +1;
           
        when s_Processing =>
            if timingCounter > TimerThresholdSyncPhase then
                framecounter <= 1;
            elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
                framecounter <= 1;
                
            --elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
            else
                if frameCounter = FRAME_LENGTH then
                    framecounter <= 1;
                else
                    framecounter <= framecounter +1; 
                end if;
            end if;
            timingcounter <= 0;
            FramingState <= s_holding;
            
        when s_holding =>
            if i_enable = '1' then
                framingstate <= s_Idle;
                w_debug <= not(w_debug);
            end if;
            timingcounter <= timingcounter +1;
            
        when others =>
            timingcounter <= timingcounter +1;
        end case;
    end if;
end process;

sample: process(clk)
begin
if rising_edge(clk) then
    if  w_RX_DV = '1' and i_enable = '0' and i_MS_SLV = '1' then
        r_generate_response <= '1';
        r_data_input <= w_RX_WORD;
        --wr_data(15 downto 0) <= w_RX_HWORD;
        --wr_en <= '1';
    else
        r_generate_response <= '0';
        --wr_en <= '0';
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

    
     io_Serial <= w_TX_Serial when (i_MS_SLV ='0') else 'Z';
     r_RX_Serial <= io_Serial when (i_MS_SLV ='1') else 'Z';

    --r_RX_Serial <= io_Serial;
    
    o_debug <= w_debug;
end behav;


--experimental
--frameing : process(clk)
--begin
--    if rising_edge(clk) then
    
--        case FramingState is
--        when s_Idle => 
--            if i_enable = '0' then
--                Framingstate <= s_processing;
--                w_debug <= '0';
--            else
--                w_Debug <= '1';
--            end if;
--            timingcounter <= timingcounter +1;
           
--        when s_Processing =>
--            if timingCounter > TimerThresholdSyncPhase then
--                --framecounter <= 1;
----            elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
----                framecounter <= 1;
                
--            --elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
--            else
--                w_debug <= '1';
--                if frameCounter = FRAME_LENGTH then
--                    framecounter <= 1;
--                else
--                    framecounter <= framecounter +1; 
--                end if;
--            end if;
--            FramingState <= s_resetCounter;
--        when s_resetcounter =>
--            timingcounter <= 0;
--            FramingState <= s_holding;
--        when s_holding =>
--            if i_enable = '1' then
--                framingstate <= s_Idle;
--                ---w_debug <= '1';
--                w_debug <= not(w_debug);
--            end if;
--            timingcounter <= timingcounter +1;
            
--        when others =>
--            timingcounter <= timingcounter +1;
--        end case;
--    end if;
--end process;



--framing : process(clk)
--begin
--if rising_edge(clk) then
--    if i_enable = '0' then
--        if enable_flag = '1' then
            
--            case SyncState is
--            when s_Sync =>
--                if i_data_input = x"8FFF" then
--                    SyncState <= s_handling;
--                    frameCounter <= 1;
--                else
--                    SyncState <= s_Sync;
--                end if;
--            when s_Handling =>
--                if i_data_input = x"5FFF" then
--                    SyncState <= s_Sync;
--                    frameCounter <= 1;
--                else
--                    SyncState <= s_Handling;
--                    if frameCounter = FRAME_LENGTH then
--                        framecounter <= 1;
--                    --w_debug <= not(w_debug);
--                    else
--                        framecounter <= framecounter +1; 
--                    end if;
--                end if;
--            when others =>
            
--            end case;
--          else
----            timingCounter <= timingcounter +1;
--                enable_flag <= '0';
--          end if;
--    else
--        enable_flag <= '1';
--        --timingCounter <= timingcounter +1;
--    end if;
    

--end if;
--end process;

--timer : process(clk)
--begin
--if rising_edge(clk) then
--    if i_enable = '0' then
--        if enable_flag = '1' then
--            if timingCounter > TimerThresholdSyncPhase then
--                framecounter <= 2;
--                --o_debug <= '1';
--                --sync flag setting
--            elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
--                framecounter <= 1;
--                --o_debug <= '1';
                
--            --elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
--            elsif timingCounter <= TimerThresholdEndStart then
--                --frameCounter++ 
--                if frameCounter = FRAME_LENGTH then
--                    framecounter <= 1;
--                    --w_debug <= not(w_debug);
--                else
--                    framecounter <= framecounter +1; 
--                end if;
--                --o_debug <= '0';
--                 --w_debug <= not(w_debug);
--            end if;
--            timingCounter <= 0;
--            enable_flag <= '0';
           
--        else
--            timingCounter <= timingcounter +1;
--            enable_flag <= '0';
--            --o_debug <= '0';  
--        end if;
--    else 
--        timingCounter <= timingcounter +1;
--        enable_flag <= '1';
--        --o_debug <= '0';
--    end if;

    
--end if;
--end process;