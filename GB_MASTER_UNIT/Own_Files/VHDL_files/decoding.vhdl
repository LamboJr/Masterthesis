library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library work;
use work.Constants_and_Declarations.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;

entity decoding is
    Port (clk : in std_logic;
          rst : in std_logic;
          i_data_input : in std_logic_vector(15 downto 0);
          i_decode_en : in std_logic;
          o_decode_valid : out std_logic
          );
end entity;

architecture Behavioral of decoding is
    signal Team : Team_structure;
    type   t_SM_Main is (s_Idle,s_handshake);
    signal r_SM_Main : t_SM_Main := s_Prepare;
begin


decode : process(clk)
begin
if rising_edge(clk) then
    if rst = '1' then
        r_SM_main <= s_Idle;
    else
        case r_SM_MAIN is
        
        when s_Idle =>
            if i_decode_en = '1' then
                
            else
                r_SM_main <= s_idle;
            end if;
            o_decode_valid <= '0';
        when others => r_SM_main <= s_idle;
        end case;
    end if;
end if;
end process;

end Behavioral;