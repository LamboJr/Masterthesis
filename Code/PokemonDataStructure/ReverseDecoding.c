#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "species.h"
#include "items.h"
#include "constants.h"
#include "abilities.h"
#include "experience_tables.h"
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
#include "parsing_File.h"

#include "ReverseDecoding.h"

#define DEBUG_PRINT

void DecodeDataField(uint16_t checksum,uint32_t personality,uint32_t trainer_id,uint32_t *DataField);

struct Pokemon decode_Pokemon_data(uint16_t *Pokemonbuffer){
	struct Pokemon Pokemonstruct;
    uint16_t* PokemonPtr = (uint16_t* )&Pokemonstruct;
    //printf("Starraddresse Struct : %p\n",PokemonPtr);
    for (size_t i = 0;i <BUFFER_SIZE;i++){
        *PokemonPtr = Pokemonbuffer[i];
        //printf("Pokemonstruct addr : %p Value = %04x\n",PokemonPtr,*PokemonPtr);
        PokemonPtr++;
    }
    for(size_t i = 0; i < NICKNAME_LENGTH;i++){
        Pokemonstruct.nickname[i] = char_conv(Pokemonstruct.nickname[i],LANGUAGE_GERMAN);
    }
    for(size_t i = 0;i<TRAINER_NAME_LENGTH;i++){
        Pokemonstruct.trainer_name[i] = char_conv(Pokemonstruct.trainer_name[i],LANGUAGE_GERMAN);
    }
    uint32_t* DataField = (uint32_t* )Pokemonstruct.data;
    DecodeDataField(Pokemonstruct.checksum,Pokemonstruct.personality,Pokemonstruct.trainer_id,DataField);

#ifdef DEBUG_PRINT
    printf("Personality Value: %08x\n",Pokemonstruct.personality);
    printf("Trainer Id : %08x\n",Pokemonstruct.trainer_id);
    printf("Nickname = %s\n",Pokemonstruct.nickname);
    printf("Trainername = %s\n",Pokemonstruct.trainer_name);
    //printf("mark : %d\n",Pokemonstruct.markings);
    /* for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        printf("DataField[%ld] = %08x\n",i,DataField[i]);
    } */
    /* for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint8_t));i++){
        printf("Data [%ld] : %02x\n",i,Pokemonstruct.data[i]);
     }*/
    //(struct Status)* PokemonstructStatus = Pokemonstruct.status; 
    uint32_t* TempValue = (uint32_t *) &Pokemonstruct.status;
    printf("Status : %d\n",*TempValue);
    printf("Level : %d\n",Pokemonstruct.level);
    printf("Pokerus : %d\n",Pokemonstruct.pokerus);
    printf("Current Health : %d\n",Pokemonstruct.current_health);
    printf("Max Health : %d\n",Pokemonstruct.max_health);
    printf("Attack : %d\n",Pokemonstruct.attack);
    printf("Defense : %d\n",Pokemonstruct.defense);
    printf("Speed : %d\n",Pokemonstruct.speed);
    printf("Special Attack : %d\n",Pokemonstruct.special_attack);
    printf("Special Defense : %d\n",Pokemonstruct.special_defense);
#endif
    return Pokemonstruct;
}
/*#define DATA_LENGTH 48
#define DATUM_LENGTH 12
#define DATUM_PER_DATA (DATA_LENGTH / DATUM_LENGTH)
*/

