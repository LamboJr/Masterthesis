-------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library work;
--use work.Constants_and_Declarations.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;


entity TopModule_TB is
--  Port ( );
end TopModule_TB;

architecture Behavioral of TopModule_TB is

 constant CLK_PERIOD : time := 8 ns;
 constant c_BIT_PERIOD : time := 8680 ns;

 signal switch0 : STD_LOGIC := '0';
 signal switch1 : STD_LOGIC := '0';
 signal switch2 : STD_LOGIC := '0';
 signal switch3 : STD_LOGIC := '0';
 
 signal resetbutton : std_logic := '0';
 signal tradebutton : std_logic := '0';
 signal io_Serial :  STD_LOGIC := 'Z';

 signal i_MS_SLV : std_logic := '0';
 signal i_Enable : std_logic := '0';
 --signals for connecting the Ringbuffer(s) to the RTL and the ZYNQ processor
signal BRAM_Controller_Addr : std_logic_vector(12 downto 0) := (others => '0');
signal clk : std_logic := '0';
signal BRAM_Controller_Data_in : std_logic_vector( 31 downto 0) := (others => '0');
signal BRAM_Controller_Data_out : std_logic_vector( 31 downto 0) := (others => '0');
signal BRAM_Controller_en : std_logic := '0';
signal rst : std_logic :='0';
signal BRAM_Controller_wr_en : std_logic_vector(3 downto 0) := (others => '0');
--32 Bit signal for transmitting data from PL to PS
signal PL_to_PS_buffer : std_logic_vector(31 downto 0) := (others => '0');

--32 Bit signal for transmitting data from PS to PL
signal PS_to_PL_buffer : std_logic_vector(31 downto 0) := (others => '0'); 

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
    o_serial <= 'Z';
    end UART_WRITE_HWORD;
    
 
begin
        TOP_INST : entity work.Moduletop_DUT
    port map(
        switch0 => switch0,
        switch1 => switch1,
        switch2 => switch2,
        switch3 => switch3,
        resetbutton => resetbutton,
        tradebutton => tradebutton,
        io_Serial => io_Serial,
        i_MS_SLV => i_MS_SLV,
        i_Enable => i_Enable,
        BRAM_PORTA_0_addr => BRAM_Controller_Addr,
        BRAM_PORTA_0_clk => clk,
        BRAM_PORTA_0_din => BRAM_Controller_data_out,
        BRAM_PORTA_0_dout => BRAM_Controller_data_in,
        BRAM_PORTA_0_en => BRAM_Controller_en,
        BRAM_PORTA_0_rst => rst,
        BRAM_PORTA_0_we => BRAM_Controller_wr_en,
        PL_to_PS_buffer => PL_to_PS_buffer,
        PS_to_PL_buffer => PS_to_PL_buffer

    );
    
    clk_gen: process 
     begin
     clk <= not clk;
     wait for CLK_PERIOD/2;
     end process clk_gen; 
     
     
    stimuli : process
    begin 
    
    wait until rising_edge(clk);
    rst <= '1';
    wait until rising_edge(clk);
    rst <= '0';
    wait until rising_edge(clk);
    ----Test ModeMonitor
    PS_to_PL_buffer<= (others => '1');
    wait until rising_edge(clk);
    i_Enable <= '0';
    i_MS_SLV <= '1';
    UART_WRITE_HWORD(X"80F0", io_serial);
    wait for 9us;
    wait until rising_edge(clk);
    i_MS_SLV <= '0';
    UART_WRITE_HWORD(X"ABCD", io_serial);
    wait for 9us;
    i_Enable <= '1';
    i_MS_SLV <= '1';
    wait until rising_edge(clk);
    BRAM_Controller_Addr(0) <= '1'; 
    BRAM_Controller_en <= '1';
    BRAM_Controller_wr_en(0) <= '0'; 
    wait until rising_edge(clk);
    if BRAM_Controller_data_in(0) = '0' then
        report "Test Passed - Correct Data Received" severity note;
    else
        report "Test Failed - Incorrect Data Received" severity note;
    end if;
    BRAM_Controller_en <= '0';
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);    
    BRAM_Controller_Addr(0) <= '0'; 
    BRAM_Controller_en <= '1';
    BRAM_Controller_wr_en(0) <= '0'; 
    wait until rising_edge(clk);
    BRAM_Controller_en <= '0';
    wait until rising_edge(clk);
    if BRAM_Controller_data_in = x"80F0ABCD" then
        report "Test Passed - Correct Data Received" severity note;
    else
        report "Test Failed - Incorrect Data Received" severity note;
    end if;
    wait for 80ns;
    
    --test ModeTrade
    PS_to_PL_buffer<= (others => '0');
    wait until rising_edge(clk);
    i_Enable <= '0';
    i_MS_SLV <= '1';
    UART_WRITE_HWORD(X"8FFF", io_serial);
    wait until rising_edge(clk);
    i_MS_SLV <= '0';
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    ---Read empty flag
    BRAM_Controller_Addr(0) <= '1'; 
    BRAM_Controller_en <= '1';
    BRAM_Controller_wr_en(0) <= '0'; 
    wait until rising_edge(clk);
    BRAM_Controller_en <= '0';
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);    
    ---Read next buffer value
    BRAM_Controller_Addr(0) <= '0'; 
    BRAM_Controller_en <= '1';
    BRAM_Controller_wr_en(0) <= '0'; 
    wait until rising_edge(clk);
    BRAM_Controller_en <= '0';
        if BRAM_Controller_data_in(0) = '0' then
        report "Test Passed - Correct Data Received" severity note;
    else
        report "Test Failed - Incorrect Data Received" severity note;
    end if;
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    if BRAM_Controller_data_in = x"00008FFF" then
        report "Test Passed - Correct Data Received" severity note;
    else
        report "Test Failed - Incorrect Data Received" severity note;
    end if;
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    wait until rising_edge(clk);
    BRAM_Controller_data_out <= x"0000B9A0";
    
    --Wirte to Buffer from SW
    BRAM_Controller_Addr(0) <= '0'; 
    BRAM_Controller_en <= '1';
    BRAM_Controller_wr_en(0) <= '1'; 
    wait until rising_edge(clk);
    BRAM_Controller_en <= '0';BRAM_Controller_wr_en(0) <= '0'; 
    wait until rising_edge(clk);
    wait for 3000 * CLK_Period;
    wait for 20us;
    assert false report "Tests Complete" severity failure;
    end process;

end Behavioral;
