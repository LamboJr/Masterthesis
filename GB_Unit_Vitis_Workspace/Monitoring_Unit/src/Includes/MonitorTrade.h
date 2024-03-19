#ifndef MONITORTRADE_HEADER
#define MONITORTRADE_HEADER

#include "Link.h"
//Debuging defines. Enables print of relevant information to UART
//#define DEBUG_STATES
//#define DEBUG_TIME_STAMPS
//#define USE_TEST_DATA

#define GET_MASTERDATA(data) ((data)>>16)
#define GET_SLAVEDATA(data) ((data)& 0x0000FFFF)

#define MASTER 0
#define SLAVE 1

#define CASEPRINT(LinkCMD) case (LinkCMD):{ printf("Command :"#LinkCMD"\n");}break;
#define PRINTMS(MSC) if((MSC) == MASTER){printf("Master ");}else{printf("SLAVE ");}



#define FRAME_LENGTH 9




void TradehandlerReset(TradeHandler* sTradeHandler);

void MonitorHandler(u32 data,u32 dump,FILE *fp);

void printBlankspace(char* text);

void updateBuffer(u8 MSC,u32 data);

void PrintNewline(FILE *fp);
void UpdateFrameCounter();
void PrintFrame(u32 data, FILE *fp);


RequestStatus MonitorBlockRequestAnalyser(u32 Framecounter,u16 data, TradeHandler* s_Tradehandler);



#endif