void DecodeDataField(uint16_t checksum,uint32_t personality,uint32_t trainer_id,uint32_t *DataField){
    
    //uint16_t DataField16[DATA_LENGTH / sizeof(uint16_t)];

    uint32_t order = personality % 24;
    //printf("Order : %d\n",order);
    /*
    00. GAEM	06. AGEM	12. EGAM	18. MGAE
    01. GAME	07. AGME	13. EGMA	19. MGEA
    02. GEAM	08. AEGM	14. EAGM	20. MAGE
    03. GEMA	09. AEMG	15. EAMG	21. MAEG
    04. GMAE	10. AMGE	16. EMGA	22. MEGA
    05. GMEA	11. AMEG	17. EMAG	23. MEAG
    */
    // Find data ordering
    /*
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint16_t)); i++) {
        DataField16[i] = ((uint16_t *) DataField)[i];
        //printf("DataField16[%ld] = %04x or %d\n",i,DataField16[i],DataField16[i]);
    }*/
    //Decrypt data
    uint32_t key = personality ^ trainer_id;
    //printf("Key : %08x\n",key);
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        DataField[i] ^= key;
    }

    //Calc checksum
    uint16_t cksum = 0;
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint16_t)); i++) {
        cksum += ((uint16_t *) DataField)[i];
        //DataField16[i] = ((uint16_t *) DataField)[i];
        //printf("DataField16[%ld] = %04x or %d\n",i,DataField16[i],DataField16[i]);
    }
    //printf("Checksum : %04x\n",cksum);
    
    uint8_t growthslot,attacksslot,miscslot,conditionslot;

    switch(order) {
    case 0 ... 5:
        growthslot = 0;
        break;
    case 6 ... 11:
        attacksslot = 0;
        break;
    case 12 ... 17:
        conditionslot = 0;
        break;
    case 18 ...23 :
        miscslot = 0;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 1:
    case 14:
    case 15:
    case 20:
    case 21:
        attacksslot = 1;
        break;
    case 2:
    case 3:
    case 8:
    case 9:
    case 22:
    case 23:
        conditionslot = 1;
        break;
    case 4:
    case 5:
    case 10:
    case 11:
    case 16:
    case 17:
        miscslot = 1;
        break;
    case 6:
    case 7:
    case 12:
    case 13:
    case 18:
    case 19:
        growthslot = 1;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 5:
    case 6:
    case 11:
    case 19:
    case 21:
        conditionslot = 2;
        break;
    case 1:
    case 3:
    case 7:
    case 9:
    case 13:
    case 15:
        miscslot = 2;
        break;
    case 2:
    case 4:
    case 12:
    case 17:
    case 18:
    case 23:
        attacksslot = 2;
        break;
    case 8:
    case 10:
    case 14:
    case 16:
    case 20:
    case 22:
        growthslot = 2;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 2:
    case 6:
    case 8:
    case 12:
    case 14:
        miscslot = 3;
        break;
    case 1:
    case 4:
    case 7:
    case 10:
    case 18:
    case 20:
        conditionslot = 3;
        break;
    case 3:
    case 5:
    case 13:
    case 16:
    case 19:
    case 22:
        attacksslot = 3;
        break;
    case 9:
    case 11:
    case 15:
    case 17:
    case 21:
    case 23:
        growthslot = 3;
        break;
    default: abort();
    };

    //uint16_t DataSorted[DATUM_PER_DATA][DATUM_LENGTH/sizeof(uint16_t)];
    uint32_t DataSorted32[DATUM_PER_DATA][DATUM_LENGTH / sizeof(uint32_t)];
    /*
    for(size_t i = 0;i < DATUM_LENGTH/sizeof(uint16_t);i++){
        DataSorted[0][i] = DataField16[i+(growthslot * DATUM_LENGTH/sizeof(uint16_t))];
        DataSorted[1][i] = DataField16[i+(attacksslot * DATUM_LENGTH/sizeof(uint16_t))];
        DataSorted[2][i] = DataField16[i+(conditionslot * DATUM_LENGTH/sizeof(uint16_t))];
        DataSorted[3][i] = DataField16[i+(miscslot * DATUM_LENGTH/sizeof(uint16_t))];
        //printf("DataSorted[0][%ld] = %04x\n",i,DataSorted[0][i]);
    }*/
    for(size_t i = 0;i < DATUM_LENGTH/sizeof(uint32_t);i++){
        DataSorted32[0][i] = DataField[i+(growthslot * DATUM_LENGTH/sizeof(uint32_t))];
        DataSorted32[1][i] = DataField[i+(attacksslot * DATUM_LENGTH/sizeof(uint32_t))];
        DataSorted32[2][i] = DataField[i+(conditionslot * DATUM_LENGTH/sizeof(uint32_t))];
        DataSorted32[3][i] = DataField[i+(miscslot * DATUM_LENGTH/sizeof(uint32_t))];
        //printf("DataSorted[0][%ld] = %04x\n",i,DataSorted[0][i]);
    }
    for(size_t i = 0; i < DATUM_PER_DATA;i++){
        for( size_t u = 0;u< (DATUM_LENGTH / sizeof(uint32_t));u++){
            //DataField[((DATUM_LENGTH / sizeof(uint32_t))*i)+u] = ((uint32_t *) DataSorted[i])[u];
            DataField[((DATUM_LENGTH / sizeof(uint32_t))*i)+u] = DataSorted32[i][u];
            //printf("Datafield debug [%ld] : %08x\n",((DATUM_LENGTH / sizeof(uint32_t))*i)+u,DataField[((DATUM_LENGTH / sizeof(uint32_t))*i)+u]);
        }
    }
    return;
}


