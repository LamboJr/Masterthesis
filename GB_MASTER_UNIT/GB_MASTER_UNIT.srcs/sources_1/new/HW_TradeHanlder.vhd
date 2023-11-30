----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/24/2023 12:04:28 PM
-- Design Name: 
-- Module Name: TradeHandler - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity HWTradeHandler is
    Port ( clk : in STD_LOGIC;
           i_enable : in std_logic;
           o_data_output : out std_logic_vector(15 downto 0);
           i_data_input : in std_logic_vector(15 downto 0);
           o_response_valid : out std_logic;
           i_generate_response : in std_logic;
           o_debug : out std_logic;        

           reset : in std_logic;
           tradebutton : in std_logic
           --o_TX8_serial : out std_logic
        );
end HWTradeHandler;

architecture Behavioral of HWTradeHandler is


type t_SM_Main is (s_Idle,s_Active);

signal timingCounter : integer := 0;
constant TimerThresholdpackage : integer :=130000; --time between two packages in 9er frame
constant TimerThresholdEndStart : integer := 900000; --time between end package and start package of normal 9er frames
constant TimerThresholdSyncPhase : integer := 2000000; -- time between two packages during sync Phase;

--detect falling edge on i_enable
signal enable_flag : std_logic := '1';
signal frameCounter : integer := 0;
constant FRAME_LENGTH : integer := 9;

signal w_debug : std_logic := '1';

--
signal w_data_output : std_logic_vector(15 downto 0) := x"FFF8";
signal s_SM_Main : t_SM_Main := s_Idle;

type LinkCommands is (LinkCMD_IDLE,LinkCMD_INIT_BLOCK,LinkCMD_REQUEST_BLOCK,LinkCMD_CONT_BLOCK,LinkCMD_SEND_LINK_TYPE);
signal LinkCMD : LinkCommands := LinkCMD_IDLE;

begin
main : process(clk)
begin
if rising_edge(clk) then
    if reset = '1' then
        o_response_valid <= '0';
        w_data_output <= x"0000";
        
    else
        
        case S_SM_Main is
        when s_idle =>
            if i_generate_Response = '1' then
                s_SM_Main <= s_Active;
            else
                s_SM_Main <= s_idle;
            end if;
            o_response_valid <= '0';
            
        when s_Active =>
        
            case FrameCounter is
            when 1 =>
                w_data_output <= i_data_input;
            when 2 =>
                case i_data_input is
                when x"CCCC" => LinKCMD <= LinkCMD_REQUEST_BLOCK;
                    w_data_output <= x"0000";
                when x"BBBB" => LinkCMD <= LinkCMD_INIT_BLOCK;
                    w_data_output <= i_data_input;
                when x"8888" => LinkCMD <= LinkCMD_CONT_BLOCK;
                    w_data_output <= i_data_input;
                when x"2222" => LinkCMD <= LinkCMD_SEND_LINK_TYPE;
                    w_data_output <= x"0000";
                when others => LinkCMD <= LinkCMD_IDLE;
                    w_data_output <= i_data_input;
                end case;
                
            
            when 3 =>
                case LinkCMD is
                when LinkCMD_REQUEST_BLOCK =>
                    w_data_output <= x"0000";
                when LinkCMD_INIT_BLOCK =>
                    w_data_output <= i_data_input;
                
                when LinkCMD_CONT_BLOCK =>
                    w_data_output <= i_data_input;
                when LinkCMD_SEND_LINK_TYPE =>
                    w_data_output <= x"0000";
                when others =>
                    w_data_output <= i_data_input;
                end case;
            when 4 =>
                case LinkCMD is
                when LINKCMD_INIT_BLOCK =>
                    w_data_output <= x"0081";
                when others =>
                    w_data_output <= i_data_input;
                end case;
                
            when others =>
                w_data_output <= i_data_input;
            end case;
        
        
        
            s_SM_Main <= s_Idle;
            --w_data_output <= i_data_input;
        when others =>
            s_SM_Main <= s_Idle;
             w_data_output <= i_data_input;
           
        end case;
    
    
    
    
    end if;
end if;
end process;



timer : process(clk)
begin
if rising_edge(clk) then
    if i_enable = '0' then
        if enable_flag = '1' then
            if timingCounter > TimerThresholdSyncPhase then
                framecounter <= 2;
                --o_debug <= '1';
                --sync flag setting
            elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
                framecounter <= 1;
                --o_debug <= '1';
                w_debug <= not(w_debug);
            elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
                --frameCounter++ 
                if frameCounter = FRAME_LENGTH then
                    framecounter <= 1;
                else
                    framecounter <= framecounter +1; 
                end if;
                --o_debug <= '0';
            else
                 --w_debug <= not(w_debug);
            end if;
            timingCounter <= 0;
            enable_flag <= '0';
           
        else
            timingCounter <= timingcounter +1;
            --o_debug <= '0';  
        end if;
    else 
        timingCounter <= timingcounter +1;
        enable_flag <= '1';
        --o_debug <= '0';
    end if;
end if;
end process;

o_data_output <= w_data_output;
o_debug <= w_debug;
end Behavioral;
