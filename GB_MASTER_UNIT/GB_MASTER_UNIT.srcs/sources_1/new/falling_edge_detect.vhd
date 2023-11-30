----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/23/2023 10:40:21 PM
-- Design Name: 
-- Module Name: falling_edge_detect - Behavioral
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

entity falling_edge_detect is
    Port ( i_enable : in STD_LOGIC;
           clk : in std_logic;
           LED : out std_logic);
end falling_edge_detect;

architecture Behavioral of falling_edge_detect is

signal timingCounter : integer := 0;
constant TimerThresholdpackage : integer :=150000; --time between two packages in 9er frame
constant TimerThresholdEndStart : integer := 950000; --time between end package and start package of normal 9er frames
constant TimerThresholdSyncPhase : integer := 2100000; -- time between two packages during sync Phase;

--detect falling edge on i_enable
signal enable_flag : std_logic := '1';
signal frameCount : integer := 0;

begin


timer : process(clk)
begin
if rising_edge(clk) then
    if i_enable = '0' and enable_flag = '1' then
        if timingCounter > TimerThresholdSyncPhase then
            LED <= '1';
            frameCount <= 0;
            --sync flag setting
        elsif timingCounter > TimerThresholdEndStart and timingCounter <= TimerThresholdSyncPhase then
            --framecounter = 0
            frameCount <= 1;
            LED <= '0';
        elsif timingCounter > TimerThresholdpackage and timingCounter <= TimerThresholdEndStart then
             --frameCounter++#
             framecount <= framecount + 1;     
             LED <= '0'; 
        end if;
        timingCounter <= 0;
        enable_flag <= '0';
    else 
        timingCounter <= timingcounter +1;
        if i_enable = '1' then
            enable_flag <= '1';
        end if;
    end if;
end if;
end process;
end Behavioral;