int DecodeBuffer(uint16_t *Pokemonbuffer){

    struct Pokemon Pokemonstruct = decode_Pokemon_data(Pokemonbuffer);

    uint8_t* PokemonDataPtr = (uint8_t* )Pokemonstruct.data;

    struct Growth growth;
    uint8_t* GrowthPtr = (uint8_t* )&growth;
    for(size_t i = 0; i< DATUM_LENGTH;i++){
        *GrowthPtr++ = *PokemonDataPtr++;
    }
    struct Attacks attacks;
    uint8_t* AttacksPtr = (uint8_t* )&attacks;
    for(size_t i = 0; i< DATUM_LENGTH;i++){
        *AttacksPtr++ = *PokemonDataPtr++;
    }

    struct Condition condition;
    uint8_t* ConditionPtr = (uint8_t* )&condition;
    for(size_t i = 0; i< DATUM_LENGTH;i++){
        *ConditionPtr++ = *PokemonDataPtr++;
    }
    struct Misc misc;
    uint8_t* MiscPtr = (uint8_t* )&misc;
    for(size_t i = 0; i< DATUM_LENGTH;i++){
        *MiscPtr++ = *PokemonDataPtr++;
    }



#ifdef DEBUG_PRINT
    //printf("Growth : \n");
    printf("Species : %d %s\n",growth.species,species_string[growth.species]);
    printf("Held Item : %d %s\n",growth.held_item,item_string_file[growth.held_item]);
    printf("Experience : %d\n",growth.experience);
    printf("PP Bnous Move1 %d\n",growth.pp_bonus.move1);
    printf("PP Bnous Move2 %d\n",growth.pp_bonus.move2);
    printf("PP Bnous Move3 %d\n",growth.pp_bonus.move3);
    printf("PP Bnous Move4 %d\n",growth.pp_bonus.move4);
    printf("Friendship : %d\n",growth.friendship);

    for(size_t i = 0;i<4;i++){
        printf("Move%ld : %d %s PP : %d\n",i+1,attacks.moves[i],move_string_file[attacks.moves[i]],attacks.pp[i]);
    }

    printf("HP EV : %d\n",condition.hp_ev);
    printf("Attack_EV : %d\n",condition.attack_ev);
    printf("Defense_EV : %d\n",condition.defense_ev);
    printf("Speed_EV : %d\n",condition.speed_ev);
    printf("SpecialAttack_EV : %d\n",condition.special_attack_ev);
    printf("SpecialDefense_EV : %d\n",condition.special_defense_ev);
    printf("Coolness : %d\n",condition.coolness);
    printf("Beauty: %d\n",condition.beauty);
    printf("Cuteness : %d\n",condition.cuteness);
    printf("Smartness : %d\n",condition.smartness);
    printf("Toughness : %d\n",condition.toughness);
    printf("Feel : %d\n",condition.feel);

    printf("Pokerus days remanining : %d  Strain : %d \n", misc.pokerus.days_remaining,misc.pokerus.strain);
    printf("Met Location : %d\n",misc.met_location);
    printf("Level met: %d\n",misc.origins.level_met);
    printf("Game met : %d\n",misc.origins.game_met);
    printf("Pokeball Type : %d\n",misc.origins.pokeball_type);
    printf("Trainder gender : %d\n",misc.origins.trainer_gender);
    printf("HP_IV: %d\n",misc.ivs.hp);
    printf("Attack_IV : %d\n",misc.ivs.attack);
    printf("Defense_IV : %d\n",misc.ivs.defense);
    printf("Speed_IV : %d\n",misc.ivs.speed);
    printf("SpecialAttack_IV : %d\n",misc.ivs.special_attack);
    printf("SpecialDefense_IV : %d\n",misc.ivs.special_defense);
    printf("Egg: %d\n",misc.ivs.egg);
    printf("Ability : %d\n",misc.ivs.ability);


    printf("Ribbons:\nCool: %d Beauty : %d Cute : %d Smart : %d\nTough : %d Champion : %d Winning : %d victory : %d\nartist : %d effort : %d special1 : %d special2 : %d\nspecial3 : %d special4 : %d special 5 : %d special6 : %d\n",misc.ribbons.cool,misc.ribbons.beauty,
    misc.ribbons.cute,misc.ribbons.smart,misc.ribbons.tough,misc.ribbons.champion,misc.ribbons.winning,
    misc.ribbons.victory,misc.ribbons.artist,misc.ribbons.effort,misc.ribbons.special1,
    misc.ribbons.special2,misc.ribbons.special3,misc.ribbons.special4,misc.ribbons.special5,
    misc.ribbons.special6);
    printf("Obedience : %d\n",misc.ribbons.obedience);
#endif
    uint32_t* TempValue = (uint32_t *) &Pokemonstruct.status;
  /*  
    assert(Pokemonstruct.personality == 0x41f30002);
    assert(Pokemonstruct.trainer_id == 0x06A64753);
    assert(strcmp(Pokemonstruct.nickname,"Kyogre")== 0);
    assert(Pokemonstruct.language == 0x0205);
    assert(strcmp(Pokemonstruct.trainer_name,"Lambo")== 0);
    assert(Pokemonstruct.markings == 0);
    assert(Pokemonstruct.checksum == 0x1089);
    assert(Pokemonstruct.unknown == 0);
    //assert(Pokemonstruct.status == 0);
    assert(*TempValue == 0);
    assert(Pokemonstruct.level == 100);
    assert(Pokemonstruct.pokerus = 0xFF);
    assert(Pokemonstruct.current_health == 0x155);
    assert(Pokemonstruct.max_health == 0x155);
    assert(Pokemonstruct.attack == 0xb8);
    assert(Pokemonstruct.defense == 0xd8);
    assert(Pokemonstruct.speed == 0x117);
    assert(Pokemonstruct.special_attack ==0x1b6);
    assert(Pokemonstruct.special_defense == 0x13c);
*/
    return 0;
}