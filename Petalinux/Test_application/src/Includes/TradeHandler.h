#ifndef TRADEHANDLER_HEADER
#define TRADEHANDLER_HEADER




#define FRAME_LENGTH 9
#define SWITCH_BITMASK 0x0000000F
#define RESETBUTTON_STATE 0x00000010
#define TRADEBUTTON_STATE 0x00000020

typedef enum {
	handshakeState,
	ExitState,
	DataState,
}Syncstate;

typedef enum{
	Tradebutton,
	BlockRequestHandler,
	Identification,
}Frametype;

typedef enum {
	//CCCC
	BlockRequest_State,
	//BBBB
	BlockInit_State,
	//8888
	BlockData_State,
	//CAFE
	Buttons_Hold_State,
}DataBlockType;

#include "../Includes/Link.h"

void GenerateEmptyFrame();
void GenerateBlockRequestResponse(BlockSizes Blocksize);

void UpdateFrameCounter(u32* FrameCounterPtr);

u32 TradeHandler(u32 data, u32 PL_to_PS_buffer_value);

u8 DetectResetButtonPress(u8 Button,char *ButtonType);
u8 DetectTradeButtonPress(u8 Button);

void GenerateBlockInit(u16 Blocksize);

void GenerateDataBlock(u16 Blocksize,u16 *Databuffer);

//void  InitTradeBuffer(u8 Spotnumber);
void *InitTradeBuffer(void* ptr);
#endif
