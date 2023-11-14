#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"

#include "structs.h"
#include "constant.h"
#include "functions.h"
#include "items.h"
#include "Pokemonnames.h"
#include "moves.h"

#define FrameMask 0xFFFF0000

char char_conv( u8 character,u16 language);

#define DEBUG_PRINT

void decode_Pokemon_data(u16 *Pokemonbuffer){
    struct Pokemon Pokemonteststructure;
    Pokemonteststructure = decode_Pokemon_structure(Pokemonbuffer);

    struct Growth growth;
    uint8_t offset32 = 0;
    uint8_t offset16 = 0;
    uint8_t offset8 = 0;

    growth.species = ((uint16_t *) Pokemonteststructure.data)[0];
    growth.held_item = ((uint16_t *) Pokemonteststructure.data)[1];
    growth.experience = ((uint32_t *) Pokemonteststructure.data)[1];
    growth.pp_bonus.move1 = Pokemonteststructure.data[8] & 0x3;
    growth.pp_bonus.move2 = (Pokemonteststructure.data[8] >>2 ) & 0x3;
    growth.pp_bonus.move3 = (Pokemonteststructure.data[8] >>4 ) & 0x3;
    growth.pp_bonus.move4 = (Pokemonteststructure.data[8] >>6 ) & 0x3;
    growth.friendship = Pokemonteststructure.data[9];

    struct Attacks attacks;
    offset8 = 12;
    offset16 = 6;
    attacks.moves[0] = ((uint16_t *)Pokemonteststructure.data)[offset16++];
    attacks.moves[1] = ((uint16_t *)Pokemonteststructure.data)[offset16++];
    attacks.moves[2] = ((uint16_t *)Pokemonteststructure.data)[offset16++];
    attacks.moves[3] = ((uint16_t *)Pokemonteststructure.data)[offset16++];
    attacks.pp[0] = Pokemonteststructure.data[8+offset8];
    attacks.pp[1] = Pokemonteststructure.data[9+offset8];
    attacks.pp[2] = Pokemonteststructure.data[10+offset8];
    attacks.pp[3] = Pokemonteststructure.data[11+offset8];

    struct Condition condition;

    offset8 = 24;
    condition.hp_ev = Pokemonteststructure.data[offset8++];
    condition.attack_ev = Pokemonteststructure.data[offset8++];
    condition.defense_ev = Pokemonteststructure.data[offset8++];
    condition.speed_ev = Pokemonteststructure.data[offset8++];
    condition.special_attack_ev = Pokemonteststructure.data[offset8++];
    condition.special_defense_ev = Pokemonteststructure.data[offset8++];
    condition.coolness = Pokemonteststructure.data[offset8++];
    condition.beauty = Pokemonteststructure.data[offset8++];
    condition.cuteness = Pokemonteststructure.data[offset8++];
    condition.smartness = Pokemonteststructure.data[offset8++];
    condition.toughness = Pokemonteststructure.data[offset8++];
    condition.feel = Pokemonteststructure.data[offset8++];

    offset16 = 18;
    offset32 = 9;
    struct Misc misc;

    misc.pokerus.days_remaining = Pokemonteststructure.data[offset8] & 0xF;
    misc.pokerus.strain = (Pokemonteststructure.data[offset8++] >> 4) &0xF;
    misc.met_location = Pokemonteststructure.data[offset8++];
    offset16++;

    uint16_t tempData16 = ((uint16_t *)Pokemonteststructure.data)[offset16++];
    offset32++;

    misc.origins.level_met = tempData16 & 0x7F;
    misc.origins.game_met = (tempData16 >> 7) & 0xF;
    misc.origins.pokeball_type = (tempData16 >>(7+4)) & 0xF;
    misc.origins.trainer_gender = (tempData16 >> (7+4+4)) & 0x1;
    uint32_t tempData32 = ((uint32_t *)Pokemonteststructure.data)[offset32++];

    misc.ivs.hp = tempData32 & 0x1F;
    misc.ivs.attack = (tempData32 >> 5) & 0x1F;
    misc.ivs.defense = (tempData32 >> (5+5)) & 0x1F;
    misc.ivs.speed = (tempData32 >> (5+5+5)) & 0x1F;
    misc.ivs.special_attack = (tempData32 >> (5+5+5+5)) & 0x1F;
    misc.ivs.special_defense = (tempData32 >> (5+5+5+5+5)) & 0x1F;
    misc.ivs.egg = (tempData32 >> (5+5+5+5+5+5)) & 0x1;
    misc.ivs.ability = (tempData32 >> (5+5+5+5+5+5+1)) & 0x1;

    tempData32 = ((uint32_t *)Pokemonteststructure.data)[offset32];

    misc.ribbons.cool = tempData32 & 0x7;
    misc.ribbons.beauty = (tempData32 >> 3)  & 0x7;
    misc.ribbons.cute = (tempData32 >> (3+3))  & 0x7;
    misc.ribbons.smart = (tempData32 >> (6+3))  & 0x7;
    misc.ribbons.tough = (tempData32 >> (9+3))  & 0x7;
    misc.ribbons.champion = (tempData32 >> (12+3))  & 0x1;
    misc.ribbons.winning = (tempData32 >> 16)  & 0x1;
    misc.ribbons.victory= (tempData32 >> 17)  & 0x1;
    misc.ribbons.artist= (tempData32 >> 18)  & 0x1;
    misc.ribbons.effort= (tempData32 >> 19)  & 0x1;
    misc.ribbons.special1= (tempData32 >> 20)  & 0x1;
    misc.ribbons.special2= (tempData32 >> 21)  & 0x1;
    misc.ribbons.special3= (tempData32 >> 22)  & 0x1;
    misc.ribbons.special4= (tempData32 >> 23)  & 0x1;
    misc.ribbons.special5= (tempData32 >> 24)  & 0x1;
    misc.ribbons.special6= (tempData32 >> 25)  & 0x1;

    misc.ribbons.padding= (tempData32 >> 26)  & 0x1F;
    misc.ribbons.obedience= (tempData32 >> 31)  & 0x1;

#ifdef DEBUG_PRINT
    xil_printf("\n");
    xil_printf("Personality Value : %08x\n",Pokemonteststructure.personality);
    xil_printf("Trainer Id : %08x\n",Pokemonteststructure.trainer_id);
    xil_printf("Nickname = %s\n",Pokemonteststructure.nickname);
    xil_printf("Trainername = %s\n",Pokemonteststructure.trainer_name);
    //xil_printf("mark : %d\n",Pokemonstruct.markings);
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
        //xil_printf("DataField[%ld] = %08x\n",i,Pokemonteststructure.DataField[i]);
    }
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint8_t));i++){
        //xil_printf("Data [%ld] : %02x\n",i,Pokemonteststructure.data[i]);
    }
    xil_printf("Status : %d\n",Pokemonteststructure.status);
    xil_printf("Level : %d\n",Pokemonteststructure.level);
    xil_printf("Pokerus : %d\n",Pokemonteststructure.pokerus);
    xil_printf("Current Health : %d\n",Pokemonteststructure.current_health);
    xil_printf("Max Health : %d\n",Pokemonteststructure.max_health);
    xil_printf("Attack : %d\n",Pokemonteststructure.attack);
    xil_printf("Defense : %d\n",Pokemonteststructure.defense);
    xil_printf("Speed : %d\n",Pokemonteststructure.speed);
    xil_printf("Special Attack : %d\n",Pokemonteststructure.special_attack);
    xil_printf("Special Defense : %d\n",Pokemonteststructure.special_defense);
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

