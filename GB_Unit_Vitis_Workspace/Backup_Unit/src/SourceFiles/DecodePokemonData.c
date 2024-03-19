
#include "../Includes/DataTypes.h"
#include "../Includes/functions.h"
#include "../Includes/structs.h"
#include <stdio.h>

#include "../Includes/moves.h"
#include "../Includes/Pokemonnames.h"
#include "../Includes/items.h"

#include "../Includes/DecodePokemonData.h"

/*
This Function takes a pointer to an uint16_t buffer with 50 elements as input
The functions uses a aux function to decode the Buffer data into the corresponding Pokemon structure
It prints all the individual values of the Pokemon data structure.
There are two version. One prints the  Values to the stdout (console) and the other one prints the output to a textfile

*/


void decode_Pokemon_data(u16 *Pokemonbuffer){
	extern FILE *PokemonFp;

    struct Pokemon Pokemonstruct = decode_Pokemon_structure(Pokemonbuffer);

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

    if(Pokemonstruct.level == 0){
    	fprintf(PokemonFp,"No valid Pokemon on this place\n");
    	return;
    }
#ifdef DEBUG_PRINT
    fprintf(PokemonFp,"\n");
    fprintf(PokemonFp,"Personality Value : %08x\n",Pokemonstruct.personality);
    fprintf(PokemonFp,"Trainer Id : %08x\n",Pokemonstruct.trainer_id);
    fprintf(PokemonFp,"Nickname = %s\n",Pokemonstruct.nickname);
    fprintf(PokemonFp,"Trainername = %s\n",Pokemonstruct.trainer_name);
    //fprintf(PokemonFp,"mark : %d\n",Pokemonstruct.markings);
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        //fprintf(PokemonFp,"DataField[%ld] = %08x\n",i,Pokemonstruct.DataField[i]);
    }
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint8_t));i++){
        //fprintf(PokemonFp,"Data [%ld] : %02x\n",i,Pokemonstruct.data[i]);
    }
    fprintf(PokemonFp,"Status : %d\n",Pokemonstruct.status);
    fprintf(PokemonFp,"Level : %d\n",Pokemonstruct.level);
    fprintf(PokemonFp,"Pokerus : %d\n",Pokemonstruct.pokerus);
    fprintf(PokemonFp,"Current Health : %d\n",Pokemonstruct.current_health);
    fprintf(PokemonFp,"Max Health : %d\n",Pokemonstruct.max_health);
    fprintf(PokemonFp,"Attack : %d\n",Pokemonstruct.attack);
    fprintf(PokemonFp,"Defense : %d\n",Pokemonstruct.defense);
    fprintf(PokemonFp,"Speed : %d\n",Pokemonstruct.speed);
    fprintf(PokemonFp,"Special Attack : %d\n",Pokemonstruct.special_attack);
    fprintf(PokemonFp,"Special Defense : %d\n",Pokemonstruct.special_defense);
    //xil_fprintf(PokemonFp,"Growth : \n");
    fprintf(PokemonFp,"Species : %d %s\n",growth.species,species_string[growth.species]);
    fprintf(PokemonFp,"Held Item : %d %s\n",growth.held_item,item_string_file[growth.held_item]);
    fprintf(PokemonFp,"Experience : %d\n",growth.experience);
    fprintf(PokemonFp,"PP Bnous Move1 %d\n",growth.pp_bonus.move1);
    fprintf(PokemonFp,"PP Bnous Move2 %d\n",growth.pp_bonus.move2);
    fprintf(PokemonFp,"PP Bnous Move3 %d\n",growth.pp_bonus.move3);
    fprintf(PokemonFp,"PP Bnous Move4 %d\n",growth.pp_bonus.move4);
    fprintf(PokemonFp,"Friendship : %d\n",growth.friendship);

    for(size_t i = 0;i<4;i++){
        fprintf(PokemonFp,"Move%d : %d %s PP : %d\n",i+1,attacks.moves[i],move_string_file[attacks.moves[i]],attacks.pp[i]);
    }

    fprintf(PokemonFp,"HP EV : %d\n",condition.hp_ev);
    fprintf(PokemonFp,"Attack_EV : %d\n",condition.attack_ev);
    fprintf(PokemonFp,"Defense_EV : %d\n",condition.defense_ev);
    fprintf(PokemonFp,"Speed_EV : %d\n",condition.speed_ev);
    fprintf(PokemonFp,"SpecialAttack_EV : %d\n",condition.special_attack_ev);
    fprintf(PokemonFp,"SpecialDefense_EV : %d\n",condition.special_defense_ev);
    fprintf(PokemonFp,"Coolness : %d\n",condition.coolness);
    fprintf(PokemonFp,"Beauty: %d\n",condition.beauty);
    fprintf(PokemonFp,"Cuteness : %d\n",condition.cuteness);
    fprintf(PokemonFp,"Smartness : %d\n",condition.smartness);
    fprintf(PokemonFp,"Toughness : %d\n",condition.toughness);
    fprintf(PokemonFp,"Feel : %d\n",condition.feel);

    fprintf(PokemonFp,"Pokerus days remanining : %d  Strain : %d \n", misc.pokerus.days_remaining,misc.pokerus.strain);
    fprintf(PokemonFp,"Met Location : %d\n",misc.met_location);
    fprintf(PokemonFp,"Level met: %d\n",misc.origins.level_met);
    fprintf(PokemonFp,"Game met : %d\n",misc.origins.game_met);
    fprintf(PokemonFp,"Pokeball Type : %d\n",misc.origins.pokeball_type);
    fprintf(PokemonFp,"Trainder gender : %d\n",misc.origins.trainer_gender);
    fprintf(PokemonFp,"HP_IV: %d\n",misc.ivs.hp);
    fprintf(PokemonFp,"Attack_IV : %d\n",misc.ivs.attack);
    fprintf(PokemonFp,"Defense_IV : %d\n",misc.ivs.defense);
    fprintf(PokemonFp,"Speed_IV : %d\n",misc.ivs.speed);
    fprintf(PokemonFp,"SpecialAttack_IV : %d\n",misc.ivs.special_attack);
    fprintf(PokemonFp,"SpecialDefense_IV : %d\n",misc.ivs.special_defense);
    fprintf(PokemonFp,"Egg: %d\n",misc.ivs.egg);
    fprintf(PokemonFp,"Ability : %d\n",misc.ivs.ability);


    fprintf(PokemonFp,"Ribbons:\nCool: %d Beauty : %d Cute : %d Smart : %d\nTough : %d Champion : %d Winning : %d victory : %d\nartist : %d effort : %d special1 : %d special2 : %d\nspecial3 : %d special4 : %d special 5 : %d special6 : %d\n",misc.ribbons.cool,misc.ribbons.beauty,
    misc.ribbons.cute,misc.ribbons.smart,misc.ribbons.tough,misc.ribbons.champion,misc.ribbons.winning,
    misc.ribbons.victory,misc.ribbons.artist,misc.ribbons.effort,misc.ribbons.special1,
    misc.ribbons.special2,misc.ribbons.special3,misc.ribbons.special4,misc.ribbons.special5,
    misc.ribbons.special6);
    fprintf(PokemonFp,"Obedience : %d\n",misc.ribbons.obedience);


