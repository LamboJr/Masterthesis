#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "species.h"
#include "items.h"
#include "constants.h"
#include "abilities.h"
#include "moves.h"
#include "functions.h"
#include "species_info.h"
//#include "natures.h"
#include "parsing.h"
#include "species_strings.h"
#include "item_strings.h"
#include "moves_strings.h"
#include "defaults.h"
#include "persVAl.h"
#include "experience_tables.h"


// Todo Experience for level calculate 
// Calculate personality value depending on random trainern ID
// Check if HP Values in total are legal
//automatically give the corret amount of PP for each move

/*
#define SECRET_ID 0x06A6
#define OT_TRAINER_ID 0x4753
#define TRAINER_ID 0x06A64753
*/




int main(int argc, char **argv) {
  // Ensure structs have correct sizes
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

  char nickname[NICKNAME_LENGTH] = NICKNAME;
  char trainer_name[TRAINER_NAME_LENGTH] = TRAINER_NAME;

  // Construct Structure
    struct Growth growth = {
        .species = SPECIES,
        .held_item = ITEM_INDEX,
        .experience = EXPERIENCE,
        .pp_bonus = (struct PPBonus) {
        .move1 = PP_BONUS_MOVE1,
        .move2 = PP_BONUS_MOVE2,
        .move3 = PP_BONUS_MOVE3,
        .move4 = PP_BONUS_MOVE4
        },
        .friendship = FRIENDSHIP,
        .unknown = 0,
    };

    struct Attacks attacks = {
        .moves = {MOVE1_INDEX, MOVE2_INDEX, MOVE3_INDEX, MOVE4_INDEX},
        .pp = {PP_MOVE1, PP_MOVE2, PP_MOVE3, PP_MOVE4}
    };

    struct Condition condition = {
        .hp_ev = HP_EV,
        .attack_ev = ATTACK_EV,
        .defense_ev = DEFENSE_EV,
        .speed_ev = SPEED_EV,
        .special_attack_ev = SPEZ_ATTACK_EV,
        .special_defense_ev = SPEZ_DEFENSE_EV,
        .coolness = COOLNESS,
        .beauty = BEAUTY,
        .cuteness = CUTENESS,
        .smartness = SMARTNESS,
        .toughness = TOUGHNESS,
        .feel = FEEL,
    };

    struct Misc misc = {
        .pokerus = (struct Pokerus) {
        .days_remaining = 0x0,
        .strain = 0x0,
        },
        .met_location = MET_LOCATION, // Fateful Encounter
        .origins = (struct Origins) {
        .level_met = MET_LEVEL,
        .game_met = MET_GAME,
        .pokeball_type = POKEBALL_CAUGHT,
        .trainer_gender = TRAINER_MALE
        },
        .ivs = (struct IVs) {
        .hp = HP_IV,
        .attack = ATTACK_IV,
        .defense = DEFENSE_IV,
        .speed = SPEED_IV,
        .special_attack = SPEZ_ATTACK_IV,
        .special_defense = SPEZ_DEFENSE_IV,
        .egg = 0,
        .ability = ABILITY_PRIMARY
        },
        .ribbons = (struct Ribbons) {
        .cool = COOL_RIBBON,
        .beauty = BEAUTY_RIBBON,
        .cute = CUTE_RIBBON,
        .smart = SMART_RIBBON,
        .tough = TOUGH_RIBBON,
        .champion = CHAMPION_RIBBON,
        .winning = WINNING_RIBBON,
        .victory = VICTORY_RIBBON,
        .artist = ARTIST_RIBBON,
        .effort = EFFORT_RIBBON,
        .special1 = SPECIAL1_RIBBON,
        .special2 = SPECIAL2_RIBBON,
        .special3 = SPECIAL3_RIBBON,
        .special4 = SPECIAL4_RIBBON,
        .special5 = SPECIAL5_RIBBON,
        .special6 = SPECIAL6_RIBBON,
        .padding = PADDING_RIBBON,
        .obedience = OBEDIENCE_RIBBON
        }
    };

  struct Pokemon pkmn = {
    .personality = 0,
    .trainer_id = TRAINER_ID,
    .language = LANGUAGE_GERMAN,
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
    .level = LEVEL,
    .pokerus = 0xFF,
    .current_health = 100,
    .max_health = 100,
    .attack = 100,
    .defense = 100,
    .speed = 100,
    .special_attack = 100,
    .special_defense = 100
  }; 


  uint8_t nature = NATURE;
  int c,pkmn_valid,item_valid = 0;
  int shiny = 0;
  int move_valid = 0;
  bool dump = false;

  while((c = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
    
    switch(c) {
    case 's': // species; see bulbapedia:List_of_Pokémon_by_index_number_(Generation_III)
      for (int count = 0; count <= NUM_SPECIES; count++)
      {
        if(!strcmp(optarg,species_string[count])){
          growth.species = (uint16_t) count;
          //printf("Species = %s\n",species_string[count]);
          pkmn_valid = 1;
        }
      }
      if(pkmn_valid == 0){
            fprintf(stderr, "No valid Pokemon name see --help\n");
            return 1;
      } 
      break;
    case 'x': // experience
      growth.experience = (uint32_t) atoi(optarg);
      break;
    case 'i': // held item; see bulbapedia:List_of_items_by_index_number_(Generation_III)
      for (int item_count = 0; item_count <= ITEMS_COUNT; item_count++)
      {
        if(!strcmp(optarg,item_string_file[item_count])){
          growth.held_item  = (uint16_t) item_count;
          item_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
      }
      if (item_valid == 0) {
            fprintf(stderr, "No valid Item name see --help\n");
            return 1;
        }    
      break;

    case 'B': // pp bonuses
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");

        growth.pp_bonus.move1 = atoi(one);
        growth.pp_bonus.move2 = atoi(two);
        growth.pp_bonus.move3 = atoi(three);
        growth.pp_bonus.move4 = atoi(four);
      }
      break;
    case 'f': // friendship
      growth.friendship = (uint8_t) atoi(optarg);
      break;
    case 'm': // moves
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");
        for (int move_count = 0; move_count <= MOVES_COUNT; move_count++)
        {
        if(!strcmp(one,move_string[move_count])){
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
        if(!strcmp(two,move_string[move_count])){
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
        if(!strcmp(three,move_string[move_count])){
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
        if(!strcmp(four,move_string[move_count])){
          attacks.moves[3] = (uint16_t) move_count;
          move_valid = 1;
         //printf("ITEM = %s\n",item_string[item_count]);
        }
        }
        if (move_valid == 0) {
            fprintf(stderr, "No valid Move4 name see --help\n");
            return 1;
        }    
        move_valid = 0;
      }
      break;
    case 'P': // pp for moves
      {
        char *one = strtok(optarg, ":");
        char *two = strtok(NULL, ":");
        char *three = strtok(NULL, ":");
        char *four = strtok(NULL, ":");

        attacks.pp[0] = (uint8_t) atoi(one);
        attacks.pp[1] = (uint8_t) atoi(two);
        attacks.pp[2] = (uint8_t) atoi(three);
        attacks.pp[3] = (uint8_t) atoi(four);
      }
      break;
    case 'j': // hp ev
      condition.hp_ev = (uint8_t) atoi(optarg);
      break;
    case 'v': // attack ev
      condition.attack_ev = (uint8_t) atoi(optarg);
      break;
    case 'e': // defense ev
      condition.defense_ev = (uint8_t) atoi(optarg);
      break;
    case 'V': // speed ev
      condition.speed_ev = (uint8_t) atoi(optarg);
      break;
    case 'K': // special attack ev
      condition.special_attack_ev = (uint8_t) atoi(optarg);
      break;
    case 'E': // special defense ev
      condition.special_defense_ev = (uint8_t) atoi(optarg);
      break;
    case 'c': // coolness
      condition.coolness = (uint8_t) atoi(optarg);
      break;
    case 'y': // beauty
      condition.beauty = (uint8_t) atoi(optarg);
      break;
    case 'C': // cuteness
      condition.cuteness = (uint8_t) atoi(optarg);
      break;
    case 'r': // smartness
      condition.smartness = (uint8_t) atoi(optarg);
      break;
    case 'T': // toughness
      condition.toughness = (uint8_t) atoi(optarg);
      break;
    case 'F': // feel
      condition.feel = (uint8_t) atoi(optarg);
      break;
    case 'R': // pokérus (days remaining:strain)
      {
        char *days = strtok(optarg, ":");
        char *strain = strtok(NULL, ":");

        misc.pokerus.days_remaining = atoi(days);
        misc.pokerus.strain = atoi(strain);
      }
      break;
    case 'k': // location met at; see bulbapedia:List_of_locations_by_index_number_(Generation_III)
      misc.met_location = (uint8_t) atoi(optarg);
      break;
    case 'M': // level met at
      misc.origins.level_met = atoi(optarg);
      break;
    case 'G': // game met in
      if(!strcmp(optarg, "colosseum-bonus")) {
        misc.origins.game_met = GAME_COLOSSEUM_BONUS;
      } else if(!strcmp(optarg, "sapphire")) {
        misc.origins.game_met = GAME_SAPPHIRE;
      } else if(!strcmp(optarg, "ruby")) {
        misc.origins.game_met = GAME_RUBY;
      } else if(!strcmp(optarg, "emerald")) {
        misc.origins.game_met = GAME_EMERALD;
      } else if(!strcmp(optarg, "firered")) {
        misc.origins.game_met = GAME_FIRERED;
      } else if(!strcmp(optarg, "leafgreen")) {
        misc.origins.game_met = GAME_LEAFGREEN;
      } else if(!strcmp(optarg, "colosseum-xd")) {
        misc.origins.game_met = GAME_COLOSSEUM_XD;
      } else {
        fputs("game must be one of colosseum-bonus|sapphire|ruby|emerald|"
              "firered|leafgreen|colosseum-xd\n", stderr);
        return 1;
      }
      break;
    case 'b': // pokeball used
      if(!strcmp(optarg, "master")) {
        misc.origins.pokeball_type = POKEBALL_MASTER;
      } else if(!strcmp(optarg, "ultra"))  {
        misc.origins.pokeball_type = POKEBALL_ULTRA;
      } else if(!strcmp(optarg, "great"))  {
        misc.origins.pokeball_type = POKEBALL_GREAT;
      } else if(!strcmp(optarg, "standard")) {
        misc.origins.pokeball_type = POKEBALL_STANDARD;
      } else if(!strcmp(optarg, "safari")) {
        misc.origins.pokeball_type = POKEBALL_SAFARI;
      } else if(!strcmp(optarg, "dive")) {
        misc.origins.pokeball_type = POKEBALL_DIVE;
      } else if(!strcmp(optarg, "nest")) {
        misc.origins.pokeball_type = POKEBALL_NEST;
      } else if(!strcmp(optarg, "repeat")) {
        misc.origins.pokeball_type = POKEBALL_REPEAT;
      } else if(!strcmp(optarg, "timer")) {
        misc.origins.pokeball_type = POKEBALL_TIMER;
      } else if(!strcmp(optarg, "luxury")) {
        misc.origins.pokeball_type = POKEBALL_LUXURY;
      } else if(!strcmp(optarg, "premier")) {
        misc.origins.pokeball_type = POKEBALL_PREMIER;
      } else {
        fputs("pokeball must be one of master|ultra|great|standard|safari|"
              "dive|nest|repeat|timer|luxury|premier\n", stderr);
        return 1;
      }
      break;
    case 'H': // hp iv
      misc.ivs.hp = atoi(optarg);
      break;
    case 'a': // attack iv
      misc.ivs.attack = atoi(optarg);
      break;
    case 'd': // defense iv
      misc.ivs.defense = atoi(optarg);
      break;
    case 'S': // speed iv
      misc.ivs.speed = atoi(optarg);
      break;
    case 'A': // special attack iv
      misc.ivs.special_attack = atoi(optarg);
      break;
    case 'D': // special defense iv
      misc.ivs.special_defense = atoi(optarg);
      break;
    case 'g': // is an egg?
      {
        misc.ivs.egg = 1;
        pkmn.language = LANGUAGE_EGG;
      }
      break;
    case '1': // use primary ability (default)
      misc.ivs.ability = ABILITY_PRIMARY;
      break;
    case '2': // use secondary ability
      misc.ivs.ability = ABILITY_SECONDARY;
      break;
    case 'p': // personality
      pkmn.personality = (uint32_t) atoi(optarg);
      break;
    case 't': // trainer id:trainer gender
      {
        char *tid = strtok(optarg, ":");
        char *gender = strtok(NULL, ":");

        pkmn.trainer_id = (uint32_t) atoi(tid);

        if(!strcmp(gender, "male")) {
          misc.origins.trainer_gender = TRAINER_MALE;
        } else if(!strcmp(gender, "female")) {
          misc.origins.trainer_gender = TRAINER_FEMALE;
        } else {
          fputs("gender must be 'male' or 'female'\n", stderr);
          return 1;
        }
      }
      break;
    case 'N': // language met in
      {
        if(misc.ivs.egg || pkmn.language == LANGUAGE_EGG) {
          fputs("Cannot set a language for eggs. Disregarding.\n", stderr);
        } else if(!strcmp(optarg, "ja")) {
          pkmn.language = LANGUAGE_JAPANESE;
        } else if(!strcmp(optarg, "en")) {
          pkmn.language = LANGUAGE_ENGLISH;
        } else if(!strcmp(optarg, "fr")) {
          pkmn.language = LANGUAGE_FRENCH;
        } else if(!strcmp(optarg, "it")) {
          pkmn.language = LANGUAGE_ITALIAN;
        } else if(!strcmp(optarg, "de")) {
          pkmn.language = LANGUAGE_GERMAN;
        } else if(!strcmp(optarg, "ko")) {
          pkmn.language = LANGUAGE_KOREAN;
        } else if(!strcmp(optarg, "es")) {
          pkmn.language = LANGUAGE_SPANISH;
        } else {
          fputs("language must be one of ja|en|fr|it|de|ko|es\n", stderr);
          return 1;
        }
      }
      break;
    case 'n':
        if(!strcmp(optarg,"Hardy")){nature =NATURE_HARDY;}
        else if(!strcmp(optarg,"Lonely")){nature =NATURE_LONELY;}
        else if(!strcmp(optarg,"Brave")){nature =NATURE_BRAVE;}
        else if(!strcmp(optarg,"Adamant")){nature =NATURE_ADAMANT;}
        else if(!strcmp(optarg,"Naughty")){nature =NATURE_NAUGHTY;}
        else if(!strcmp(optarg,"Bold")){nature =NATURE_BOLD;}
        else if(!strcmp(optarg,"Docile")){nature =NATURE_DOCILE;}
        else if(!strcmp(optarg,"Relaxed")){nature =NATURE_RELAXED;}
        else if(!strcmp(optarg,"Impish")){nature =NATURE_IMPISH;}
        else if(!strcmp(optarg,"Lax")){nature =NATURE_LAX;}
        else if(!strcmp(optarg,"Timid")){nature =NATURE_TIMID;}
        else if(!strcmp(optarg,"Hasty")){nature =NATURE_HASTY;}
        else if(!strcmp(optarg,"Serious")){nature =NATURE_SERIOUS;}
        else if(!strcmp(optarg,"Jolly")){nature =NATURE_JOLLY;}
        else if(!strcmp(optarg,"Naive")){nature =NATURE_NAIVE;}
        else if(!strcmp(optarg,"Modest")){nature =NATURE_MODEST;}
        else if(!strcmp(optarg,"Mild")){nature =NATURE_MILD;}
        else if(!strcmp(optarg,"Quiet")){nature =NATURE_QUIET;}
        else if(!strcmp(optarg,"Bashful")){nature =NATURE_BASHFUL;}
        else if(!strcmp(optarg,"Rash")){nature =NATURE_RASH;}
        else if(!strcmp(optarg,"Calm")){nature =NATURE_CALM;}
        else if(!strcmp(optarg,"Gentle")){nature =NATURE_GENTLE;}
        else if(!strcmp(optarg,"Sassy")){nature =NATURE_SASSY;}
        else if(!strcmp(optarg,"Careful")){nature =NATURE_CAREFUL;}
        else if(!strcmp(optarg,"Quirky")){nature =NATURE_QUIRKY;}
        else {fprintf(stderr, "No valid Nature see --help\n");return 1;}
      break;
    case 'L':
      for (int index = 0; index < NUM_SPECIES; index++)
      {
        if(((index % 5) ==0) && index != 0){printf("\n");}
        fprintf(stderr,"%s     \t",species_string[index]);
      }
      printf("\n");
      return 0;
      break;
    case 'I':
      for (int index = 0; index < ITEMS_COUNT+1; index++)
      {
        if(((index % 5) ==0) && index != 0){printf("\n");}
        fprintf(stderr,"%s     \t",item_string[index]);
      }
      printf("\n");
      return 0;
      break;
    case 'u':
      for (int index = 0; index < MOVES_COUNT; index++)
      {
        if(((index % 5) ==0) && index != 0){printf("\n");}
        fprintf(stderr,"%s     \t",move_string[index]);
      }
      printf("\n");
      return 0;
      break;
    case 'l': // pokemon level (recalculated on game save/load)
      pkmn.level = (uint8_t) atoi(optarg);
      break;
    case 'Y': // pokérus remaining cache
      pkmn.pokerus = (uint8_t) atoi(optarg);
      break;

    case 'U':
      shiny = 1;

    case 'o': // dump raw
      dump = false;
      break;
    case 'O': // hexdump for vhdl code
      dump = true;
      break;
    case 'h':
    default:
      fprintf(stderr, usage, argv[0]);
      return 0;
    }
  }

  // check that positional arguments are present
  if(argc < optind + 2) {
    //fprintf(stderr, usage, argv[0]);
    fprintf(stderr,"Usage: %s [options] <pokémon name> <trainer name>\n",argv[0]);
    //return 1;
  }
  else{
      //strncpy(nickname, argv[optind], NICKNAME_LENGTH);
      //strncpy(trainer_name, argv[optind + 1], TRAINER_NAME_LENGTH);
  }
 //Calculate actual Stats for given modification
  
  pkmn.current_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
  pkmn.max_health = calc_HP_stat(condition.hp_ev,misc.ivs.hp,Species_Infos[growth.species].baseHP,pkmn.level);
  pkmn.attack = calc_Stat_Value(condition.attack_ev,misc.ivs.attack,Species_Infos[growth.species].baseAttack,pkmn.level,NatureModifer_Attack(nature));
  pkmn.defense = calc_Stat_Value(condition.defense_ev,misc.ivs.defense,Species_Infos[growth.species].baseDefense,pkmn.level,NatureModifer_Defense(nature));
  pkmn.speed = calc_Stat_Value(condition.speed_ev,misc.ivs.speed,Species_Infos[growth.species].baseSpeed,pkmn.level,NatureModifer_Speed(nature));
  pkmn.special_attack = calc_Stat_Value(condition.special_attack_ev,misc.ivs.special_attack,Species_Infos[growth.species].baseSpAttack,pkmn.level,NatureModifer_SpezAttack(nature));
  pkmn.special_defense = calc_Stat_Value(condition.special_defense_ev,misc.ivs.special_defense,Species_Infos[growth.species].baseSpDefense,pkmn.level,NatureModifer_SpezDefense(nature));

  
  if (pkmn.level == 100){
      growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level]);
  }else{
      growth.experience = (uint32_t) (gExperienceTables[Species_Infos[growth.species].growthRate][pkmn.level+1] -1);
  }
 
  strncpy(nickname, argv[optind], NICKNAME_LENGTH);
  strncpy(trainer_name, argv[optind + 1], TRAINER_NAME_LENGTH);
  
  // Finalize structure
  assert(pcsconv(nickname, NICKNAME_LENGTH, LANGUAGE_ENGLISH) == true);
  memcpy(pkmn.nickname, nickname, NICKNAME_LENGTH);

  assert(pcsconv(trainer_name, TRAINER_NAME_LENGTH, LANGUAGE_ENGLISH) == true);
  memcpy(pkmn.trainer_name, trainer_name, TRAINER_NAME_LENGTH);

  pkmn.personality = get_pers_value(TRAINER_ID,nature,shiny);

  pkmn.checksum = data_encrypt_to(
    pkmn.data,
    pkmn.personality,
    pkmn.trainer_id,
    growth,
    attacks,
    condition,
    misc
  );

  pkmn.personality = get_pers_value(TRAINER_ID,nature,shiny);

  //for (int i = 0; i < NUM_SPECIES; i++){printf("Index %d = %s \n",i,species_string[i]);  }
   printf("Species = %s\n",species_string[growth.species]);
   printf("Nature = %s\n",nature_string[nature]);
   printf("Experience = %d\n",growth.experience);
   //printf("Stat : %d\n",pkmn.special_attack);
   for (int i = 0; i < 4; i++)
   {
    //printf("MOVE%d = %s index = %d\n",i,move_string[attacks.moves[i]],attacks.moves[i]);
   }
   if (dump ==true){
      hexdump_VHDL(&pkmn, sizeof(struct Pokemon),species_string[growth.species]);
   }else{
      hexdump_C (&pkmn, sizeof(struct Pokemon),species_string[growth.species]);}
}
 // calc stats from input 