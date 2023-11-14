

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


package Constants_and_Declarations is

    
    constant B9A0_sync_count : natural := 100;  ---Value for Counter in SYNC 1. 
    constant FRAME_LENGTH : natural := 8; --Frame length = length of frame_buffer. Frame buffer = 9 single 16 bit packets before longer Pause and checksum
    constant PKMN_BUFFER_DEPTH : natural := 50;  -- Pokemon structure has 100 bytes. Data is send in 16 bit chunks so Buffer_depth is 50
    
    constant MAX_TEAM_SIZE : natural := 6;
    constant TEAM_SIZE : natural := 6;
    type frame_buffer_type is array (0 to FRAME_LENGTH) of std_logic_vector(15 downto 0);
    subtype frame_buffer_index_type is natural range frame_buffer_type'range;
    
    type Pokemon_structure_buffer_type is array (0 to PKMN_BUFFER_DEPTH-1) of std_logic_vector(15 downto 0);
    subtype PKMN_structure_buffer_index_type is integer range Pokemon_structure_buffer_type'range;
    
    type Team_structure is array (0 to MAX_TEAM_SIZE-1) of Pokemon_structure_buffer_type; 
    
    type Pers_Val_type is array (0 to 1) of std_logic_vector(15 downto 0);
    type TRAINERID_type is array (0 to 1) of std_logic_vector(15 downto 0);
    type PKMN_NICKNAME_type is array (0 to 4) of std_logic_vector(15 downto 0);
    type OT_NAME_MARKINGS_type is array (0 to 3) of std_logic_vector(15 downto 0);
    type DATA_type is array (0 to 23) of std_logic_vector(15 downto 0);
    type STATUS_COND_type is array (0 to 1) of std_logic_vector(15 downto 0);
    
    
    constant Trainer_Name : OT_NAME_MARKINGS_type := (x"D5C6",x"D6E1",x"FFE3",x"00FF"); -- Lambo
    constant Trainer_ID : TRAINERID_type := (x"4753",x"06A6");
    
    type OT_NAME_type is array (0 to 3) of std_logic_vector(15 downto 0);
    type Money_type is array (0 to 1) of std_logic_vector(15 downto 0);
    type BC_FP_type is array (0 to 1) of std_logic_vector(15 downto 0);
    type EASY_CHAT_type is array (0 to 3) of std_logic_vector(15 downto 0);
    type StickerArray_type is array (0 to 3) of std_logic_vector(7 downto 0);
    
    constant Bot_Name : OT_Name_type := (x"E3BC",x"FFE8",x"FFFF",x"0000");
    --constant Pokemon_to_trade : std_logic_vector(15 downto 0) := x"0001";
    --Sequence of 16bit data to respond if a button press from the slave is needed
    constant frame_buffer_1 : frame_buffer_type := (x"0000",x"BBBB",x"0014",x"0081",x"0000",x"0000",x"0000",x"0000",x"0000");
    constant frame_buffer_2 : frame_buffer_type := (x"0000",x"8888",x"AABB",x"0000",x"0000",x"0000",x"0000",x"0000",x"0000");
    constant frame_buffer_3 : frame_buffer_type := (x"0000",x"8888",x"0000",x"0000",x"0000",x"FFFF",x"FFFF",x"FFFF",x"0000");
    
    
    type Trainer_pass_structure is record
        Version : std_logic_vector( 7 downto 0);
        Trainer_ID : std_logic_vector(15 downto 0);
        Secret_ID : std_logic_vector(15 downto 0);
        Trainer_Name : OT_NAME_type;
        Character_shown : std_logic_vector(7 downto 0);
        Stars : std_logic_vector(7 downto 0);
        Gender : std_logic_vector(7 downto 0);
        Caughtallhoen : std_logic_vector(7 downto 0);
        got_pokedex : std_logic_vector(7 downto 0);
        HOF_Hours : std_logic_vector(15 downto 0);
        HOF_Minutes : std_logic_vector(15 downto 0);
        HOF_Seconds : std_logic_vector(15 downto 0);
        Caught_PKMN : std_logic_vector(15 downto 0);
        Hours_played : std_logic_vector(15 downto 0);
        Minutes_played : std_logic_vector(15 downto 0);
        LinkBattles_Won : std_logic_vector(15 downto 0);
        LinkBattles_Lose : std_logic_vector(15 downto 0);
        BattleTower_Wins : std_logic_vector(15 downto 0);
        BattleTower_StraightWins : std_logic_vector(15 downto 0);
        Contest_with_friends : std_logic_vector(15 downto 0);
        Pokeblocks : std_logic_vector(15 downto 0);
        Pokemon_Trades : std_logic_vector(15 downto 0);
        Money : Money_type;
        Easy_chat_Profile : EASY_CHAT_TYPE;
        Has_all_frontierSymbols : std_logic_vector(15 downto 0);
        BC_FP : BC_FP_type;
        UnionRoom_Num : BC_FP_type;
        Berries_picked : std_logic_vector(15 downto 0);
        JumpsInRow : BC_FP_type;
        DrawStickers: std_logic_vector(7 downto 0);
        HasAllMons : std_logic_vector(7 downto 0);
        MonIconTint :  std_logic_vector(7 downto 0);
        UnionRoomClass :  std_logic_vector(7 downto 0);
        StickerArray : StickerArray_type;
        
    end record Trainer_pass_structure;
    
    constant Trainer_card : Trainer_pass_structure := (
        Version => x"05",  ---03 is ermerlad --05 is leafgreen
        Trainer_ID => Trainer_ID(0),
        Secret_ID => Trainer_ID(1),
        Trainer_Name => Bot_Name,
        Character_shown => x"00", --Only gender byte not AF or similar unknown Byte  1 = female Male = 0
        Stars => x"04", --number of stars
        Gender => x"00", --gender 0 = male / 1 = female
        Caughtallhoen => x"00",
        got_pokedex => x"01",
        HOF_Hours => x"0002",
        HOF_Minutes => x"0006",
        HOF_Seconds => x"0025",
        Caught_PKMN => x"0070",
        Hours_played => x"00FF",
        Minutes_played => x"000F",
        LinkBattles_Won => x"0064",
        LinkBattles_Lose => x"0020",
        BattleTower_Wins => x"0001",
        BattleTower_StraightWins => x"0001",
        Contest_with_friends => x"0001",
        Pokeblocks => x"0001",
        Pokemon_Trades => x"0064",
        Money => (x"FFFF",x"0002"),
        Easy_chat_Profile => (x"0A29",x"1413",x"1037",x"020E"),
        Has_all_frontierSymbols => x"0001",
        BC_FP => (x"0065",x"0000"),
        UnionRoom_Num => (x"0030",x"0000"),
        Berries_picked => x"0030",
        JumpsInRow => (x"0020",x"0000"),
        DrawStickers => x"00",
        HasAllMons => x"01",
        MonIconTint => x"01",
        UnionRoomClass => x"00",
        StickerArray => (X"00",x"6F",x"01",x"01")
    );
    
    constant trainer_pass_buffer2 : frame_buffer_type := (x"0000",x"8888",x"0000",x"40" & Trainer_card.Version, x"8000",
                                                            Trainer_Card.Trainer_ID, Trainer_Card.Secret_ID ,Trainer_Card.Trainer_Name(0), Trainer_Card.Trainer_Name(1));
   
    constant trainer_pass_buffer3 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.Trainer_Name(2), Trainer_Card.Trainer_Name(3),x"0011",
                                                          Trainer_Card.character_shown & x"00",x"1133",x"0000",x"0000");
                                                          
    constant trainer_pass_buffer6 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.stars & Trainer_Card.gender,Trainer_Card.caughtallHoen & Trainer_Card.got_pokedex,
                                                           x"0000",Trainer_Card.HOF_Hours,Trainer_Card.HOF_Minutes,Trainer_Card.HOF_Seconds,Trainer_Card.Caught_PKMN);
                                                           
    constant trainer_pass_buffer7 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.Trainer_ID,Trainer_Card.Hours_played,Trainer_Card.Minutes_played,
                                                            Trainer_Card.LinkBattles_Won,Trainer_Card.LinkBattles_Lose,Trainer_Card.BattleTower_Wins,Trainer_Card.BattleTower_StraightWins);
                                                            
    constant trainer_pass_buffer8 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.Contest_with_friends,Trainer_Card.Pokeblocks,Trainer_Card.Pokemon_Trades,x"0000",
                                                            Trainer_Card.Money(0),Trainer_Card.Money(1),Trainer_Card.Easy_chat_profile(0));
    constant trainer_pass_buffer9 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.Easy_chat_profile(1),Trainer_Card.Easy_chat_profile(2),Trainer_Card.Easy_chat_profile(3),
                                                          Trainer_Card.Trainer_Name(0),Trainer_Card.Trainer_Name(1),Trainer_Card.Trainer_Name(2),Trainer_Card.Trainer_Name(3));
                                                          
    constant trainer_pass_buffer10 : frame_buffer_type := (x"0000",x"8888",x"00" & Trainer_Card.version,Trainer_Card.Has_all_frontierSymbols,Trainer_card.BC_FP(0),
                                                          Trainer_Card.BC_FP(1),Trainer_Card.UnionRoom_num(0),Trainer_Card.unionRoom_num(1),Trainer_Card.Berries_picked);
                                                                           --version 
    constant trainer_pass_buffer11 : frame_buffer_type := (x"0000",x"8888",Trainer_Card.JumpsinRow(0),Trainer_Card.JumpsInRow(1),
                                                           Trainer_Card.DrawStickers & Trainer_Card.HasAllMons,Trainer_Card.UnionRoomClass & Trainer_Card.MonIconTint,
                                                           Trainer_Card.StickerArray(0) & Trainer_Card.StickerArray(1),
                                                           Trainer_Card.StickerArray(2) & Trainer_Card.StickerArray(3),x"0000");
    
    
    type PKMN_structure is record
        Pers_VAl : Pers_Val_type;
        OT_ID : TRAINERID_type;
        Nickname : PKMN_NICKNAME_type;
        EGG_LANGUAGE : std_logic_vector(15 downto 0);
        OT_NAME_MARKINGS : OT_NAME_MARKINGS_type;
        Checksum : std_logic_vector(15 downto 0);
        ZERO_field : std_logic_vector(15 downto 0);
        Data_field : DATA_type;
        STATUS : STATUS_COND_type;
        Pokerus_Level : std_logic_vector(15 downto 0);
        Current_HP : std_logic_vector(15 downto 0);
        MAX_HP : std_logic_vector(15 downto 0);
        Attack : std_logic_vector(15 downto 0);
        Defense : std_logic_vector(15 downto 0);
        Speed : std_logic_vector(15 downto 0);
        SP_Attack : std_logic_vector(15 downto 0);
        SP_Defense : std_logic_vector(15 downto 0);
        
    end record PKMN_structure;
                                         
 
 constant No_pokemon_buffer : Pokemon_structure_buffer_type := (others => x"0000");

   
 
                                                           
    procedure incr_PKMN_buffer(signal index : inout PKMN_structure_buffer_index_type;
                   signal buffer_full : inout std_logic) ;    
                   
    
                   
    function Convert_to_buffer( PKMN_struc_to_covert : PKMN_structure) 
                               return Pokemon_structure_buffer_type;               