#endif


    return;
}


struct Pokemon PrintDecodedPokemonData(u16 *Pokemonbuffer){
	extern FILE *PokemonFp;

    struct Pokemon Pokemonstruct = decode_Pokemon_structure(Pokemonbuffer);

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

    if(Pokemonstruct.level == 0){
    	printf("No valid Pokemon on this place\n");
    	return Pokemonstruct;
    }
#ifdef DEBUG_PRINT
    printf("\n");
    printf("Personality Value : %08x\n",Pokemonstruct.personality);
    printf("Trainer Id : %08x\n",Pokemonstruct.trainer_id);
    printf("Nickname = %.10s\n",Pokemonstruct.nickname);
    printf("Trainername = %s\n",Pokemonstruct.trainer_name);
    //fprintf(PokemonFp,"mark : %d\n",Pokemonstruct.markings);
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        //printf(PokemonFp,"DataField[%ld] = %08x\n",i,Pokemonstruct.DataField[i]);
    }
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint8_t));i++){
        //printf(PokemonFp,"Data [%ld] : %02x\n",i,Pokemonstruct.data[i]);
    }
    printf("Status : %d\n",Pokemonstruct.status);
    printf("Level : %d\n",Pokemonstruct.level);
    printf("Pokerus : %d\n",Pokemonstruct.pokerus);
    printf("Current Health : %d\n",Pokemonstruct.current_health);
    printf("Max Health : %d\n",Pokemonstruct.max_health);
    printf("Attack : %d\n",Pokemonstruct.attack);
    printf("Defense : %d\n",Pokemonstruct.defense);
    printf("Speed : %d\n",Pokemonstruct.speed);
    printf("Special Attack : %d\n",Pokemonstruct.special_attack);
    printf("Special Defense : %d\n",Pokemonstruct.special_defense);
    //xil_printf(PokemonFp,"Growth : \n");
    printf("Species : %d %s\n",growth.species,species_string[growth.species]);
    printf("Held Item : %d %s\n",growth.held_item,item_string_file[growth.held_item]);
    printf("Experience : %d\n",growth.experience);
    printf("PP Bnous Move1 %d\n",growth.pp_bonus.move1);
    printf("PP Bnous Move2 %d\n",growth.pp_bonus.move2);
    printf("PP Bnous Move3 %d\n",growth.pp_bonus.move3);
    printf("PP Bnous Move4 %d\n",growth.pp_bonus.move4);
   printf("Friendship : %d\n",growth.friendship);

    for(size_t i = 0;i<4;i++){
       printf("Move%d : %d %s PP : %d\n",i+1,attacks.moves[i],move_string_file[attacks.moves[i]],attacks.pp[i]);
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


   return Pokemonstruct;
}
