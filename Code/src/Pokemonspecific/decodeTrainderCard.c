#include "../Includes/DataTypes.h"
#include "../Includes/Link.h"
#include "../Includes/structs.h"

#include <stdio.h>
#include "../Includes/functions.h"

#include "../Includes/decodeTrainderCard.h"

/*This function is used to decode the TrainerCard Buffer into the corresponding TrainerCard struct
 * It takes a Pointer to an uint16_t buffer as input and prints the individual field of the trainer card struct
 */

void decodeTrainerCard(uint16_t* TrainerBuffer){
    struct TrainerCard trainercard;
    uint16_t* TrainerPtr = (uint16_t* )&trainercard;

        for (size_t i = 0;i <TRAINER_BUFFER_SIZE;i++){
        *TrainerPtr = TrainerBuffer[i];
        //printf("Trainerstruct[%ld] addr : %p Value = %04x\n",i,TrainerPtr,*TrainerPtr);
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
        printf("EasyChatProfile[%d] : %d\n",i,trainercard.easyChatProfile[i]);
    }
    printf("PlayerName : %s\n",trainercard.playerName);
    printf("Version : %d\n",trainercard.version);
    printf("LinkHasAllFrontierSymbols : %d\n",trainercard.linkHasAllFrontierSymbols);
    printf("LinkPointss : %d\n",trainercard.linkPoints.frontier);
    printf("UnionroomNumber : %d\n",trainercard.unionRoomNum);
    for(size_t i = 0;i<8;i++){
        printf("Filler[%d] : %d\n",i,trainercard.filler[i]);
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


    return;
}
