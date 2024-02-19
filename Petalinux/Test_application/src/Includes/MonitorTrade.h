#ifndef MONITORTRADE_HEADER
#define MONITORTRADE_HEADER


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

typedef enum {
	IdleState,  // State for Phase before both gAmeboys are ready to communicate
	TransitionState,	//State for handshake between GBs. Phase if only one GB is Ready till the sny signal 8FFF is sen by master
	TradingState,

} s_SystemState;

void MonitorHandler(u32 data,u32 dump,FILE *fp);

void printDataFrame(u32 dump,u32 data,FILE *fp);
void printBlankspace(char* text);

void updateBuffer(u8 MSC,u32 data);


#endif