end package Constants_and_Declarations;

package body Constants_and_Declarations is

    function Convert_to_buffer( PKMN_struc_to_covert : PKMN_structure) 
                               return Pokemon_structure_buffer_type is 
    variable Converted_Structure : Pokemon_structure_buffer_type;
    begin
    Converted_Structure := (PKMN_struc_to_covert.Pers_Val(0),PKMN_struc_to_covert.Pers_Val(1),
                                PKMN_struc_to_covert.OT_ID(0),PKMN_struc_to_covert.OT_ID(1),
                                PKMN_struc_to_covert.Nickname(0),PKMN_struc_to_covert.Nickname(1),PKMN_struc_to_covert.Nickname(2),PKMN_struc_to_covert.Nickname(3),PKMN_struc_to_covert.Nickname(4),
                                PKMN_struc_to_covert.EGG_LANGUAGE,
                                PKMN_struc_to_covert.OT_NAME_MARKINGS(0),PKMN_struc_to_covert.OT_NAME_MARKINGS(1),PKMN_struc_to_covert.OT_NAME_MARKINGS(2),
                                PKMN_struc_to_covert.OT_NAME_MARKINGS(3),
                                PKMN_struc_to_covert.Checksum,PKMN_struc_to_covert.ZERO_field,
                                PKMN_struc_to_covert.Data_field(0),PKMN_struc_to_covert.Data_field(1),PKMN_struc_to_covert.Data_field(2),PKMN_struc_to_covert.Data_field(3),
                                PKMN_struc_to_covert.Data_field(4),PKMN_struc_to_covert.Data_field(5),PKMN_struc_to_covert.Data_field(6),PKMN_struc_to_covert.Data_field(7),
                                PKMN_struc_to_covert.Data_field(8),PKMN_struc_to_covert.Data_field(9),PKMN_struc_to_covert.Data_field(10),PKMN_struc_to_covert.Data_field(11),
                                PKMN_struc_to_covert.Data_field(12),PKMN_struc_to_covert.Data_field(13),PKMN_struc_to_covert.Data_field(14),PKMN_struc_to_covert.Data_field(15),
                                PKMN_struc_to_covert.Data_field(16),PKMN_struc_to_covert.Data_field(17),PKMN_struc_to_covert.Data_field(18),PKMN_struc_to_covert.Data_field(19),
                                PKMN_struc_to_covert.Data_field(20),PKMN_struc_to_covert.Data_field(21),PKMN_struc_to_covert.Data_field(22),PKMN_struc_to_covert.Data_field(23),
                                PKMN_struc_to_covert.Status(0),PKMN_struc_to_covert.Status(1),PKMN_struc_to_covert.Pokerus_level,PKMN_struc_to_covert.Current_HP,PKMN_struc_to_covert.MAX_HP,
                                PKMN_struc_to_covert.Attack,PKMN_struc_to_covert.Defense,PKMN_struc_to_covert.Speed,PKMN_struc_to_covert.SP_Attack,PKMN_struc_to_covert.Sp_Defense); 
    return Converted_Structure;
    end function;


    procedure incr_PKMN_buffer(signal index : inout PKMN_structure_buffer_index_type;
                   signal buffer_full : inout std_logic) is
    begin
    if index = PKMN_structure_buffer_index_type'high then
      index <= PKMN_structure_buffer_index_type'low;
      buffer_full <= '1';
    else
      index <= index + 1;
      buffer_full <= '0';
    end if;
    end procedure;

end Constants_and_Declarations;
