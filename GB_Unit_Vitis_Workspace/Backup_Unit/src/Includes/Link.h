#ifndef LINK_HEADER
#define LINK_HEADER

#include "DataTypes.h"
#include "constant.h"

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
	u8 InitBlock;
	u8 ContBlock;
	u8 ReadyCloseLink; //5FFF
	u16 LinkCMD;
	u16 LinkType;
	u16 TempBuffer[MAX_BUFFER_SIZE];
	u16 PressedButtons;
	u16 DataIndex;
}TradeHandler;

typedef enum{
	NoThread,
	Finished,
	Running
}ThreadStatus;
typedef enum{
	SameID,
	DifferentID
}IDMatching;
typedef enum{
	Searching,
	Handshake,
	ExchangePokemonData,
	ExchangeTrainerData,
	ExchangeLetterData,
	ExchangeRibbonData,
	Exit_0, //Exitvalue = 0
	Exit_20, // ExitValue 20;
	Exit_C,
	SendInitialBlock,
	SendTradeCommand

}ID_Type;

typedef struct ThreadData {
   u8 TeamIndex;
   ID_Type Connect_ID;
   IDMatching MatchingID;
   u16 ArgBufferSize;
   u16 *ArgBufferPtr;
   u16 ArgBufferPtr2[50];
   char *Tablename;
   u8 result;

} ThreadData;

#endif