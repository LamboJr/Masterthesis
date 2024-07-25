#include "MainCHeader.h"

#include "PokemonHeader/Pokemonbuffer.h"

#include "GenerateDataStructure.h"


//Functions for Creating Buffers out of the text files at Initialisation

//Converts Pokemon Data from a textfile into a uint_16 [50] Buffer structure 
void convertStructureintoBuffer(uint16_t * TransmitBuffer,size_t Buffersize,char * filename){

    if (generatePokemonStructure(filename,TransmitBuffer) != 1)
    {
        printf("Success\n");
    }
    else{
        printf("No valid Data Buffer for the Pokemon structure generated\n");
        abort();
    }
    
};

int generatePokemonStructure(char *filename, uint16_t *structure_addr){
    assert(sizeof(struct PPBonus) == 1);
    assert(sizeof(struct Growth) == 12);
    assert(sizeof(struct Attacks) == 12);
    assert(sizeof(struct Condition) == 12);
    assert(sizeof(struct Pokerus) == 1);
    assert(sizeof(struct Origins) == 2);
    assert(sizeof(struct IVs) == 4);
    assert(sizeof(struct Ribbons) == 4);
    assert(sizeof(struct Misc) == 12);
    assert(sizeof(struct Status) == 4);
    assert(sizeof(struct Pokemon) == 100);

    char Pokemon_name[20];

    char nickname[NICKNAME_LENGTH] = NICKNAME_DEFAULT;
    char trainer_name[TRAINER_NAME_LENGTH] = TRAINER_NAME_DEFAULT;

    // Construct Structure
    struct Growth growth = {
        .species = SPECIES_DEFAULT,
        .held_item = ITEM_INDEX_DEFAULT,
        .experience = EXPERIENCE_DEFAULT,
        .pp_bonus = (struct PPBonus) {
        .move1 = PP_BONUS_MOVE1_DEFAULT,
        .move2 = PP_BONUS_MOVE2_DEFAULT,
        .move3 = PP_BONUS_MOVE3_DEFAULT,
        .move4 = PP_BONUS_MOVE4_DEFAULT
        },
        .friendship = FRIENDSHIP,
        .unknown = 0,
    };

    struct Attacks attacks = {
        .moves = {MOVE1_INDEX_DEFAULT, 
                  MOVE2_INDEX_DEFAULT, 
                  MOVE3_INDEX_DEFAULT, 
                  MOVE4_INDEX_DEFAULT},
        .pp = {PP_MOVE1_DEFAULT, PP_MOVE2_DEFAULT, PP_MOVE3_DEFAULT, PP_MOVE4_DEFAULT}
    };

    struct Condition condition = {
        .hp_ev = HP_EV_DEFAULT,
        .attack_ev = ATTACK_EV_DEFAULT,
        .defense_ev = DEFENSE_EV_DEFAULT,
        .speed_ev = SPEED_EV_DEFAULT,
        .special_attack_ev = SPEZ_ATTACK_EV_DEFAULT,
        .special_defense_ev = SPEZ_DEFENSE_EV_DEFAULT,
        .coolness = COOLNESS_DEFAULT,
        .beauty = BEAUTY_DEFAULT,
        .cuteness = CUTENESS_DEFAULT,
        .smartness = SMARTNESS_DEFAULT,
        .toughness = TOUGHNESS_DEFAULT,
        .feel = FEEL_DEFAULT,
    };

    struct Misc misc = {
        .pokerus = (struct Pokerus) {
        .days_remaining = 0x0,
        .strain = 0x0,
        },
        .met_location = MET_LOCATION_DEFAULT, // Fateful Encounter
        .origins = (struct Origins) {
        .level_met = MET_LEVEL_DEFAULT,
        .game_met = MET_GAME_DEFAULT,
        .pokeball_type = POKEBALL_CAUGHT_DEFAULT,
        .trainer_gender = TRAINER_MALE
        },
        .ivs = (struct IVs) {
        .hp = HP_IV_DEFAULT,
        .attack = ATTACK_IV_DEFAULT,
        .defense = DEFENSE_IV_DEFAULT,
        .speed = SPEED_IV_DEFAULT,
        .special_attack = SPEZ_ATTACK_IV_DEFAULT,
        .special_defense = SPEZ_DEFENSE_IV_DEFAULT,
        .egg = 0,
        .ability = ABILITY_PRIMARY
        },
        .ribbons = (struct Ribbons) {
        .cool = COOL_RIBBON_DEFAULT,
        .beauty = BEAUTY_RIBBON_DEFAULT,
        .cute = CUTE_RIBBON_DEFAULT,
        .smart = SMART_RIBBON_DEFAULT,
        .tough = TOUGH_RIBBON_DEFAULT,
        .champion = CHAMPION_RIBBON_DEFAULT,
        .winning = WINNING_RIBBON_DEFAULT,
        .victory = VICTORY_RIBBON_DEFAULT,
        .artist = ARTIST_RIBBON_DEFAULT,
        .effort = EFFORT_RIBBON_DEFAULT,
        .special1 = SPECIAL1_RIBBON_DEFAULT,
        .special2 = SPECIAL2_RIBBON_DEFAULT,
        .special3 = SPECIAL3_RIBBON_DEFAULT,
        .special4 = SPECIAL4_RIBBON_DEFAULT,
        .special5 = SPECIAL5_RIBBON_DEFAULT,
        .special6 = SPECIAL6_RIBBON_DEFAULT,
        .padding = PADDING_RIBBON_DEFAULT,
        .obedience = OBEDIENCE_RIBBON_DEFAULT
        }
    };

    struct Pokemon pkmn = {
        .personality = 0,
        .trainer_id = TRAINER_ID,
        .language = MET_LANGUAGE_DEFAULT,
        .markings = MARKING_BULLET,
        .unknown = 0,
        .status = (struct Status) {
        .sleep = 0x0,
        .poisoned = 0x0,
        .burnt = 0x0,
        .frozen = 0x0,
        .paralyzed = 0x0,
        .bad_poisoned = 0x0,
        .zero_filler = 0x0 //keep always zero
        },
        .level = LEVEL_DEFAULT,
        .pokerus = 0xFF,
        .current_health = 100,
        .max_health = 100,
        .attack = 100,
        .defense = 100,
        .speed = 100,
        .special_attack = 100,
        .special_defense = 100
    }; 


    uint8_t nature = NATURE_DEFAULT;
    int c;
    int pkmn_valid = 0;
    int item_valid = 0;
    int shiny = 0;
    int move_valid = 0;
    bool dump = true;

    
    
    


    //strncpy(nickname, argv[optind], NICKNAME_LENGTH);
    //strncpy(trainer_name, argv[optind + 1], TRAINER_NAME_LENGTH);
    
    // Finalize structure
    assert(pcsconv(nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);
    memcpy(pkmn.nickname, nickname, NICKNAME_LENGTH);

    assert(pcsconv(trainer_name, TRAINER_NAME_LENGTH, LANGUAGE_ENGLISH) == true);
    memcpy(pkmn.trainer_name, trainer_name, TRAINER_NAME_LENGTH);

    struct Pokemon_Info Info = parsingTextFile(filename);
    pkmn.level = Info.Level;

        //printf("Nature : %s\n",Info.nature);
        if(!strcmp(Info.nature,"Hardy")){nature =NATURE_HARDY;}
        else if(!strcmp(Info.nature,"Lonely")){nature =NATURE_LONELY;}
        else if(!strcmp(Info.nature,"Brave")){nature =NATURE_BRAVE;}
        else if(!strcmp(Info.nature,"Adamant")){nature =NATURE_ADAMANT;}
        else if(!strcmp(Info.nature,"Naughty")){nature =NATURE_NAUGHTY;}
        else if(!strcmp(Info.nature,"Bold")){nature =NATURE_BOLD;}
        else if(!strcmp(Info.nature,"Docile")){nature =NATURE_DOCILE;}
        else if(!strcmp(Info.nature,"Relaxed")){nature =NATURE_RELAXED;}
        else if(!strcmp(Info.nature,"Impish")){nature =NATURE_IMPISH;}
        else if(!strcmp(Info.nature,"Lax")){nature =NATURE_LAX;}
        else if(!strcmp(Info.nature,"Timid")){nature =NATURE_TIMID;}
        else if(!strcmp(Info.nature,"Hasty")){nature =NATURE_HASTY;}
        else if(!strcmp(Info.nature,"Serious")){nature =NATURE_SERIOUS;}
        else if(!strcmp(Info.nature,"Jolly")){nature =NATURE_JOLLY;}
        else if(!strcmp(Info.nature,"Naive")){nature =NATURE_NAIVE;}
        else if(!strcmp(Info.nature,"Modest")){nature =NATURE_MODEST;}
        else if(!strcmp(Info.nature,"Mild")){nature =NATURE_MILD;}
        else if(!strcmp(Info.nature,"Quiet")){nature =NATURE_QUIET;}
        else if(!strcmp(Info.nature,"Bashful")){nature =NATURE_BASHFUL;}
        else if(!strcmp(Info.nature,"Rash")){nature =NATURE_RASH;}
        else if(!strcmp(Info.nature,"Calm")){nature =NATURE_CALM;}
        else if(!strcmp(Info.nature,"Gentle")){nature =NATURE_GENTLE;}
        else if(!strcmp(Info.nature,"Sassy")){nature =NATURE_SASSY;}
        else if(!strcmp(Info.nature,"Careful")){nature =NATURE_CAREFUL;}
        else if(!strcmp(Info.nature,"Quirky")){nature =NATURE_QUIRKY;}
        else {fprintf(stderr, "No valid Nature see --help\n");return 1;}

    
    for (int count = 0; count <= NUM_SPECIES; count++)
      {
        if(!strcmp(Info.Pokemon_name, species_string[count])){
          growth.species = (uint16_t) count;
          //printf("Species = %s\n",species_string[count]);
          pkmn_valid = 1;
        }
      }
      if(pkmn_valid == 0){
            fprintf(stderr, "No valid Pokemon name see --help\n");
            return 1;
      } 
    //printf("Item : %s\n",Info.item);
    for (int item_count = 0; item_count <= ITEMS_COUNT; item_count++)
      {
        if(!strcmp(Info.item,item_string_file[item_count])){
          growth.held_item  = (uint16_t) item_count;
          item_valid = 1;
         printf("ITEM = %s\n",item_string[item_count]);
        }
      }
      if (item_valid == 0) {
            fprintf(stderr, "No valid Item name see --help\n");
            return 1;
    }

    growth.friendship = Info.happiness;

    //printf("Move1 : %s|\n",Info.Moves[0]);
    for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info.Moves[0],move_string_file[move_count])){
          attacks.moves[0] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move1 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info.Moves[1],move_string_file[move_count])){
          attacks.moves[1] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move2 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info.Moves[2],move_string_file[move_count])){
          attacks.moves[2] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move3 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info.Moves[3],move_string_file[move_count])){
          attacks.moves[3] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move4 name see --help\n");
            return 1;
        }    
        condition.hp_ev = Info.ev_HP;
        condition.attack_ev = Info.ev_Attack;
        condition.defense_ev = Info.ev_Defense;
        condition.speed_ev = Info.ev_Speed;
        condition.special_attack_ev = Info.ev_SpAttack;
        condition.special_defense_ev = Info.ev_SpDefense;

        misc.ivs.hp = Info.iv_HP;
        misc.ivs.attack = Info.iv_Attack;
        misc.ivs.defense = Info.iv_Defense;
        misc.ivs.speed = Info.iv_Speed;
        misc.ivs.special_attack = Info.iv_SpAttack;
        misc.ivs.special_defense = Info.iv_SpDefense;

        misc.origins.game_met = GAME_LEAFGREEN;
        misc.origins.level_met = 0;
        misc.met_location = 88;

        //misc.ivs.ability = ABILITY_SECONDARY;

        printf("NicknameInfo : %s\n",Info.nickname);
        strncpy(pkmn.nickname,Info.nickname,NICKNAME_LENGTH);
        assert(pcsconv(pkmn.nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);

        //memcpy(pkmn.nickname, Info.nickname, NICKNAME_LENGTH);
        //printf("Nickname : %s\n",pkmn.nickname);
        //assert(pcsconv(pkmn.nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);

        shiny = Info.shiny;

        pkmn.personality = get_pers_value(TRAINER_ID,nature,shiny);

        if (pkmn.level == 100){
            growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level]);
        }else{
            growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level+1] -1);
        }

        pkmn.current_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
        pkmn.max_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
        pkmn.attack = calc_Stat_Value(condition.attack_ev,misc.ivs.attack,Species_Infos[growth.species].baseAttack,pkmn.level,NatureModifer_Attack(nature));
        pkmn.defense = calc_Stat_Value(condition.defense_ev,misc.ivs.defense,Species_Infos[growth.species].baseDefense,pkmn.level,NatureModifer_Defense(nature));
        pkmn.speed = calc_Stat_Value(condition.speed_ev,misc.ivs.speed,Species_Infos[growth.species].baseSpeed,pkmn.level,NatureModifer_Speed(nature));
        pkmn.special_attack = calc_Stat_Value(condition.special_attack_ev,misc.ivs.special_attack,Species_Infos[growth.species].baseSpAttack,pkmn.level,NatureModifer_SpezAttack(nature));
        pkmn.special_defense = calc_Stat_Value(condition.special_defense_ev,misc.ivs.special_defense,Species_Infos[growth.species].baseSpDefense,pkmn.level,NatureModifer_SpezDefense(nature));


    pkmn.checksum = data_encrypt_to(
        pkmn.data,
        pkmn.personality,
        pkmn.trainer_id,
        growth,
        attacks,
        condition,
        misc
    );
   /* uint8_t Testname[10]={0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2};

    for (int i = 0; i< 10;i++){
        printf("%c ",char_conv(Testname[i],LANGUAGE_GERMAN));
    }
   */

    //for (int i = 0; i < NUM_SPECIES; i++){printf("Index %d = %s \n",i,species_string[i]);  }
    printf("Species = %s\n",species_string[growth.species]);
    printf("Nature = %s\n",nature_string[nature]);
    printf("Experience = %d\n",growth.experience);
    printf("Met Location = %d level met = %d\n",misc.met_location,misc.origins.level_met);
    //printf("Stat : %d\n",pkmn.special_attack);
    for (int i = 0; i < 4; i++)
    {
        //printf("MOVE%d = %s index = %d\n",i,move_string[attacks.moves[i]],attacks.moves[i]);
    }

    hexdump_C(&pkmn, sizeof(struct Pokemon),species_string[growth.species]);
    

    uint16_t *pkmn_ptr = (uint16_t *) &pkmn;

    for (size_t i = 0; i < 50; i++)
    {
        *(structure_addr+i) = *(pkmn_ptr+i);
    }
    
    return 0;
}

