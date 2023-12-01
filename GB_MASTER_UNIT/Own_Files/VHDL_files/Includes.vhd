

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


package Constants_and_Declarations is 

    constant FRAME_LENGTH : integer := 9;
    
    constant PKMN_BUFFER_DEPTH : natural := 50;  -- Pokemon structure has 100 bytes. Data is send in 16 bit chunks so Buffer_depth is 50
    
    constant MAX_TEAM_SIZE : natural := 6;
    constant TEAM_SIZE : natural := 6;
    type frame_buffer_type is array (0 to FRAME_LENGTH-1) of std_logic_vector(15 downto 0);
    subtype frame_buffer_index_type is natural range frame_buffer_type'range;
    
    type Pokemon_structure_buffer_type is array (0 to PKMN_BUFFER_DEPTH-1) of std_logic_vector(15 downto 0);
    subtype PKMN_structure_buffer_index_type is integer range Pokemon_structure_buffer_type'range;
    
    type Team_structure is array (0 to MAX_TEAM_SIZE-1) of Pokemon_structure_buffer_type; 
    
    type t_BufferSize100 is array (0 to 99) of std_logic_vector(15 downto 0);
    type t_BufferSize50 is array (0 to 49) of std_logic_vector(15 downto 0);
    type t_BufferSize110 is array (0 to 109) of std_logic_vector(15 downto 0);
    type t_BufferSize10 is array ( 0 to 9) of std_logic_vector(15 downto 0);
    
    type t_TwoPokemonBuffer is array ( 0 to 2) of t_bufferSize110;
    
    type t_BlockRequestType is (BLOCK_REQ_SIZE_200,BLOCK_REQ_SIZE_220,BLOCK_REQ_SIZE_100,BLOCK_REQ_SIZE_40);
    --constant Pokemon_to_trade : std_logic_vector(15 downto 0) := x"0001";
    
    constant ReadyToTradeBuffer : t_BufferSize10 := (x"AABB",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000");
    
    constant EmptyBuffer110 : t_BufferSize110 := (others => x"0000");
    
    constant No_pokemon_buffer : Pokemon_structure_buffer_type := (others => x"0000");
    
    constant Zapdos_buffer : Pokemon_structure_buffer_type := (
     x"0002", x"41f3", x"4753", x"06a6", x"d5d4", x"d8e4", x"e7e3", x"ffff",
     x"ffff", x"0205", x"d5c6", x"d6e1", x"ffe3", x"00ff", x"0e6b", x"0000",
     x"4706", x"4790", x"47bc", x"4700", x"4d5b", x"4d5f", x"b851", x"25d0",
     x"bf4f", x"7aba", x"8e75", x"c75a", x"47c0", x"479d", x"5581", x"4746",
     x"b8ae", x"4755", x"4735", x"47c9", x"47ad", x"b8aa", x"b8ae", x"47aa",
     x"0000", x"0000", x"ff64", x"0159", x"0159", x"00a6", x"00f4", x"00ec",
     x"017e", x"00d7");
    constant Milotic_buffer : Pokemon_structure_buffer_type := (
     x"0004", x"41f1", x"4753", x"06a6", x"ddc7", x"e3e0", x"dde8", x"ffd7",
     x"ffff", x"0205", x"d5c6", x"d6e1", x"ffe3", x"00ff", x"6bb8", x"0000",
     x"461e", x"479f", x"5587", x"4744", x"b8a8", x"4757", x"b857", x"25d2",
     x"bb48", x"78a8", x"8e73", x"c758", x"473e", x"476f", x"4708", x"476d",
     x"4d5d", x"4d5d", x"47ff", x"47ab", x"1f57", x"b8a8", x"b8a8", x"47a8",
     x"0000", x"0000", x"ff64", x"0175", x"0175", x"0070", x"0101", x"00c6",
     x"00ec", x"0152");
     
     constant Trainer_buffer : t_BufferSize50 :=(
     x"0100",x"0001",x"7200",x"0021",x"0020",x"0017",x"009D",
     x"C371",x"007B",x"0026",x"0001",x"0000",x"0000",x"0000",
     x"0000",x"0000",x"003F",x"0000",x"141C",x"0002",x"0A29",
     x"1413",x"1037",x"020E",x"D5BE",x"DDE2",x"E0D9",x"6EFF",
     x"2E05",x"0000",x"0000",x"0000",x"000A",x"0000",x"0000",
     x"0000",x"0000",x"0000",x"0001",x"6F00",x"0101",x"0000",
     x"0000",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000",
     x"0000");
         

end package Constants_and_Declarations;
