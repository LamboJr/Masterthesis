#include "decode_Pokemon_structure.h"
#include "moves.h"
#include "Pokemonnames.h"
#include "items.h"


void decode_Pokemon_data(u16 *Pokemonbuffer){
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
    	xil_printf("No Pokemon on this place\n");
    	return;
    }
#ifdef DEBUG_PRINT
    xil_printf("\n");
    xil_printf("Personality Value : %08x\n",Pokemonstruct.personality);
    xil_printf("Trainer Id : %08x\n",Pokemonstruct.trainer_id);
    xil_printf("Nickname = %s\n",Pokemonstruct.nickname);
    xil_printf("Trainername = %s\n",Pokemonstruct.trainer_name);
    //xil_printf("mark : %d\n",Pokemonstruct.markings);
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        //xil_printf("DataField[%ld] = %08x\n",i,Pokemonstruct.DataField[i]);
    }
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint8_t));i++){
        //xil_printf("Data [%ld] : %02x\n",i,Pokemonstruct.data[i]);
    }
    xil_printf("Status : %d\n",Pokemonstruct.status);
    xil_printf("Level : %d\n",Pokemonstruct.level);
    xil_printf("Pokerus : %d\n",Pokemonstruct.pokerus);
    xil_printf("Current Health : %d\n",Pokemonstruct.current_health);
    xil_printf("Max Health : %d\n",Pokemonstruct.max_health);
    xil_printf("Attack : %d\n",Pokemonstruct.attack);
    xil_printf("Defense : %d\n",Pokemonstruct.defense);
    xil_printf("Speed : %d\n",Pokemonstruct.speed);
    xil_printf("Special Attack : %d\n",Pokemonstruct.special_attack);
    xil_printf("Special Defense : %d\n",Pokemonstruct.special_defense);
    //xil_xil_printf("Growth : \n");
    xil_printf("Species : %d %s\n",growth.species,species_string[growth.species]);
    xil_printf("Held Item : %d %s\n",growth.held_item,item_string_file[growth.held_item]);
    xil_printf("Experience : %d\n",growth.experience);
    xil_printf("PP Bnous Move1 %d\n",growth.pp_bonus.move1);
    xil_printf("PP Bnous Move2 %d\n",growth.pp_bonus.move2);
    xil_printf("PP Bnous Move3 %d\n",growth.pp_bonus.move3);
    xil_printf("PP Bnous Move4 %d\n",growth.pp_bonus.move4);
    xil_printf("Friendship : %d\n",growth.friendship);

    for(size_t i = 0;i<4;i++){
        xil_printf("Move%ld : %d %s PP : %d\n",i+1,attacks.moves[i],move_string_file[attacks.moves[i]],attacks.pp[i]);
    }

    xil_printf("HP EV : %d\n",condition.hp_ev);
    xil_printf("Attack_EV : %d\n",condition.attack_ev);
    xil_printf("Defense_EV : %d\n",condition.defense_ev);
    xil_printf("Speed_EV : %d\n",condition.speed_ev);
    xil_printf("SpecialAttack_EV : %d\n",condition.special_attack_ev);
    xil_printf("SpecialDefense_EV : %d\n",condition.special_defense_ev);
    xil_printf("Coolness : %d\n",condition.coolness);
    xil_printf("Beauty: %d\n",condition.beauty);
    xil_printf("Cuteness : %d\n",condition.cuteness);
    xil_printf("Smartness : %d\n",condition.smartness);
    xil_printf("Toughness : %d\n",condition.toughness);
    xil_printf("Feel : %d\n",condition.feel);

    xil_printf("Pokerus days remanining : %d  Strain : %d \n", misc.pokerus.days_remaining,misc.pokerus.strain);
    xil_printf("Met Location : %d\n",misc.met_location);
    xil_printf("Level met: %d\n",misc.origins.level_met);
    xil_printf("Game met : %d\n",misc.origins.game_met);
    xil_printf("Pokeball Type : %d\n",misc.origins.pokeball_type);
    xil_printf("Trainder gender : %d\n",misc.origins.trainer_gender);
    xil_printf("HP_IV: %d\n",misc.ivs.hp);
    xil_printf("Attack_IV : %d\n",misc.ivs.attack);
    xil_printf("Defense_IV : %d\n",misc.ivs.defense);
    xil_printf("Speed_IV : %d\n",misc.ivs.speed);
    xil_printf("SpecialAttack_IV : %d\n",misc.ivs.special_attack);
    xil_printf("SpecialDefense_IV : %d\n",misc.ivs.special_defense);
    xil_printf("Egg: %d\n",misc.ivs.egg);
    xil_printf("Ability : %d\n",misc.ivs.ability);


    xil_printf("Ribbons:\nCool: %d Beauty : %d Cute : %d Smart : %d\nTough : %d Champion : %d Winning : %d victory : %d\nartist : %d effort : %d special1 : %d special2 : %d\nspecial3 : %d special4 : %d special 5 : %d special6 : %d\n",misc.ribbons.cool,misc.ribbons.beauty,
    misc.ribbons.cute,misc.ribbons.smart,misc.ribbons.tough,misc.ribbons.champion,misc.ribbons.winning,
    misc.ribbons.victory,misc.ribbons.artist,misc.ribbons.effort,misc.ribbons.special1,
    misc.ribbons.special2,misc.ribbons.special3,misc.ribbons.special4,misc.ribbons.special5,
    misc.ribbons.special6);
    xil_printf("Obedience : %d\n",misc.ribbons.obedience);


#endif


    return;
}
