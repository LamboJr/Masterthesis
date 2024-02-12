----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/12/2024 01:51:11 PM
-- Design Name: 
-- Module Name: FrameCounterGenerator_TB - Behavioral
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

entity FrameCounterGenerator_TB is
--  Port ( );
end FrameCounterGenerator_TB;

architecture Behavioral of FrameCounterGenerator_TB is
signal clk    : std_logic                    := '0';
constant CLK_PERIOD : time := 8 ns;

signal reset : std_logic;
signal enable : std_logic;
signal Framecounter : integer := 0;


begin

FramecounterGenerator_Inst : entity work.FramecounterGenerator
port map (
    clk => clk,
    reset => reset,
    enable => enable,
    FrameCounter => Framecounter
);



clk_gen: process 
begin
clk <= not clk;
wait for CLK_PERIOD/2;
end process clk_gen; 

stimuli : process
begin

    enable <= '1';
    wait for 32 ns;
   
   wait until rising_edge(clk);
   enable <= '0';
   wait until rising_edge(clk);
   enable <= '1';
   wait until rising_edge(clk);
   wait for 400 us;
   
   wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
   wait for 10ms;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 800us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 800us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 800us;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   

   
   wait for 10ms;
   wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
      wait for 18ms;
    wait until rising_edge(clk);
   enable <= '0';
   wait for 40us;
   wait until rising_edge(clk);
   enable <= '1';
   
   
   
   
    wait for 1000us;
          
    assert false report "Tests Complete" severity failure;

end process stimuli;
end Behavioral;
