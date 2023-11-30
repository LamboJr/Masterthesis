----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/23/2023 10:43:18 PM
-- Design Name: 
-- Module Name: Detect_tb - Behavioral
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

entity Detect_tb is
--  Port ( );
end Detect_tb;

architecture Behavioral of Detect_tb is
signal clk : std_logic                    := '0';
constant CLK_PERIOD : time := 8 ns;

signal i_enable : std_logic;
signal LED : std_logic;

begin

inst : entity work.falling_edge_detect
port map(
clk => clk,
LED => LED,
i_enable => i_enable);



clk_gen: process 
begin
clk<= not clk;
wait for CLK_PERIOD/2;
end process clk_gen; 

stimuli : process
begin
wait until rising_edge(clk);
i_enable <= '1';

wait for 17 ms;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 17 ms;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 7431 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';

wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';
wait for 1166 us;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';

wait for 17 ms;
i_enable <= '0';
wait for 361 us;
i_enable <= '1';

assert false report "Tests Complete" severity failure;

end process;


end Behavioral;
