----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11/24/2023 12:04:28 PM
-- Design Name: 
-- Module Name: TradeHandler - Behavioral
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
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;
use IEEE.std_logic_unsigned.all;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TradeHandler is
    Port ( clk : in STD_LOGIC;
           o_data_output : out std_logic_vector(15 downto 0);
           i_data_input : in std_logic_vector(15 downto 0);
           o_response_valid : out std_logic;
           i_generate_response : in std_logic;
           o_debug : out std_logic;        

           frameCounter : out integer;
           DataInputBuffer : in t_BufferSize110;
           BlockRequestActive : out std_logic;
           BlockInitActive : out std_logic;
           BlockDatalength : out integer;
           Blockrequesttype : out t_BlockRequestType;
           BlockRequestPokemonIndex : out natural range 0 to 2;
           
           TradeSpotNumber : in std_logic_vector(2 downto 0);
           reset : in std_logic;
           tradebutton : in std_logic
           --o_TX8_serial : out std_logic
        );
end TradeHandler;

architecture Behavioral of TradeHandler is




type t_SM_Main is (s_Idle,s_Framing,s_Active,s_InitTrade,s_Validation);

type t_SM_Sync is (s_Sync,s_handling);
signal SyncState : t_SM_Sync := s_Sync;

signal w_frameCounter : integer := 1;
constant FRAME_LENGTH : integer := 9;

signal w_debug : std_logic := '1';

--
signal w_data_output : std_logic_vector(15 downto 0) := x"FFF8";
signal s_SM_Main : t_SM_Main := s_Idle;

type LinkCommands is (LinkCMD_IDLE,LinkCMD_INIT_BLOCK,LinkCMD_REQUEST_BLOCK,LinkCMD_CONT_BLOCK,LinkCMD_SEND_LINK_TYPE);
signal LinkCMD : LinkCommands := LinkCMD_IDLE;
-----------------Own Blocks---------------------------------
signal InitTradecounter : natural := 0;


signal STartInitTrade : std_logic := '0';
signal EndInitTrade : std_logic := '0';

------------------LinkCMD CONT Block-------------------------

--buffers
signal Pokemonbuffer1 : Pokemon_structure_buffer_type := Zapdos_buffer;
signal Pokemonbuffer2 : Pokemon_structure_buffer_type := Milotic_Buffer;
signal Pokemonbuffer3 : Pokemon_structure_buffer_type := No_pokemon_buffer;
signal Pokemonbuffer4 : Pokemon_structure_buffer_type := No_pokemon_buffer;
signal Pokemonbuffer5 : Pokemon_structure_buffer_type := No_pokemon_buffer;
signal Pokemonbuffer6 : Pokemon_structure_buffer_type := No_pokemon_buffer;

signal Team : Team_structure := (Zapdos_buffer,Milotic_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer,No_Pokemon_buffer);

-- Index counter Team structure
signal team_index : natural range 0 to TEAM_SIZE := 0;
--buffer counter for indexing position of buffer
signal buffer_index : natural range 0 to 110 := 0;


signal BufferSize100 : t_BufferSize100 := (others => x"0000");
signal BufferSize50 : t_BufferSize50 := (others => x"0000");
signal BufferSize110 : t_BufferSize110 := (others => x"0000");

signal w_BlockContActive : std_logic := '0';


-------------------LinkCMD Init Block----------------------------
signal w_BlockDataLength : natural := 0;
signal w_BlockInitActive : std_logic := '0';


-------------------LinkCMD request Block-------------------------

signal w_BlockRequestType : t_BlockRequestType;
signal w_BlockrequestActive : std_logic := '0';
signal w_BlockRequestPokemonIndex : natural range 0 to 2 := 2;

begin

