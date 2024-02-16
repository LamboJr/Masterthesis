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

struct TradeHandler{
	u16 PokemonTeam[MAX_TEAM_SIZE][POKEMON_BUFFER_LENGTH];
	u16 TrainerCard[TRAINER_BUFFER_SIZE];
	u16 TeamIndex;
	u16 DataIndex;
	u8 BlockRequest;
	u16 BlockRequestSize;
	u8 InitBlock;
	u16 InitBlockSize;
	u8 ContBlock;
	u8 ReadyCloseLink; //5FFF
	u16 LinkCMD;
	u16 LinkType;
	u16 TempBuffer[MAX_BUFFER_SIZE];
	u16 PressedButtons;

};


#endif