//Functions for generation Buffers out of the Specs. from the GUI Process
int generatePokemonStructurefromGUI(uint16_t *structure_addr,struct PokemonStructureGUI *Info
){
    assert(sizeof(struct PPBonus) == 1);
    assert(sizeof(struct Growth) == 12);
    assert(sizeof(struct Attacks) == 12);
    assert(sizeof(struct Condition) == 12);
    assert(sizeof(struct Pokerus) == 1);
    assert(sizeof(struct Origins) == 2);
    assert(sizeof(struct IVs) == 4);
    assert(sizeof(struct Ribbons) == 4);
    assert(sizeof(struct Misc) == 12);
    assert(sizeof(struct Status) == 4);
    assert(sizeof(struct Pokemon) == 100);

    char Pokemon_name[20];

    char nickname[NICKNAME_LENGTH] = NICKNAME_DEFAULT;
    char trainer_name[TRAINER_NAME_LENGTH] = TRAINER_NAME_DEFAULT;

    // Construct Structure
    struct Growth growth = {
        .species = SPECIES_DEFAULT,
        .held_item = ITEM_INDEX_DEFAULT,
        .experience = EXPERIENCE_DEFAULT,
        .pp_bonus = (struct PPBonus) {
        .move1 = PP_BONUS_MOVE1_DEFAULT,
        .move2 = PP_BONUS_MOVE2_DEFAULT,
        .move3 = PP_BONUS_MOVE3_DEFAULT,
        .move4 = PP_BONUS_MOVE4_DEFAULT
        },
        .friendship = FRIENDSHIP,
        .unknown = 0,
    };

    struct Attacks attacks = {
        .moves = {MOVE1_INDEX_DEFAULT, 
                  MOVE2_INDEX_DEFAULT, 
                  MOVE3_INDEX_DEFAULT, 
                  MOVE4_INDEX_DEFAULT},
        .pp = {PP_MOVE1_DEFAULT, PP_MOVE2_DEFAULT, PP_MOVE3_DEFAULT, PP_MOVE4_DEFAULT}
    };

    struct Condition condition = {
        .hp_ev = HP_EV_DEFAULT,
        .attack_ev = ATTACK_EV_DEFAULT,
        .defense_ev = DEFENSE_EV_DEFAULT,
        .speed_ev = SPEED_EV_DEFAULT,
        .special_attack_ev = SPEZ_ATTACK_EV_DEFAULT,
        .special_defense_ev = SPEZ_DEFENSE_EV_DEFAULT,
        .coolness = COOLNESS_DEFAULT,
        .beauty = BEAUTY_DEFAULT,
        .cuteness = CUTENESS_DEFAULT,
        .smartness = SMARTNESS_DEFAULT,
        .toughness = TOUGHNESS_DEFAULT,
        .feel = FEEL_DEFAULT,
    };

    struct Misc misc = {
        .pokerus = (struct Pokerus) {
        .days_remaining = 0x0,
        .strain = 0x0,
        },
        .met_location = MET_LOCATION_DEFAULT, // Fateful Encounter
        .origins = (struct Origins) {
        .level_met = MET_LEVEL_DEFAULT,
        .game_met = MET_GAME_DEFAULT,
        .pokeball_type = POKEBALL_CAUGHT_DEFAULT,
        .trainer_gender = TRAINER_MALE
        },
        .ivs = (struct IVs) {
        .hp = HP_IV_DEFAULT,
        .attack = ATTACK_IV_DEFAULT,
        .defense = DEFENSE_IV_DEFAULT,
        .speed = SPEED_IV_DEFAULT,
        .special_attack = SPEZ_ATTACK_IV_DEFAULT,
        .special_defense = SPEZ_DEFENSE_IV_DEFAULT,
        .egg = 0,
        .ability = ABILITY_PRIMARY
        },
        .ribbons = (struct Ribbons) {
        .cool = COOL_RIBBON_DEFAULT,
        .beauty = BEAUTY_RIBBON_DEFAULT,
        .cute = CUTE_RIBBON_DEFAULT,
        .smart = SMART_RIBBON_DEFAULT,
        .tough = TOUGH_RIBBON_DEFAULT,
        .champion = CHAMPION_RIBBON_DEFAULT,
        .winning = WINNING_RIBBON_DEFAULT,
        .victory = VICTORY_RIBBON_DEFAULT,
        .artist = ARTIST_RIBBON_DEFAULT,
        .effort = EFFORT_RIBBON_DEFAULT,
        .special1 = SPECIAL1_RIBBON_DEFAULT,
        .special2 = SPECIAL2_RIBBON_DEFAULT,
        .special3 = SPECIAL3_RIBBON_DEFAULT,
        .special4 = SPECIAL4_RIBBON_DEFAULT,
        .special5 = SPECIAL5_RIBBON_DEFAULT,
        .special6 = SPECIAL6_RIBBON_DEFAULT,
        .padding = PADDING_RIBBON_DEFAULT,
        .obedience = OBEDIENCE_RIBBON_DEFAULT
        }
    };

    struct Pokemon pkmn = {
        .personality = 0,
        .trainer_id = TRAINER_ID,
        .language = MET_LANGUAGE_DEFAULT,
        .markings = MARKING_BULLET,
        .unknown = 0,
        .status = (struct Status) {
        .sleep = 0x0,
        .poisoned = 0x0,
        .burnt = 0x0,
        .frozen = 0x0,
        .paralyzed = 0x0,
        .bad_poisoned = 0x0,
        .zero_filler = 0x0 //keep always zero
        },
        .level = LEVEL_DEFAULT,
        .pokerus = 0xFF,
        .current_health = 100,
        .max_health = 100,
        .attack = 100,
        .defense = 100,
        .speed = 100,
        .special_attack = 100,
        .special_defense = 100
    }; 


    uint8_t nature = NATURE_DEFAULT;
    int c;
    int pkmn_valid = 0;
    int item_valid = 0;
    int shiny = 0;
    int move_valid = 0;
    bool dump = true;

    //strncpy(nickname, argv[optind], NICKNAME_LENGTH);
    //strncpy(trainer_name, argv[optind + 1], TRAINER_NAME_LENGTH);
    
    // Finalize structure
    assert(pcsconv(nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);
    memcpy(pkmn.nickname, nickname, NICKNAME_LENGTH);

    assert(pcsconv(trainer_name, TRAINER_NAME_LENGTH, LANGUAGE_ENGLISH) == true);
    memcpy(pkmn.trainer_name, trainer_name, TRAINER_NAME_LENGTH);

    //struct Pokemon_Info Info = parsingTextFile(filename);
    pkmn.level = Info->level;

        //printf("Nature : %s\n",Info->nature);
        if(!strcmp(Info->nature,"Hardy")){nature =NATURE_HARDY;}
        else if(!strcmp(Info->nature,"Lonely")){nature =NATURE_LONELY;}
        else if(!strcmp(Info->nature,"Brave")){nature =NATURE_BRAVE;}
        else if(!strcmp(Info->nature,"Adamant")){nature =NATURE_ADAMANT;}
        else if(!strcmp(Info->nature,"Naughty")){nature =NATURE_NAUGHTY;}
        else if(!strcmp(Info->nature,"Bold")){nature =NATURE_BOLD;}
        else if(!strcmp(Info->nature,"Docile")){nature =NATURE_DOCILE;}
        else if(!strcmp(Info->nature,"Relaxed")){nature =NATURE_RELAXED;}
        else if(!strcmp(Info->nature,"Impish")){nature =NATURE_IMPISH;}
        else if(!strcmp(Info->nature,"Lax")){nature =NATURE_LAX;}
        else if(!strcmp(Info->nature,"Timid")){nature =NATURE_TIMID;}
        else if(!strcmp(Info->nature,"Hasty")){nature =NATURE_HASTY;}
        else if(!strcmp(Info->nature,"Serious")){nature =NATURE_SERIOUS;}
        else if(!strcmp(Info->nature,"Jolly")){nature =NATURE_JOLLY;}
        else if(!strcmp(Info->nature,"Naive")){nature =NATURE_NAIVE;}
        else if(!strcmp(Info->nature,"Modest")){nature =NATURE_MODEST;}
        else if(!strcmp(Info->nature,"Mild")){nature =NATURE_MILD;}
        else if(!strcmp(Info->nature,"Quiet")){nature =NATURE_QUIET;}
        else if(!strcmp(Info->nature,"Bashful")){nature =NATURE_BASHFUL;}
        else if(!strcmp(Info->nature,"Rash")){nature =NATURE_RASH;}
        else if(!strcmp(Info->nature,"Calm")){nature =NATURE_CALM;}
        else if(!strcmp(Info->nature,"Gentle")){nature =NATURE_GENTLE;}
        else if(!strcmp(Info->nature,"Sassy")){nature =NATURE_SASSY;}
        else if(!strcmp(Info->nature,"Careful")){nature =NATURE_CAREFUL;}
        else if(!strcmp(Info->nature,"Quirky")){nature =NATURE_QUIRKY;}
        else {fprintf(stderr, "No valid Nature see --help\n");return 1;}

    for (int count = 0; count <= NUM_SPECIES; count++)
      {
        if(!strcmp(Info->species,species_string[count])){
          growth.species = (uint16_t) count;
          //printf("Species = %s\n",species_string[count]);
          pkmn_valid = 1;
        }
      }
      if(pkmn_valid == 0){
            fprintf(stderr, "No valid Pokemon name see --help\n");
            return 1;
      } 
    //printf("Item : %s\n",Info->item);
    for (int item_count = 0; item_count <= ITEMS_COUNT; item_count++)
      {
        if(!strcmp(Info->item,item_string_file[item_count])){
          growth.held_item  = (uint16_t) item_count;
          item_valid = 1;
         printf("ITEM = %s\n",item_string[item_count]);
        }
      }
      if (item_valid == 0) {
            fprintf(stderr, "No valid Item name see --help\n");
            return 1;
    }

    //growth.friendship = Info->happiness;

    //printf("Move1 : %s|\n",Info->Moves[0]);
    for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info->Move1,move_string_file[move_count])){
          attacks.moves[0] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move1 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info->Move2,move_string_file[move_count])){
          attacks.moves[1] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move2 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info->Move3,move_string_file[move_count])){
          attacks.moves[2] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move3 name see --help\n");
            return 1;
        }    
        move_valid = 0;
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(Info->Move4,move_string_file[move_count])){
          attacks.moves[3] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move4 name see --help\n");
            return 1;
        }    
        condition.hp_ev = Info->ev_HP;
        condition.attack_ev = Info->ev_Attack;
        condition.defense_ev = Info->ev_Defense;
        condition.speed_ev = Info->ev_Speed;
        condition.special_attack_ev = Info->ev_SpAttack;
        condition.special_defense_ev = Info->ev_SpDefense;

        misc.ivs.hp = Info->iv_HP;
        misc.ivs.attack = Info->iv_Attack;
        misc.ivs.defense = Info->iv_Defense;
        misc.ivs.speed = Info->iv_Speed;
        misc.ivs.special_attack = Info->iv_SpAttack;
        misc.ivs.special_defense = Info->iv_SpDefense;

        misc.origins.game_met = GAME_LEAFGREEN;
        misc.origins.level_met = 0;
        misc.met_location = 88;

        if (Species_Infos[growth.species].abilities[1] != ABILITY_NONE){
            misc.ivs.ability = Info->ability;
        }

        printf("NicknameInfo : %s\n",Info->nickname);
        strncpy(pkmn.nickname,Info->nickname,NICKNAME_LENGTH);
        assert(pcsconv(pkmn.nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);

        //memcpy(pkmn.nickname, Info->nickname, NICKNAME_LENGTH);
        //printf("Nickname : %s\n",pkmn.nickname);
        //assert(pcsconv(pkmn.nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);

        shiny = Info->shiny;

        //pkmn.personality = get_pers_value(TRAINER_ID,nature,shiny);

        pkmn.personality = create_pers_value(TRAINER_ID,nature,Info->shiny,Info->ability,Info->genderValue);


        if (pkmn.level == 100){
            growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level]);
        }else{
            growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level+1] -1);
        }

        pkmn.current_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
        pkmn.max_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
        pkmn.attack = calc_Stat_Value(condition.attack_ev,misc.ivs.attack,Species_Infos[growth.species].baseAttack,pkmn.level,NatureModifer_Attack(nature));
        pkmn.defense = calc_Stat_Value(condition.defense_ev,misc.ivs.defense,Species_Infos[growth.species].baseDefense,pkmn.level,NatureModifer_Defense(nature));
        pkmn.speed = calc_Stat_Value(condition.speed_ev,misc.ivs.speed,Species_Infos[growth.species].baseSpeed,pkmn.level,NatureModifer_Speed(nature));
        pkmn.special_attack = calc_Stat_Value(condition.special_attack_ev,misc.ivs.special_attack,Species_Infos[growth.species].baseSpAttack,pkmn.level,NatureModifer_SpezAttack(nature));
        pkmn.special_defense = calc_Stat_Value(condition.special_defense_ev,misc.ivs.special_defense,Species_Infos[growth.species].baseSpDefense,pkmn.level,NatureModifer_SpezDefense(nature));


    pkmn.checksum = data_encrypt_to(
        pkmn.data,
        pkmn.personality,
        pkmn.trainer_id,
        growth,
        attacks,
        condition,
        misc
    );
   /* uint8_t Testname[10]={0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2};

    for (int i = 0; i< 10;i++){
        printf("%c ",char_conv(Testname[i],LANGUAGE_GERMAN));
    }
   */

    //for (int i = 0; i < NUM_SPECIES; i++){printf("Index %d = %s \n",i,species_string[i]);  }
    printf("Species = %s\n",species_string[growth.species]);
    printf("Nature = %s\n",nature_string[nature]);
    printf("Experience = %d\n",growth.experience);
    printf("Ability = %d \n",misc.ivs.ability);
    printf("Met Location = %d level met = %d\n",misc.met_location,misc.origins.level_met);
    //printf("Stat : %d\n",pkmn.special_attack);
    for (int i = 0; i < 4; i++)
    {
        //printf("MOVE%d = %s index = %d\n",i,move_string[attacks.moves[i]],attacks.moves[i]);
    }

    hexdump_C(&pkmn, sizeof(struct Pokemon),species_string[growth.species]);
    

    uint16_t *pkmn_ptr = (uint16_t *) &pkmn;

    for (size_t i = 0; i < 50; i++)
    {
        *(structure_addr+i) = *(pkmn_ptr+i);
    }
    
    return 0;
}




