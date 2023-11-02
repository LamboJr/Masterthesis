----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 07/15/2023 12:17:24 PM
-- Design Name: 
-- Module Name: UART_TB - Behavioral
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity UART_TB is
--  Port ( );
end UART_TB;

architecture Behavioral of UART_TB is
    component UART_TX is
        generic (
            g_CLKS_PER_BIT : integer := 1085   -- Needs to be set correctly
        );
        port (
        sysclk       : in  std_logic;
        i_TX_DV     : in  std_logic;
        i_TX_HWord   : in  std_logic_vector(15 downto 0);
        o_TX_Active : out std_logic;
        o_TX_Serial : out std_logic;
        o_TX_Done   : out std_logic
        );
    end component uart_tx;
 
    component UART_RX is
        generic (
            g_CLKS_PER_BIT : integer := 1085   -- Needs to be set correctly
        );
        port (
        sysclk       : in  std_logic;
        i_RX_Serial : in  std_logic;
        o_RX_DV     : out std_logic;
        o_RX_HWord   : out std_logic_vector(15 downto 0)
        );
    end component uart_rx;
  
   
    -- Test Bench uses a 125 MHz Clock
    -- Want to interface to 115200 baud UART
    -- 125000000 / 115200 = 1085 Clocks Per Bit.
    constant c_CLKS_PER_BIT : integer := 1085;
    
    constant CLK_PERIOD : time := 8 ns;
 
    constant c_BIT_PERIOD : time := 8680 ns;  --- 1/BAUD RAte
   
    signal r_CLOCK     : std_logic                    := '0';
    signal r_TX_DV     : std_logic                    := '0';
    signal r_TX_HWORD   : std_logic_vector(15 downto 0) := (others => '0');
    signal w_TX_SERIAL : std_logic;
    signal w_TX_DONE   : std_logic;
    signal w_RX_DV     : std_logic;
    signal w_RX_HWORD   : std_logic_vector(15 downto 0);
    signal r_RX_SERIAL : std_logic := '1'; 
  
    -- Low-level byte-write
    procedure UART_WRITE_HWORD (
        i_data_in       : in  std_logic_vector(15 downto 0);
        signal o_serial : out std_logic) is
    begin
 
    -- Send Start Bit
    o_serial <= '0';
    wait for c_BIT_PERIOD;
 
    -- Send Data Byte
    for i in 0 to 15  loop
      o_serial <= i_data_in(i);
      wait for c_BIT_PERIOD;
    end loop;  -- i
 
    -- Send Stop Bit
    o_serial <= '1'; 
    wait for c_BIT_PERIOD; 
    
    end UART_WRITE_HWORD;
  
   
begin 
    -- Instantiate UART transmitter 
    UART_TX_INST : uart_tx generic map ( g_CLKS_PER_BIT => c_CLKS_PER_BIT
    )
    port map (
      sysclk       => r_CLOCK,
      i_TX_DV     => r_TX_DV,
      i_TX_HWord   => r_TX_HWORD,
      o_TX_Active => open,
      o_TX_Serial => w_TX_SERIAL,
      o_TX_Done   => w_TX_DONE
      );
 
  -- Instantiate UART Receiver
    UART_RX_INST : uart_rx
    generic map (
      g_CLKS_PER_BIT => c_CLKS_PER_BIT
      )
    port map (
      sysclk       => r_CLOCK,
      i_RX_Serial => r_RX_SERIAL,
      o_RX_DV     => w_RX_DV,
      o_RX_HWord   => w_RX_HWORD
      );
 
 
    clk_gen: process 
    begin
        r_CLOCK <= not r_CLOCK;
        wait for CLK_PERIOD/2;
    end process clk_gen; 
    stimuli : process
    begin
 
        -- Tell the UART to send a command.
        wait until rising_edge(r_CLOCK);
        wait until rising_edge(r_CLOCK);
        r_TX_DV   <= '1';
        r_TX_HWORD <= X"ABCD";
        wait until rising_edge(r_CLOCK);
        r_TX_DV   <= '0';
        wait until w_TX_DONE = '1';
 
     
        -- Send a command to the UART
        wait until rising_edge(r_CLOCK);
        UART_WRITE_HWORD(X"3FCD", r_RX_SERIAL);
        wait until rising_edge(r_CLOCK);
 
        -- Check that the correct command was received
        if w_RX_HWORD = X"3FCD" then
            report "Test Passed - Correct Data Received" severity note;
        else
            report "Test Failed - Incorrect Data Received" severity note;
        end if;
 
        assert false report "Tests Complete" severity failure;
     
  end process;

end Behavioral;