main : process(clk)
begin
if rising_edge(clk) then
    if reset = '1' then
        o_response_valid <= '0';
        w_data_output <= x"0000";
        s_SM_Main <= s_idle;
        team_index <= 0;
        buffer_index <= 0;
        w_BlockInitActive <= '0';
        w_BlockRequestActive <= '0';
        w_BlockRequestPokemonIndex <= 2;
        
    else
        
        case S_SM_Main is
        when s_idle =>
            if i_generate_Response = '1' then
                s_SM_Main <= s_framing;
            else
                s_SM_Main <= s_idle;
            end if;
            o_response_valid <= '0';
        when s_framing =>
            if SyncState = s_Sync then
                w_FrameCounter <= 2;
                w_BlockRequestActive <= '0';
                w_BlockInitActive <= '0';
                w_BlockRequestPokemonIndex <= 2;
            else
                if w_frameCounter = FRAME_LENGTH then
                    w_framecounter <= 1;
                else
                    w_framecounter <= w_framecounter +1; 
                end if;
            end if;
            if StartInitTrade = '0' then
                s_SM_Main <= s_Active;
            else
                s_SM_Main <= s_InitTrade;
            end if;
            o_response_valid <= '0';
        when s_Active =>
        
            case w_FrameCounter is
            when 1 =>
                w_data_output <= i_data_input;
            when 2 =>
                case i_data_input is
                when x"CCCC" => LinKCMD <= LinkCMD_REQUEST_BLOCK;
                    w_data_output <= x"0000";
                    w_BlockRequestActive <= '1';
                    w_BlockContActive <= '0';
                    if w_BlockRequestPokemonIndex = 2 then
                        w_BlockRequestPokemonIndex <= 0;
                    else
                        w_BlockRequestPokemonIndex <= w_BlockRequestPokemonIndex +1;
                    end if;
                when x"BBBB" => LinkCMD <= LinkCMD_INIT_BLOCK;
                    w_data_output <= i_data_input;
                    w_BlockInitActive <= '1';
                    w_BlockContActive <= '0';
                    buffer_index <= 0;
                when x"8888" => LinkCMD <= LinkCMD_CONT_BLOCK;
                    w_data_output <= i_data_input;
                    w_BlockContActive <= '1';
                when x"2222" => LinkCMD <= LinkCMD_SEND_LINK_TYPE;
                    w_data_output <= x"0000";
                when x"8FFF" => SyncState <= s_handling;
                    --w_debug <= not(w_debug);
                    w_framecounter <= 9;
                when x"5FFF" => SyncState <= s_Sync;
                    w_data_output <= i_data_input;
                when others => LinkCMD <= LinkCMD_IDLE;
                    if w_BlockContActive = '1' then
                        w_BlockRequestActive <= '0';
                        w_BlockInitActive <= '0';
                        w_BlockContActive <= '0';
                    end if;
                    w_data_output <= i_data_input;
                    buffer_index <= 0;
                end case;
                
            
            when 3 =>
                case LinkCMD is
                
                when LinkCMD_REQUEST_BLOCK =>
                    case i_data_input is
                    when x"0001" => --Pokemon Data
                        w_BlockrequestType <= BLOCK_REQ_SIZE_200;
                    when x"0002" => --Trainder Data
                        w_BlockrequestType <= BLOCK_REQ_SIZE_100;
                    when x"0003" => --Mail
                        w_BlockrequestType <= BLOCK_REQ_SIZE_220;
                    when x"0004" => --Gift ribbons
                        w_BlockrequestType <= BLOCK_REQ_SIZE_40;
                    when others =>
                        w_BlockrequestType <= BLOCK_REQ_SIZE_200;
                    end case;
                    w_data_output <= x"0000";
                    
                when LinkCMD_INIT_BLOCK =>
                    w_data_output <= i_data_input;
                    w_BlockDataLength <= to_integer(unsigned(i_data_input)) ;
                    
                when LinkCMD_CONT_BLOCK =>
                if (w_BlockRequestActive = '1' and w_BlockInitActive = '1' ) then
                        if buffer_index < w_BlockDataLength then
                            w_data_output <= DataInputBuffer(buffer_index);
                            buffer_index <= buffer_index +1;
                        else
                            w_data_output <= x"0000";
                            w_BlockRequestActive <= '0';
                            w_BlockInitActive <= '0';
                        end if;
                else
                    w_data_output <= i_data_input;
                    w_BlockRequestActive <= '0';
                    w_BlockInitActive <= '0';
                end if;               
                    
                when LinkCMD_SEND_LINK_TYPE =>
                    w_data_output <= x"0000";
                when others =>
                    w_data_output <= i_data_input;
                end case;
            when 4 =>
                case LinkCMD is
                
                when LINKCMD_INIT_BLOCK =>
                    w_data_output <= x"0081";
                    
                when LinkCMD_CONT_BLOCK =>
                if (w_BlockRequestActive = '1' and w_BlockInitActive = '1' ) then
                        if buffer_index < w_BlockDataLength then
                            w_data_output <= DataInputBuffer(buffer_index);
                            buffer_index <= buffer_index +1;
                        else
                            w_BlockRequestActive <= '0';
                            w_BlockInitActive <= '0';
                            w_data_output <= x"0000";
                            
                        end if;
                else
                    w_data_output <= i_data_input;
                    w_BlockRequestActive <= '0';
                    w_BlockInitActive <= '0';
                end if; 
                   
                when others =>
                    w_data_output <= i_data_input;
                end case;
                
            when others =>
            
                case LinkCMD is
                
                when LinkCMD_CONT_BLOCK =>
                if (w_BlockRequestActive = '1' and w_BlockInitActive = '1' ) then
                        if buffer_index < w_BlockDataLength then
                            w_data_output <= DataInputBuffer(buffer_index);
                            buffer_index <= buffer_index +1;
                        else
                            w_BlockRequestActive <= '0';
                            w_BlockInitActive <= '0';
                            w_data_output <= x"0000";
                        end if;
                else
                    w_data_output <= i_data_input;
                    w_BlockRequestActive <= '0';
                    w_BlockInitActive <= '0';
                end if;    
                when others =>
                    w_data_output <= i_data_input;
                end case;
                --w_data_output <= i_data_input;
            end case;
            o_response_valid <= '0';
            s_SM_Main <= s_validation;
            
            --w_data_output <= i_data_input;
            
        when s_initTrade =>
            case w_FrameCounter is 
            when 1 =>
                w_data_output <= i_data_input;
            when 4 =>
                if InitTradeCounter = 1 then
                    w_data_output <= x"000" & '0' & TradeSpotNumber;
                else
                    w_data_output <= ReadyToTradeBuffer(InitTradeCounter)(w_FrameCounter-1);
                end if;
            when 9 =>
                w_data_output <= ReadyToTradeBuffer(InitTradeCounter)(w_FrameCounter-1);
                if InitTradeCounter = 2 then
                    InitTradeCounter <= 0;
                    EndInitTrade <= '1';
                else
                    InitTradeCounter <= InitTradeCounter +1;
                    EndInitTrade <= '0';
                end if;
            when others =>
                w_data_output <= ReadyToTradeBuffer(InitTradeCounter)(w_FrameCounter-1);
                
            end case;   
            
            
            
             o_response_valid <= '0';
            s_SM_Main <= s_validation;
            
        when s_validation =>
            s_SM_Main <= s_Idle;
            o_response_valid <= '1';
            --w_debug <= not(w_debug);
        when others =>
            s_SM_Main <= s_Idle;
             w_data_output <= i_data_input;
             o_response_valid <= '0';
           
        end case;
    
    
    end if;
end if;
end process;


tradeing : process(clk)
begin
if rising_edge(clk) then
    if StartInitTrade = '0' then
        if Tradebutton = '1' then
            StartInitTrade <= '1' ;
        else
            StartInitTrade <= '0';
        end if;
    
    else
        if EndInitTrade = '0' then
            StartInitTrade <= '1';
        else
            StartInitTrade <= '0';
        end if;
    
    end if;
end if;
end process;


BlockRequesttype <= w_BlockRequesttype;
BlockRequestActive <= w_BlockRequestActive;
BlockDataLength <= w_BlockDataLength;
BlockInitActive <= w_BlockInitActive;
BlockRequestPokemonIndex <= w_BlockRequestPokemonIndex;

o_data_output <= w_data_output;
o_debug <= w_debug;
FrameCounter <= w_framecounter;
end Behavioral;
