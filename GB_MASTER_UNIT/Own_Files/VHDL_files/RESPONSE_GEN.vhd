library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library work;
use work.Constants_and_Declarations.all;
use IEEE.numeric_std.all;
use IEEE.std_logic_unsigned.all;


entity RESPONSE_GEN is
    Port ( sysclk : in STD_LOGIC;
           --data in/out signals
           o_data_output : out std_logic_vector(15 downto 0);
           i_data_input : in std_logic_vector(15 downto 0);
           --control signals
           o_response_valid : out std_logic;
           i_generate_response : in std_logic;
           o_debug : out std_logic;
           --switches        
           switch0 : in std_logic;
           switch1 : in std_logic;
           switch2 : in std_logic;
           switch3 : in std_logic;
           --buttons
           resetbutton : in std_logic;
           tradebutton : in std_logic;
           --Signals for trasnfering Pokemon data between top module and response gen
           Pokemon_buffer_1_new : in Pokemon_structure_buffer_type;
           Pokemon_buffer_1_valid : in std_logic
           
           );
end RESPONSE_GEN;

architecture Behavioral of RESPONSE_GEN is

    -- signals for Pokemon bufer which store the 50 16bit pokemon data structure                         
    signal Pokemon_buffer_1 : Pokemon_structure_buffer_type := Groudon_buffer;
    constant Pokemon_buffer_2 : Pokemon_structure_buffer_type := Breloom_buffer;
    constant Pokemon_buffer_3 : Pokemon_structure_buffer_type := Mawile_buffer;
    constant Pokemon_buffer_4 : Pokemon_structure_buffer_type := Suicune_buffer;
    constant Pokemon_buffer_5 : Pokemon_structure_buffer_type := Entei_buffer;
    constant Pokemon_buffer_6 : Pokemon_structure_buffer_type := Milotic_buffer;
    -- Team structure which includes 6 Pokemon structure buffers
    signal Team : Team_structure;
    --possible structure for Team which is received by real gameboy game
    signal Received_team : Team_structure := (others => No_pokemon_buffer);
    
    -- Index counter Team structure
    signal team_index : natural range 0 to TEAM_SIZE := 0;
    --buffer counter for indexing position of buffer
    signal buffer_counter : natural range 0 to 100 := 0;
    signal Trainer_buffer_counter : natural range 1 to 14 :=1;
    
    signal max_trainer_buf_count :natural := 14;
    --counter signals for received packages "B9A0" at the beginning of severeal sync phases
    signal counter_for_send_B9A0 : natural range 0 to 127 := 0;  
    
    signal frame_counter : natural range 0 to 8 := 0;
    
    signal w_data_output : std_logic_vector(15 downto 0) := x"FFF8";

    type   t_SM_Main is (s_Prepare,s_SOF,s_frameing,s_op_state_select,s_Validation,
                            s_Respond_sync1,s_Respond_sync2,s_Respond,s_Respond_move,s_Respond_Slave,s_Send_Pokemon_data,s_send_trainer_data);
    signal r_SM_Main : t_SM_Main := s_Prepare;
    
    signal Slave_choice : std_logic_vector(1 downto 0) := "00";
    
    type t_Operation_state is (op_sync_1,op_move,op_data_exchange_1,op_sync_2,op_data_exchange_2,op_wait_for_slave_reponse,op_send_pokemon_Data,op_trainer_data,op_trainer_data_short);
    signal Operation_state : t_Operation_state := op_sync_1;

    
begin



