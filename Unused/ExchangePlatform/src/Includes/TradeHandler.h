#ifndef TRADEHANDLER_HEADER
#define TRADEHANDLER_HEADER




#define FRAME_LENGTH 9
#define SWITCH_BITMASK 0x0000000F
#define RESETBUTTON_STATE 0x00000010
#define TRADEBUTTON_STATE 0x00000020

typedef enum {
	SearchState,  // Wait for connection of second device
	handshakeState, // Wait for master to confirm connection
	ExitState,		// 5FFF Exit Sate if 5FFF is send
	RequestExitState,  // Sub State before Exit state to handler Ethernet connectino in state
	DataState,		//Identify control field and corresponding data
	SendHoldKeysState,	//State for sending hold keys
	BlockRequestHandler,
	InitBlockState,
	TradebuttonState
}Syncstate;

#include "../Includes/Link.h"

void GenerateEmptyFrame();
void GenerateBlockRequestResponse(BlockSizes Blocksize);

void UpdateFrameCounter(u32* FrameCounterPtr);

u32 MainTradeHandler(u32 data, u32 PL_to_PS_buffer_value);

u16 GetBlockSize(BlockSizes BlockSize);
u8 DetectResetButtonPress(u8 Button,char *ButtonType);
u8 DetectTradeButtonPress(u8 Button);

void GenerateBlockInit(u16 Blocksize);
void GenerateEmptyFrame();
void GenerateDataBlock(u16 Blocksize,u16 *Databuffer);

RequestStatus InitBlockAnalyser(u32 Framecounter, u16 data);

//void  InitTradeBuffer(u8 Spotnumber);
void *InitTradeBuffer(void* ptr);
void GenerateTradeCommandBlock(u16 TradeCommand,u16 TradeCommandData);
RequestStatus BlockRequestAnalyser(u32 Framecounter,u16 data);

RequestStatus CreateExitThread(u32 Framecounter);

#endif
