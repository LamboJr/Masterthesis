#ifndef LINK_HEADER
#define LINK_HEADER

#include "DataTypes.h"
#include "constant.h"


//Enum for the different Block Request Sizes. Send after the 0xCCCC control field
typedef enum{
    BLOCK_REQ_SIZE_NONE, // Identical to 200
    BLOCK_REQ_SIZE_200, //xC8   //PokemonData
    BLOCK_REQ_SIZE_100, //64	TrainderCard Data
    BLOCK_REQ_SIZE_220, //DC	Letter Buffer data
    BLOCK_REQ_SIZE_40, //28		Ribbon buffer data
	BLOCK_SIZE_60,  //3C for initial block
	BLOCK_SIZE_20
}BlockSizes;


typedef enum{
	NoRequest,
	Active,
	Pending,
	Granted
}RequestStatus;

typedef enum{
	NoData,
	CaptureData,
	WaitforThread,
	SendData
} DataHandlerStatus;

typedef enum {
	SearchState,  // Wait for connection of second device
	handshakeState, // Wait for master to confirm connection
	ExitState,		// 5FFF Exit Sate if 5FFF is send
	RequestExitState,  // Sub State before Exit state to handler Ethernet connectino in state
	DataState,		//Identify control field and corresponding data
	SendHoldKeysState,	//State for sending hold keys
	BlockRequestState,
	InitBlockState,
	TradebuttonState
}SyncState;

typedef enum{
	IdendificationState,
	BlockSendState
} DataState_State;


 //Tradehandler struct. Has different flag for tracking the Tradehandling
typedef struct TradeHandler{
	u16 PokemonTeam[MAX_TEAM_SIZE][POKEMON_BUFFER_LENGTH];
	u16 TrainerCard[TRAINER_BUFFER_SIZE];
	u16 PokemonTeamBuffer[3][100];
	u16 TeamIndex;
	u16 LetterBuffer[MAX_BUFFER_SIZE];
	u16 RibbonsBuffer[20];
	u16 ExitValue;
	RequestStatus BlockRequest;
	BlockSizes BlockRequestSize;
	DataHandlerStatus DataHandlerstatus;
	DataHandlerStatus SendLinkTypeHandler;

	u16 InitialBuffer[30];

	u16 TradeCommandBuffer[10];
	u16 InitBlockSize;
	u16 ReceivedSpotNumber;
	u16 TradeCommand;

	//Used by Monitoring Unit

	u8 ReadyCloseLink; //5FFF
	u16 LinkCMD;
	u16 LinkType;

	u16 BufferIndex;
	RequestStatus DataFieldActive ;
	DataState_State s_NextdataState;

}TradeHandler;




#endif