main : process(sysclk,switch0)
begin
if rising_edge(sysclk) then
        if resetbutton = '1' then
            --reset operations
            o_response_valid <= '0';
            w_data_output <= x"0000";
            counter_for_send_B9A0 <= 0;
            buffer_counter <= 0;
            frame_counter <= 0;
            r_SM_MAIN <= s_Prepare;
            Operation_State <= op_sync_1;
            Slave_choice <= "00";
            Trainer_buffer_counter <= 1;  
            max_trainer_buf_count <= 14;  
            Team_index <= 0; 
                   
        else    
        case r_SM_MAIN is
        
        when s_Prepare =>
            -- praparation phase of the Main FSM, waits for i_generate_response for starting to generate a respone for respective input
            if i_generate_response = '1' then
                r_SM_MAIN <= s_frameing;
            else
                r_SM_Main <= s_Prepare;
            end if;
            o_response_valid <= '0';
   
        when s_frameing =>
            --state for framing the incoming pakages. Gameboy sends data in form of 9 packages per "frame"
            if i_data_input = x"8FFF" then
                frame_counter <= FRAME_LENGTH;
                --sync of frame counter to start of 9er frame sending
                r_SM_MAIN <= s_op_state_select;
                
            else
                --differciate between start of 9er frame or other parts of the frame
                if frame_counter = 0 then
                    r_SM_MAIN <= s_SOF;
                else
                    r_SM_MAIN <= s_op_state_select;
                    
                end if;
                frame_counter <= frame_counter;
            end if;
            o_response_valid <= '0';

        when s_SOF =>  --start of frame
            w_data_output <= i_data_input;
            --just send the data which was received from the master 
            r_SM_Main <= s_Validation;
            
            
            
        when s_op_state_select =>
                case Operation_state is
                    when op_sync_1 =>
                        if i_data_input = x"8FFF" then
                            Operation_state <= op_data_exchange_1;
                            r_SM_Main <= s_Respond;
                            Trainer_buffer_counter <= 1;
                        else 
                            Operation_state <= op_sync_1;
                            r_SM_Main <= s_Respond_sync1;
                        end if; 
                    when op_data_exchange_1 =>
                        if i_data_input = x"CAFE" then
                            Operation_state <= op_move;
                            r_SM_Main <= s_Respond_move;
                        elsif i_data_input = x"B9A0" then
                            Operation_state <= op_sync_2;
                            r_SM_Main <= s_Respond_sync2;
                         
                        elsif i_data_input = x"8888" and frame_counter = 1 and Trainer_buffer_counter < max_trainer_buf_count then
                            Operation_state <= op_trainer_data;
                            o_debug <= '1';
                            r_SM_Main <= s_send_trainer_data;
                        else 
                            Operation_state <= op_data_exchange_1;
                            r_SM_Main <= s_Respond;
                            o_debug <= '0';
                        end if;
                    when op_trainer_data =>
                        if  Trainer_buffer_counter < 14  then
                            if frame_counter = 1 then
                                if i_data_input = x"8888" then
                                    Operation_state <= op_trainer_data;
                                    r_SM_Main <= s_send_trainer_data;
                                else
                                    Operation_state <= op_data_exchange_1;
                                    r_SM_Main <= s_Respond;
                                end if;
                            else
                               Operation_state <= op_trainer_data;
                               r_SM_Main <= s_send_trainer_data; 
                            end if;
                            
                        else
                            Operation_state <= op_data_exchange_1;
                            r_SM_Main <= s_RESPOND;
                        end if;   
                        
                    when op_move =>
                         if i_data_input = x"B9A0" then
                            Operation_state <= op_sync_2;
                            r_SM_Main <= s_Respond_sync2;
                        else 
                            Operation_state <= op_move;
                            r_SM_Main <= s_Respond_move;
                        end if;
                    when op_sync_2 =>
                        if i_data_input = x"8FFF" then
                            Operation_state <= op_data_exchange_2;
                            r_SM_Main <= s_Respond;
                            Trainer_buffer_counter <= 1;
                            Team_index <= 0;
                            
                        else 
                            Operation_state <= op_sync_2;
                            r_SM_Main <= s_Respond_sync2;
                            
                        end if; 
                     when op_data_exchange_2 =>
                        if tradebutton = '1' and frame_counter = 1 then
                            Operation_state <= op_wait_for_slave_reponse;
                            r_SM_Main <= s_Respond_Slave;
                        else
                            if i_data_input = x"B9A0" then
                                Operation_state <= op_sync_1;
                                r_SM_Main <= s_Respond_sync1;
                            elsif i_data_input = x"8888" and frame_counter = 1 then
                                case Trainer_buffer_counter is
                                    when 1 | 2 | 3 | 4 | 5  =>
                                        Operation_state <= op_trainer_data_short;
                                        r_SM_Main <= s_send_trainer_data;
                                    when 6 | 7 | 8 =>
                                        buffer_counter <= 0;
                                        Trainer_buffer_counter <=  Trainer_buffer_counter +1;
                                        Operation_state <= op_send_pokemon_data;
                                        r_SM_Main <= s_Send_Pokemon_Data; 
                                    when others =>
                                        Operation_state <= op_data_exchange_2; 
                                        r_SM_Main <= s_Respond;
                                        o_debug <= '0'; 
                                                                       
                                end case;
                            else
                                Operation_state <= op_data_exchange_2; 
                                r_SM_Main <= s_Respond;
                                o_debug <= '0';
                             end if;
                           ---add a small fraction of the trainer data pass here before the pokemon get send
                        end if;
                        
                    when op_trainer_data_short =>
                        if  Trainer_buffer_counter < 6  then
                            if frame_counter = 1 then
                                if i_data_input = x"8888" then
                                    Operation_state <= op_trainer_data_short;
                                    r_SM_Main <= s_send_trainer_data;
                                else
                                    Operation_state <= op_data_exchange_2;
                                    r_SM_Main <= s_Respond;
                                    o_debug <= '1';
                                end if;
                            else
                               Operation_state <= op_trainer_data_short;
                               r_SM_Main <= s_send_trainer_data; 
                            end if;
                            
                        else
                            Operation_state <= op_data_exchange_2;
                            r_SM_Main <= s_Respond;
                        end if;     
                        
                    when op_send_pokemon_data =>
                        if buffer_counter < 50 then
                            Operation_state <= op_send_pokemon_data;
                            r_SM_Main <= s_Send_Pokemon_Data;
                            o_debug <= '1';
                        elsif buffer_counter < 100 and buffer_counter >= 50 then
                            Operation_state <= op_send_pokemon_data;
                            r_SM_Main <= s_Send_Pokemon_Data;
                            o_debug <= '1';
                        else
                            o_debug <= '0';
                            Operation_state <= op_data_exchange_2; 
                            r_SM_Main <= s_Respond;
                        end if;
                         
                    when op_wait_for_slave_reponse =>
                        if tradebutton = '1'  then
                            Operation_state <= op_wait_for_slave_reponse;
                            r_SM_Main <= s_Respond_Slave;
                        else
                           Operation_state <= op_data_exchange_2;
                           Slave_choice <= "00"; 
                           r_SM_Main <= s_Respond;
                        end if;

                    when others =>
                        Operation_state <= op_sync_1;
                        r_SM_Main <= s_Respond_sync1;
                end case;
                o_response_valid <= '0';
        when s_Respond_sync1 =>
                       
                if counter_for_send_B9A0 < B9A0_sync_count then
                    w_data_output <= x"FFF8" ;
                    counter_for_send_B9A0 <= counter_for_send_B9A0 + 1;
                elsif counter_for_send_B9A0 = B9A0_sync_count then
                    w_data_output <= x"0000" ;
                    counter_for_send_B9A0 <= counter_for_send_B9A0 + 1;
                else
                    w_data_output <= i_data_input;
                end if;
            r_SM_Main <= s_Validation;
            o_response_valid <= '0';
            
        when s_Respond_sync2 => 
            w_data_output<= i_data_input;
            r_SM_Main <= s_Validation;
            o_response_valid <= '0';
            
        when s_Respond_move =>  
             w_data_output<= i_data_input;
             r_SM_Main <= s_Validation;
             o_response_valid <= '0';
             
        when s_send_Pokemon_data =>
            if frame_counter < 2 then
                w_data_output<= i_data_input;
            else
                if Team_index < TEAM_SIZE then
                   if buffer_counter <50 then               
                        w_data_output <= Team(Team_index)(buffer_counter);
                        Received_Team(Team_index)(buffer_counter) <= i_data_input;
                        if buffer_counter = 49 then
                                Team_index <= team_index +1;
                        end if;
                   elsif buffer_counter <100 and buffer_counter >=50 then
                        w_data_output <= Team(Team_index)(buffer_counter-50);
                        Received_Team(Team_index)(buffer_counter-50) <= i_data_input;
                        if buffer_counter = 99 then
                                Team_index <= team_index +1;
                        end if;
                   else
                        
                        w_data_output<= i_data_input; 
                   end if;
                   
                else
                   w_data_output<= x"0000";
                   --w_data_output<= i_data_input; 
                end if;
                buffer_counter <= buffer_counter +1;
            end if;
            r_SM_Main <= s_Validation;
            o_response_valid <= '0';    
        when s_send_trainer_data =>
             case Trainer_buffer_counter is
                when 1 => w_data_output<= i_data_input;
                when 2 => w_data_output<= trainer_pass_buffer2(frame_counter);
                when 3 => 
                if frame_counter = 4 or frame_counter = 6 then
                    w_data_output<= i_data_input;
                else
                    w_data_output <= trainer_pass_buffer3(frame_counter);
                end if;    
                when 4 => w_data_output<= i_data_input;
                when 5 => w_data_output<= i_data_input;
                when 6 => w_data_output<= trainer_pass_buffer6(frame_counter);
                when 7 => w_data_output<= trainer_pass_buffer7(frame_counter);
                when 8 => w_data_output<= trainer_pass_buffer8(frame_counter);
                when 9 => w_data_output<= trainer_pass_buffer9(frame_counter);
                when 10 => w_data_output<= trainer_pass_buffer10(frame_counter);
                when 11 => w_data_output<= trainer_pass_buffer11(frame_counter);
                when 12 => w_data_output<= i_data_input;
                when 13 => w_data_output<= i_data_input;
                if frame_counter = FRAME_LENGTH then
                    max_trainer_buf_count <= 6;
                end if; 
             when others => w_data_output<= i_data_input;
             end case; 
             if frame_counter = FRAME_LENGTH then
                Trainer_buffer_counter <= Trainer_buffer_counter +1;
             end if; 
             r_SM_Main <= s_Validation;
             o_response_valid <= '0';      
        when s_Respond =>
            case i_data_input is    
                    when x"8FFF" => w_data_output <= x"B9A0";            
                    when x"2222" |x"CCCC"  => w_data_output <= x"0000"; 
                    when x"1133" => w_data_output <= x"0000";
                    when x"1122" =>  w_data_output <= x"0000";
                    when x"0080" => w_data_output <= x"0081";                             
                 when others => 
                        w_data_output<= i_data_input;
            end case; 
            r_SM_Main <= s_Validation;
            o_response_valid <= '0';
        when s_Respond_Slave =>
            case Slave_choice is
            when "00" =>
                w_data_output <= frame_buffer_1(frame_counter);
                if frame_counter = 8 then
                    Slave_Choice <= "01";
                end if;
            when "01" =>
                if frame_counter = 3 then
                    if switch2 = '1' then
                        if switch1 = '0' then
                            w_data_output <= x"000" & '0' & switch2 & switch1 & switch0;   ---choose which pokemon to trade on bot side
                        else
                            w_data_output <= x"0000";
                        end if;
                    else
                        w_data_output <= x"000" & '0' & switch2 & switch1 & switch0;   ---choose which pokemon to trade on bot side
                    end if;
                    
                else
                w_data_output <= frame_buffer_2(frame_counter);
                end if;
                if frame_counter = 8 then
                    Slave_Choice <= "10";
                end if;
            when "10" =>
                w_data_output <= frame_buffer_3(frame_counter);
                if frame_counter = 8 then
                    Slave_Choice <= "11";
                end if;
            when others =>
                w_data_output <= i_data_input;
            end case;
            r_SM_Main <= s_Validation;
            o_response_valid <= '0';
        when s_Validation => 
            
            if frame_counter < FRAME_LENGTH then
                frame_counter <= frame_counter + 1;
            else
                frame_counter <= 0;
            end if;
            r_SM_Main <= s_Prepare;
            o_response_valid <= '1';            
        when others =>
               r_SM_Main <= s_Prepare;        
        end case;
       
        
        end if;
        
        
end if;
end process;

Pokemon_buffer_1 <= Pokemon_buffer_1_new when Pokemon_buffer_1_valid = '1' else Pokemon_buffer_1;
Team <= (Pokemon_buffer_1,Pokemon_buffer_2,Pokemon_buffer_3,Pokemon_buffer_4,Pokemon_buffer_5,Pokemon_buffer_6);
o_data_output <= w_data_output;

end Behavioral;
