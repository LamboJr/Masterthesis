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

entity UART_RX is
    generic (
        g_CLKS_PER_BIT : integer := 1085);       ---Needs to be set to (sysclk freqeuncy)/(Frequency of UART, BAUD RATE) =1085.069
    Port ( sysclk : in STD_LOGIC;
           i_RX_Serial : in STD_LOGIC;
           o_RX_DV : out STD_LOGIC;
           o_RX_Word : out STD_LOGIC_VECTOR (15 downto 0));
end UART_RX;

architecture Behavioral of UART_RX is
    type t_SM_Main is (s_Idle, s_RX_Start_Bit, s_RX_Data_Bits, s_RX_Stop_Bit, s_Cleanup);
    
    signal r_SM_Main : t_SM_Main := s_Idle;
    
    signal r_RX_Data_R : std_logic := '0';
    signal r_RX_Data : std_logic := '0';
    
    signal r_CLK_Count : integer range 0 to g_CLKS_PER_BIT - 1 := 0;
    signal r_Bit_Index : integer range 0 to 15 := 0 ; --- 16 Data bits total
    signal r_RX_Word : std_logic_vector(15 downto 0) := (others => '0');
    signal r_RX_DV : std_logic := '0';
    
begin
    p_SAMPLE : process (sysclk)
    begin
        if rising_edge(sysclk) then
            r_RX_Data_R <= i_RX_Serial;
            r_RX_Data   <= r_RX_Data_R;
        end if;
    end process p_SAMPLE; -- Purpose: Control RX state machine 
    p_UART_RX : process (sysclk) 
    begin 
        if rising_edge(sysclk) then 
            case r_SM_Main is 
            when s_Idle =>
                r_RX_DV     <= '0';
                r_Clk_Count <= 0;
                r_Bit_Index <= 0;
                if r_RX_Data = '0' then       -- Start bit detected
                    r_SM_Main <= s_RX_Start_Bit;
                else
                     r_SM_Main <= s_Idle; 
                end if; -- Check middle of start bit to make sure it's still low 
            when s_RX_Start_Bit =>
                if r_Clk_Count = (g_CLKS_PER_BIT-1)/2 then
                    if r_RX_Data = '0' then
                        r_Clk_Count <= 0;  -- reset counter since we found the middle
                        r_SM_Main   <= s_RX_Data_Bits;
                    else
                        r_SM_Main   <= s_Idle;
                    end if;
                else
                    r_Clk_Count <= r_Clk_Count + 1;
                    r_SM_Main   <= s_RX_Start_Bit; 
                end if; -- Wait g_CLKS_PER_BIT-1 clock cycles to sample serial data 
            when s_RX_Data_Bits =>
                if r_Clk_Count < g_CLKS_PER_BIT-1 then
                    r_Clk_Count <= r_Clk_Count + 1;
                    r_SM_Main   <= s_RX_Data_Bits;
                else
                    r_Clk_Count            <= 0;
                    r_RX_Word(r_Bit_Index) <= r_RX_Data;
                    -- Check if we have sent out all bits
                    if r_Bit_Index < 15 then
                        r_Bit_Index <= r_Bit_Index + 1;
                        r_SM_Main   <= s_RX_Data_Bits;
                    else
                        r_Bit_Index <= 0;
                        r_SM_Main   <= s_RX_Stop_Bit; 
                    end if; 
                end if; -- Receive Stop bit. Stop bit = 1 
            when s_RX_Stop_Bit =>
            -- Wait g_CLKS_PER_BIT-1 clock cycles for Stop bit to finish
                if r_Clk_Count < g_CLKS_PER_BIT-1 then
                    r_Clk_Count <= r_Clk_Count + 1;
                    r_SM_Main   <= s_RX_Stop_Bit;
                else
                    r_RX_DV     <= '1';
                    r_Clk_Count <= 0;
                    r_SM_Main   <= s_Cleanup; 
                end if; -- Stay here 1 clock 
            when s_Cleanup =>
                r_SM_Main <= s_Idle;
                r_RX_DV   <= '0'; 
            when others =>
                r_SM_Main <= s_Idle;
 
            end case;
        end if;
     end process p_UART_RX;
    o_RX_DV <= r_RX_DV;
    o_RX_Word <= r_RX_Word;

end Behavioral;
