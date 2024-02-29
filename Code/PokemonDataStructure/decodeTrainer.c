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

uint16_t TrainerBuffer[50] = {
 0x0100    ,0x0001    ,0x7200    ,0x0021    ,0x0020    ,0x0017    ,0x009D
,0xC371    ,0x007B    ,0x0026    ,0x0001    ,0x0000    ,0x0000    ,0x0000
,0x0000    ,0x0000    ,0x003F    ,0x0000    ,0x141C    ,0x0002    ,0x0A29
,0x1413    ,0x1037    ,0x020E    ,0xD5BE    ,0xDDE2    ,0xE0D9    ,0x6EFF
,0x2E05    ,0x0000    ,0x0000    ,0x0000    ,0x000A    ,0x0000    ,0x0000
,0x0000    ,0x0000    ,0x0000    ,0x0001    ,0x6F00    ,0x0101    ,0x0000
,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
,0x0000
};

struct TrainerCard decodeTrainerCard(uint16_t* TrainerBuffer){
    struct TrainerCard trainercard;
    printf("Size TrainerCard : %ld\n",sizeof(struct TrainerCard));
    uint16_t* TrainerPtr = (uint16_t* )&trainercard;

        for (size_t i = 0;i <BUFFER_SIZE;i++){
        *TrainerPtr = TrainerBuffer[i];
        printf("Trainerstruct[%ld] addr : %p Value = %04x\n",i,TrainerPtr,*TrainerPtr);
        TrainerPtr++;
    }
    for(size_t i = 0;i<TRAINER_NAME_LENGTH+1;i++){
        trainercard.playerName[i] = char_conv(trainercard.playerName[i],LANGUAGE_GERMAN);
    }
    printf("Gender : %d\n",trainercard.gender);
    printf("Stars : %d\n",trainercard.stars);
    printf("HasPokedex : %d\n",trainercard.hasPokedex);
    printf("CaughtAllHoen : %d\n",trainercard.caughtAllHoenn);
    printf("hasAllPaintings : %d\n",trainercard.hasAllPaintings);
    printf("HOFDebutHours : %d\n",trainercard.hofDebutHours);
    printf("HOFDebutMinutes : %d\n",trainercard.hofDebutMinutes);
    printf("HOFDebutSeconds : %d\n",trainercard.hofDebutSeconds);
    printf("caughtMonscount : %d\n",trainercard.caughtMonsCount);
    printf("trainerId : %d\n",trainercard.trainerId);
    printf("PlayTimeHours : %d\n",trainercard.playTimeHours);
    printf("PlayTimeMinutes : %d\n",trainercard.playTimeMinutes);
    printf("LinkBattleWins : %d\n",trainercard.linkBattleWins);
    printf("LinkBattleLosses : %d\n",trainercard.linkBattleLosses);
    printf("BattleTowerWins : %d\n",trainercard.battleTowerWins);
    printf("BattleTowerStarightWins : %d\n",trainercard.battleTowerStraightWins);
    printf("ContestwithFriends : %d\n",trainercard.contestsWithFriends);
    printf("Pokeblockswithfriends : %d\n",trainercard.pokeblocksWithFriends);
    printf("PokemonTrades : %d\n",trainercard.pokemonTrades);
    printf("Money : %d\n",trainercard.money);
    for(size_t i = 0;i<TRAINER_CARD_PROFILE_LENGTH;i++){
        printf("EasyChatProfile[%ld] : %d\n",i,trainercard.easyChatProfile[i]);
    }
    printf("PlayerName : %s\n",trainercard.playerName);
    printf("Version : %d\n",trainercard.version);
    printf("LinkHasAllFrontierSymbols : %d\n",trainercard.linkHasAllFrontierSymbols);
    printf("LinkPointss : %d\n",trainercard.linkPoints.frontier);
    printf("UnionroomNumber : %d\n",trainercard.unionRoomNum);
    for(size_t i = 0;i<8;i++){
        printf("Filler[%ld] : %d\n",i,trainercard.filler[i]);
    }
    printf("ShouldDrwastickers : %d\n",trainercard.shouldDrawStickers);
    printf("Unused : %d\n",trainercard.unused);
    printf("monIconTint : %d\n",trainercard.monIconTint);
    printf("Unionroomclass : %d\n",trainercard.unionRoomClass);
    for(size_t i = 0;i<TRAINER_CARD_STICKER_TYPES;i++){
        printf("Stickers : %d\n",trainercard.stickers[i]);
    }
    for(size_t i = 0;i<PARTY_SIZE;i++){
         printf("MonSpecies : %d\n",trainercard.monSpecies[i]);
    }
   
    printf("HasAllFrontierSymbols : %d\n",trainercard.hasAllFrontierSymbols);
    printf("FrontierBP : %d\n",trainercard.frontierBP);


    return trainercard;
}


int main(){

    struct TrainerCard trainercard = decodeTrainerCard(TrainerBuffer);
    return 0;
}