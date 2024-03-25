#include "decodeTrainderCard.h"

void decodeTrainerCard(uint16_t* TrainerBuffer){
    struct TrainerCard trainercard;
    uint16_t* TrainerPtr = (uint16_t* )&trainercard;

        for (size_t i = 0;i <TRAINER_BUFFER_SIZE;i++){
        *TrainerPtr = TrainerBuffer[i];
        //xil_printf("Trainerstruct[%ld] addr : %p Value = %04x\n",i,TrainerPtr,*TrainerPtr);
        TrainerPtr++;
    }
    for(size_t i = 0;i<TRAINER_NAME_LENGTH+1;i++){
        trainercard.playerName[i] = char_conv(trainercard.playerName[i],LANGUAGE_GERMAN);
    }
    xil_printf("Gender : %d\n",trainercard.gender);
    xil_printf("Stars : %d\n",trainercard.stars);
    xil_printf("HasPokedex : %d\n",trainercard.hasPokedex);
    xil_printf("CaughtAllHoen : %d\n",trainercard.caughtAllHoenn);
    xil_printf("hasAllPaintings : %d\n",trainercard.hasAllPaintings);
    xil_printf("HOFDebutHours : %d\n",trainercard.hofDebutHours);
    xil_printf("HOFDebutMinutes : %d\n",trainercard.hofDebutMinutes);
    xil_printf("HOFDebutSeconds : %d\n",trainercard.hofDebutSeconds);
    xil_printf("caughtMonscount : %d\n",trainercard.caughtMonsCount);
    xil_printf("trainerId : %d\n",trainercard.trainerId);
    xil_printf("PlayTimeHours : %d\n",trainercard.playTimeHours);
    xil_printf("PlayTimeMinutes : %d\n",trainercard.playTimeMinutes);
    xil_printf("LinkBattleWins : %d\n",trainercard.linkBattleWins);
    xil_printf("LinkBattleLosses : %d\n",trainercard.linkBattleLosses);
    xil_printf("BattleTowerWins : %d\n",trainercard.battleTowerWins);
    xil_printf("BattleTowerStarightWins : %d\n",trainercard.battleTowerStraightWins);
    xil_printf("ContestwithFriends : %d\n",trainercard.contestsWithFriends);
    xil_printf("Pokeblockswithfriends : %d\n",trainercard.pokeblocksWithFriends);
    xil_printf("PokemonTrades : %d\n",trainercard.pokemonTrades);
    xil_printf("Money : %d\n",trainercard.money);
    for(size_t i = 0;i<TRAINER_CARD_PROFILE_LENGTH;i++){
        xil_printf("EasyChatProfile[%ld] : %d\n",i,trainercard.easyChatProfile[i]);
    }
    xil_printf("PlayerName : %s\n",trainercard.playerName);
    xil_printf("Version : %d\n",trainercard.version);
    xil_printf("LinkHasAllFrontierSymbols : %d\n",trainercard.linkHasAllFrontierSymbols);
    xil_printf("LinkPointss : %d\n",trainercard.linkPoints.frontier);
    xil_printf("UnionroomNumber : %d\n",trainercard.unionRoomNum);
    for(size_t i = 0;i<8;i++){
        xil_printf("Filler[%ld] : %d\n",i,trainercard.filler[i]);
    }
    xil_printf("ShouldDrwastickers : %d\n",trainercard.shouldDrawStickers);
    xil_printf("Unused : %d\n",trainercard.unused);
    xil_printf("monIconTint : %d\n",trainercard.monIconTint);
    xil_printf("Unionroomclass : %d\n",trainercard.unionRoomClass);
    for(size_t i = 0;i<TRAINER_CARD_STICKER_TYPES;i++){
        xil_printf("Stickers : %d\n",trainercard.stickers[i]);
    }
    for(size_t i = 0;i<PARTY_SIZE;i++){
         xil_printf("MonSpecies : %d\n",trainercard.monSpecies[i]);
    }

    xil_printf("HasAllFrontierSymbols : %d\n",trainercard.hasAllFrontierSymbols);
    xil_printf("FrontierBP : %d\n",trainercard.frontierBP);


    return;
}

