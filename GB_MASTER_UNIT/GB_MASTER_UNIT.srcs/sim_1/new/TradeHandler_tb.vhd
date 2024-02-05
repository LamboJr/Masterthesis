----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/24/2023 12:56:38 PM
-- Design Name: 
-- Module Name: TradeHandler_tb - Behavioral
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
use work.Constants_and_Declarations.all;
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TradeHandler_tb is
--  Port ( );
end TradeHandler_tb;

architecture Behavioral of TradeHandler_tb is

    signal clk : std_logic                    := '0';
    constant CLK_PERIOD : time := 8 ns;
    
    signal i_enable : std_logic := '1';
    signal o_data_output : std_logic_vector(15 downto 0);
    signal i_data_input : std_logic_vector(15 downto 0) := x"0000";
    signal o_response_valid : std_logic;
    signal i_generate_response : std_logic := '0';
    signal o_debug : std_logic;        

    signal frameCounter : integer;

    signal reset : std_logic;
    signal tradebutton : std_logic;
    
    signal PS_to_PL_buffer : std_logic_vector(31 downto 0) := (others => '0');
    
signal BlockRequestActive : std_logic;
signal BlockInitActive : std_logic;
signal BlockDatalength : integer;
signal Blockrequesttype : t_BlockRequestType;

signal DataInputBuffer : t_BufferSize110 := (others => x"0003");

signal team_spot : natural range 0 to TEAM_SIZE := 0;
signal team_buffer_index : natural range 0 to (PKMN_BUFFER_DEPTH ) := 0;

--buffer counter for indexing position of buffer
signal buffer_index : natural range 0 to 110 := 0;

signal Team : Team_structure := (Zapdos_buffer,Milotic_buffer,Milotic_buffer,No_Pokemon_buffer,Milotic_buffer,Zapdos_buffer);
signal team_index : natural range 0 to TEAM_SIZE -1 := 0;

signal Team_new : Team_structure := (Zapdos_buffer,Milotic_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer);
signal Team_valid : std_logic;

signal Team2PokemonBuffer : t_TwoPokemonbuffer := (EmptyBuffer110,EmptyBuffer110,EmptyBuffer110);
signal TrainerDataSize110 : t_BufferSize110 := (others => x"0000");

signal BlockRequestPokemonIndex : natural range 0 to 2;
signal TradeSpotNumber : std_logic_vector(2 downto 0) := "001";
    
    procedure Generate_Input (
        i_data_in : in  std_logic_vector(15 downto 0);
        signal w_data_out : out std_logic_vector(15 downto 0);
        signal w_generate_response : out std_logic) is
    begin
    wait until rising_edge(clk);
    w_data_out <= i_data_in;
    wait until rising_edge(clk);
    w_generate_response <= '1';
    wait until rising_edge(clk);
    w_generate_response <= '0';
    wait for 10 * CLK_PERIOD;

    end Generate_Input;
    
    
begin



TradeHandler_inst : entity work.TradeHandler
port map(
    clk => clk,
    o_data_output => o_data_output,
    i_data_input => i_data_input,
    o_response_valid => o_response_valid,
    i_generate_response => i_generate_response,
    reset => reset,
    tradebutton => tradebutton,
    o_debug => o_debug,
    frameCounter => frameCounter,
    BlockrequestType => Blockrequesttype,
    BlockDataLength => BlockDataLength,
    BlockrequestActive => BlockRequestActive,
    BlockInitActive => BlockInitActive,
    DataInputBuffer => DataInputbuffer,
    BlockRequestPokemonIndex => BlockRequestPokemonIndex,
    tradespotnumber => tradespotnumber
);

UpdateBuffer : process(clk)
begin
    if rising_edge(clk) then
        if BlockRequestActive = '1'  and BlockInitActive = '1' then
            case BlockRequestType is
                when BLOCK_REQ_SIZE_200 =>
                    DataInputBuffer <= Team2Pokemonbuffer(BlockRequestPokemonIndex);
                        
                when BLOCK_REQ_SIZE_100 =>
                    DataInputBuffer <= TrainerDataSize110;

                when others=>
                     
                end case;
        end if;
    end if;
end process;


    
    Fill_Team2PokemonBuffer : process(clk)
    begin
    if rising_edge(clk) then
        if (PS_to_PL_buffer(7 downto 4) = 8) then
            if buffer_index < 50 then
                Team2Pokemonbuffer(0)(buffer_index) <= Team(0)(buffer_index);
                Team2Pokemonbuffer(0)(buffer_index+50) <= Team(1)(buffer_index);
                Team2Pokemonbuffer(1)(buffer_index) <= Team(2)(buffer_index);
                Team2Pokemonbuffer(1)(buffer_index+50) <= Team(3)(buffer_index);
                Team2Pokemonbuffer(2)(buffer_index) <= Team(4)(buffer_index);
                Team2Pokemonbuffer(2)(buffer_index+50) <= Team(5)(buffer_index);
                TrainerDataSize110(buffer_index) <= Trainer_buffer(buffer_index);
                buffer_index <= buffer_index +1;
            end if;
        else    
            buffer_index <= 0;
        end if;
    end if;
    end process;


clk_gen: process 
begin
clk<= not clk;
wait for CLK_PERIOD/2;
end process clk_gen;




stimuli : process
begin
wait until rising_edge(clk);
reset <= '1';
wait until rising_edge(clk);
reset <= '0';
wait for 3 * CLK_PERIOD;
PS_to_PL_buffer(7 downto 4) <= x"8";
wait for 60 * CLK_PERIOD;


--sync 
Generate_Input (x"B9A0",i_data_input,i_generate_response);
Generate_Input (x"8FFF",i_data_input,i_generate_response);

--first frame Data request
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"CCCC",i_data_input,i_generate_response);
Generate_Input (x"0001",i_data_input,i_generate_response);
for ii in 0 to 5 loop
Generate_Input (x"0000",i_data_input,i_generate_response);
end loop;

--second frame data Init
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"BBBB",i_data_input,i_generate_response);
Generate_Input (x"0064",i_data_input,i_generate_response);
Generate_Input (x"0080",i_data_input,i_generate_response);
for ii in 0 to 4 loop
Generate_Input (x"0F00",i_data_input,i_generate_response);
end loop;

--3 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--4 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;

--5 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--6 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--7 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--8 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--9 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
--10 frame
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;

--send trainer data
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"CCCC",i_data_input,i_generate_response);
Generate_Input (x"0002",i_data_input,i_generate_response);
for ii in 0 to 5 loop
Generate_Input (x"0000",i_data_input,i_generate_response);
end loop;

Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"BBBB",i_data_input,i_generate_response);
Generate_Input (x"0032",i_data_input,i_generate_response);
Generate_Input (x"0080",i_data_input,i_generate_response);
for ii in 0 to 4 loop
Generate_Input (x"0F00",i_data_input,i_generate_response);
end loop;

Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;
Generate_Input (x"2345",i_data_input,i_generate_response);
Generate_Input (x"8888",i_data_input,i_generate_response);
for ii in 0 to 6 loop
Generate_Input (x"0007",i_data_input,i_generate_response);
end loop;

wait for 2 us;
assert false report "Tests Complete" severity failure;

end process;

end Behavioral;
