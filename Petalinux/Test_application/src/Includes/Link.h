#ifndef LINK_HEADER
#define LINK_HEADER
/*
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#include "linkcommand.h"
#include "structs.h"

#include "functions.h"
#include "items.h"
#include "Pokemonnames.h"
#include "moves.h"
#include "decode_Pokemon_structure.h"
#include "decodeTrainderCard.h"*/
#include "DataTypes.h"
#include "constant.h"

typedef enum{
    BLOCK_REQ_SIZE_NONE, // Identical to 200
    BLOCK_REQ_SIZE_200, //xC8
    BLOCK_REQ_SIZE_100, //64
    BLOCK_REQ_SIZE_220, //DC
    BLOCK_REQ_SIZE_40 //28
}BlockSizes;
typedef enum{
	NoRequest,
	Active
}RequestStatus;

typedef enum{
	NoData,
	CaptureData,
	WaitforThread,
	SendData
} DataHandlerStatus;

struct TradeHandler{
	u16 PokemonTeam[MAX_TEAM_SIZE][POKEMON_BUFFER_LENGTH];
	u16 TrainerCard[TRAINER_BUFFER_SIZE];
	u16 TeamIndex;
	u16 DataIndex;
	RequestStatus BlockRequest;
	BlockSizes BlockRequestSize;
	DataHandlerStatus DataHandlerstatus;
	u8 InitBlock;
	u16 InitBlockSize;
	u8 ContBlock;
	u8 ReadyCloseLink; //5FFF
	u16 LinkCMD;
	u16 LinkType;
	u16 TempBuffer[MAX_BUFFER_SIZE];
	u16 PressedButtons;

};

typedef enum{
	NoThread,
	Finished,
	Running

}ThreadStatus;



#endif
