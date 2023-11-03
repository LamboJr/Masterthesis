

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;
use IEEE.NUMERIC_STD.ALL;


entity ModuleTop is 
      Port(--sysclk : in STD_LOGIC;
        --o_TX_Serial : out STD_LOGIC;
        io_Serial : in STD_LOGIC;
        i_MS_SLV : in std_logic;
        i_Enable : in std_logic;
        resetbutton : in std_logic;
        --switch3  : in std_logic;
        LED : out std_logic);
end ModuleTop;

architecture behav of ModuleTop is

--signals for connecting the BRAM to the RTL and the ZYNQ processor
signal bram_porta_0_addr : std_logic_vector(12 downto 0);
signal clk : std_logic;
signal bram_porta_0_din : std_logic_vector( 31 downto 0);
signal bram_porta_0_dout : std_logic_vector( 31 downto 0);
signal bram_porta_0_en : std_logic;
signal rst : std_logic;
signal bram_porta_0_we : std_logic_vector(3 downto 0);

--Bram Port b
signal bram_portb_0_addr : std_logic_vector(10 downto 0) := (others => '0');
signal bram_portb_0_din : std_logic_vector(31 downto 0) := (others => '0');
signal bram_portb_0_we : std_logic := '0';

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
      i_TX_Word   => r_TX_Word,
      o_TX_Active => w_TX_Active,
      --o_TX_Serial => o_TX_Serial,
      o_TX_Serial => w_TX_Serial,
      o_TX_Done   => w_TX_DONE
      ); 




--zynq system block
zynq_ps_interface_inst: entity work.GB_UNIT_design_wrapper
    port map (
        BRAM_PORTA_0_addr => bram_porta_0_addr,
        BRAM_PORTA_0_clk => clk,
        BRAM_PORTA_0_din => bram_porta_0_din,
        BRAM_PORTA_0_dout => bram_porta_0_dout,
        BRAM_PORTA_0_en => bram_porta_0_en,
        BRAM_PORTA_0_rst => rst,
        BRAM_PORTA_0_we => bram_porta_0_we);
        
-- BRAM memory 2K by 32 bit

blk_mem_gen0_inst: entity work.blk_mem_gen_0
    port map (
    --zynq Ps access through software
    clka => clk,
    ena => bram_porta_0_en,
    wea => bram_porta_0_we( 0 downto 0),
    addra => bram_porta_0_addr(12 downto 2),
    dina => bram_porta_0_din,
    douta => bram_porta_0_dout,
    --PL fabric access
    clkb => clk,
    enb => '1',
    web(0) => bram_portb_0_we,
    addrb => bram_portb_0_addr,
    dinb => bram_portb_0_din,
    doutb => open);
    
-- process (clk)
-- begin
--    if rising_edge(clk) then
--       if rst = '1' then
--            bram_portb_0_addr <= (others => '0');
--            bram_portb_0_din <= (others => '0');
--            bram_portb_0_we <= '0';
--       else
--            if (bram_portb_0_addr /= x"7FF") then
--                bram_portb_0_addr <= bram_portb_0_addr + 1;
--                bram_portb_0_din <= bram_portb_0_din +1;
--                bram_portb_0_we <= '1';
--            else
--                bram_portb_0_we <= '0';
--            end if;
--       end if;    
--    end if;
-- end process;
 
 process (clk)
 begin
    if rising_edge(clk) then
       if rst = '1' then
            bram_portb_0_addr <= (others => '0');
            bram_portb_0_din <= (others => '0');
            bram_portb_0_we <= '0';
       else

            if w_RX_DV ='1' and i_enable = '0' then
                --if i_MS_SLV = '0' then
                        bram_portb_0_din(15 downto 0) <= w_RX_WORD;
                        --bram_portb_0_din(31 downto 16) <= x"1111";
                        bram_portb_0_we <= '1';
                        if (bram_portb_0_addr /= x"7FF") then
                            bram_portb_0_addr <= bram_portb_0_addr + 1;
                        else
                            bram_portb_0_addr <= (others => '0');
                        end if;
               -- else
                        --bram_portb_0_din(31 downto 16) <= w_RX_HWORD;
                        --bram_portb_0_we <= '0';
                --end if;
            else
                    bram_portb_0_we <= '0';
            end if;
            
       end if;    
    end if;
 end process;
 r_RX_Serial <= io_Serial;
    
end behav;