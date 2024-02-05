#ifndef TRADEHANDLER_HEADER
#define TRADEHANDLER_HEADER

#include "xil_types.h"
#include "xil_printf.h"
#include "Link.h"
#include "linkcommand.h"
#include <stdlib.h>


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

s8 Ready_to_Trade(u32 FrameCounter,u32 data,u16* returnvalue,u8 SpotNumber);

void UpdateFrameCounter(u32* FrameCounterPtr);

u32 TradeHandler_func(u32 data, u32 PL_to_PS_buffer_value);

#endif
