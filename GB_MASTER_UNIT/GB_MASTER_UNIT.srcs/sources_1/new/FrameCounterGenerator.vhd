----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/12/2024 12:08:41 PM
-- Design Name: 
-- Module Name: FrameCounterGenerator - Behavioral
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

entity FrameCounterGenerator is
    Port (  clk          : in STD_LOGIC;
            reset        : in std_logic;
            enable       : in std_logic;
            
            frameCounter : out integer);
end FrameCounterGenerator;

architecture Behavioral of FrameCounterGenerator is
signal timingCounter : integer := 0;
constant TimerThresholdpackage : integer :=100000; --time between two packages in 9er frame = 8 ms
constant TimerThresholdEndStart : integer := 700000; --time between end package and start package of normal 9er frames
constant TimerThresholdSyncPhase : integer := 1500000; -- time between two packages during sync Phase;

constant TimerThreshold : integer := 300000;

signal w_frameCounter : integer := 1;
constant FRAME_LENGTH : integer := 9;

signal enable_prev : std_logic ;
begin
timer : process(clk)
begin
if rising_edge(clk) then
    if enable = '0' then
        if enable_prev = '1' then
            if timingCounter > TimerThreshold then
                ---sowhl sync phase als auch end of 9er frame
                w_framecounter <= 1;
                --o_debug <= '1';
                --sync flag setting
            else
                --frameCounter++ 
                if w_frameCounter = FRAME_LENGTH then
                    w_framecounter <= 1;
                    --w_debug <= not(w_debug);
                else
                    w_framecounter <= w_framecounter +1; 
                end if;
                --o_debug <= '0';
                 --w_debug <= not(w_debug);
            end if;
            timingCounter <= 0;
            enable_prev <= '0';
           
        else
            timingCounter <= timingcounter +1;
            enable_prev <= '0';
            --o_debug <= '0';  
        end if;
    else 
        if timingCounter < 2000000 then
            timingCounter <= timingcounter +1;
        end if;
        enable_prev <= '1';
        --o_debug <= '0';
    end if;

    
end if;
end process;
--timer : process(clk)
--begin
--if rising_edge(clk) then
--    if enable = '0' then
--        if enable_prev = '1' then
--            if timingCounter > TimerThresholdSyncPhase then
--                w_framecounter <= 1;
--                --o_debug <= '1';
--                --sync flag setting
--            elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
--                w_framecounter <= 1;
--                --o_debug <= '1';
                
--            --elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
--            elsif timingCounter <= TimerThresholdEndStart then
--                --frameCounter++ 
--                if w_frameCounter = FRAME_LENGTH then
--                    w_framecounter <= 1;
--                    --w_debug <= not(w_debug);
--                else
--                    w_framecounter <= w_framecounter +1; 
--                end if;
--                --o_debug <= '0';
--                 --w_debug <= not(w_debug);
--            end if;
--            timingCounter <= 0;
--            enable_prev <= '0';
           
--        else
--            timingCounter <= timingcounter +1;
--            enable_prev <= '0';
--            --o_debug <= '0';  
--        end if;
--    else 
--        if timingCounter < 2000000 then
--            timingCounter <= timingcounter +1;
--        end if;
--        enable_prev <= '1';
--        --o_debug <= '0';
--    end if;

    
--end if;
--end process;


FrameCounter <= w_FrameCounter;

end Behavioral;
