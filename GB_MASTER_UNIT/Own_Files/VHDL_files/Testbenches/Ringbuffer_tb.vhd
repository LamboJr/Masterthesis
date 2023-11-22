----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 10/16/2023 12:53:28 PM
-- Design Name: 
-- Module Name: Ringbuffer_tb - Behavioral
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
library work;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Ringbuffer_tb is
--  Port ( );
end Ringbuffer_tb;

architecture Behavioral of Ringbuffer_tb is

signal r_CLOCK     : std_logic                    := '0';
constant CLK_PERIOD : time := 8 ns;

constant RAM_WIDTH : natural := 32;
constant RAM_DEPTH : natural := 2048;
 
signal rst : std_logic;

signal wr_en : std_logic;
signal wr_data : std_logic_vector(RAM_WIDTH - 1 downto 0);

signal rd_en : std_logic;
signal rd_valid : std_logic;
signal rd_data : std_logic_vector(RAM_WIDTH - 1 downto 0);

signal empty : std_logic;
signal empty_next : std_logic;
signal full : std_logic;
signal full_next : std_logic;

signal fill_count : integer range RAM_DEPTH -1 downto 0;

begin
ringbuffer_inst : entity work.ringbuffer
generic map (RAM_DEPTH => RAM_DEPTH,RAM_WIDTH => RAM_WIDTH)
port map(
clk => r_CLOCK,
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

clk_gen: process 
begin
r_CLOCK <= not r_CLOCK;
wait for CLK_PERIOD/2;
end process clk_gen; 


stimuli : process
begin

--reset stimuli
rst <= '0';
wait until rising_edge(r_Clock);
rst <= '1';
wait until rising_edge(r_Clock);
rst <= '0';
wait until rising_edge(r_Clock);

-- fill buffer with data
wr_en <= '1';
wr_data <= x"00000001";
wait until rising_edge(r_Clock);
wr_data <= x"00000002";
wait until rising_edge(r_Clock);
wr_data <= x"00000003";
wait until rising_edge(r_Clock);
RD_EN <= '1';
wr_data <= x"00000004";
wait until rising_edge(r_Clock);
wr_data <= x"00000005";
wait until rising_edge(r_Clock);
wr_data <= x"00000006";
wait until rising_edge(r_Clock);
wr_en <= '0';


wait until rising_edge(r_CLOCK);
rd_En <= '1';

wait for 1000us;
    
    
    
assert false report "Tests Complete" severity failure;
end process;

end Behavioral;
