#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"

#define TrainerNameLength 8
/*
struct TrainerCard {
	u8 version;
	u16 TrainerID;
	u16 SecretID;
	char TrainerName[TrainerNameLength];
	u8 CharacterSHown;
	u8 Stars;
	u8 Gender;
	u8 CaughtAllHoen;
	u8 GotPokedex;
	u16 HOF_Hours;
	u16 HOF_Minutes;
	u16 HOF_Seconds;
	u16 CaughtPokemon;
	u16 HoursPlayed;
	u16 MinutesPlayed;
	u16 LinkBattlesWon;
	u16 LinkBattlesLost;
	u16 BattleTowerWins;
	u16 BattleTowerStraightWins;
	u16 ContestFriends;
	u16 PokeBlocks;
	u16 PokemonTrades;
	u32 Money;
	u16 EasyChatProfile[4];
	u16 HasAllFrontierSymbols;
	u32 BC_FP;
	u32 UnionRoomNum;
	u16 BerriesPicked;
	u32 JumpsInRow;
	u8 DrawStickers;
	u8 HasAllMons;
	u8 MonIconTint;
	u8 UnionRoomClass;
	u8 StickerArray[4];
};


void print_trainer_data(u32 *trainerbuffer,struct TrainerCard Trainer_Card){
	u32 i;
	for (i = 0; i < 100; i++) {
		trainerbuffer[i] = trainerbuffer[i] >> 16;
	}
	//version
	Trainer_Card.version = trainerbuffer[8] & 0x00FF;
	//trainerbuffer[9] = 8000 not relevant
	Trainer_Card.TrainerID = trainerbuffer[10];
	Trainer_Card.SecretID = trainerbuffer[11];
	Trainer_Card.TrainerName[0] = trainerbuffer[12] & 0x00FF;
	Trainer_Card.TrainerName[1] = trainerbuffer[12] >> 8;
	Trainer_Card.TrainerName[2] = trainerbuffer[13] & 0x00FF;
	Trainer_Card.TrainerName[3] = trainerbuffer[13] >> 8;
	Trainer_Card.TrainerName[4] = trainerbuffer[14] & 0x00FF;
	Trainer_Card.TrainerName[5] = trainerbuffer[14] >> 8;
	Trainer_Card.TrainerName[6] = trainerbuffer[15] & 0x00FF;
	Trainer_Card.TrainerName[7] = trainerbuffer[15] >> 8;
	//trainerbuffer[16] = 0011
	Trainer_Card.CharacterSHown = trainerbuffer[17] >> 8;
	//1133 trainerbuffer[18]
	//trainerbuffer[19] = 0000
	//trainerbuffer[20] = 0000
	Trainer_Card.Gender = trainerbuffer[21] >> 8;
	Trainer_Card.Stars = trainerbuffer[21] & 0x00FF;





	//xil_printf("\nVersion : %d\n",Trainer_Card.version);

}
