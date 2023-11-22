#include <stdio.h>
#include "xil_printf.h"
#include "xtime_l.h"
#include <stdlib.h>
#include <string.h>

//#include "decode_trainer_data.h"
//#include "decode_Pokemon_structure.h"


//Debuging defines. Enables print of relevant information to UART
#define DEBUG_STATES
//#define DEBUG_TIME_STAMPS
#define USE_TEST_DATA

#define GET_MASTERDATA(data) ((data)>>16)
#define GET_SLAVEDATA(data) ((data)& 0x0000FFFF)

typedef enum {
	IdleState,  // State for Phase before both gAmeboys are ready to communicate
	TransitionState,	//State for handshake between GBs. Phase if only one GB is Ready till the sny signal 8FFF is sen by master
	TrainerDataExchangeState,	//first state for exchanging information. general and trainer card information
	TradingRoomMovementState,	// State for Moving around in the trading room
	PokemonDataExchangeState,	//State for exchanging Data about the Team Pokemon after taking Place in the chair
	TradingState,

} s_SystemState;

typedef enum{
	PaddingState, //0000 no useful data or control sequence is sent
	syncState, //B9A0 used for sync the two gameboys at the beginning or certain sequences
	ControlStateCheck, //if master sends 2222 in control field
	DataIdentification,	//CCCC The follwing field showas which kind of data follows in the 8888 Field
	DataFieldSize,		//BBBB Indicates the number of bytes of the upcoming 8888 Data Field
	FlowControl8FFF,	//8FFF ,terminates the b9A0 sync sequence and continues with the program
	FlowControl2FFE,	//2FFE also terminates some internal controlflow state
	FlowControl5FFF,	//5FFF terminates a sequence and signals a following B9A0 seqeunce. //TODO Check for comm termination
	MovementIndication, //CAFE indicates Movement in the trading room
	DataFieldIndicator,			//8888 indicates a Datafield is following
} s_ControlFieldState;

s_ControlFieldState s_NextCFS_Master;
s_ControlFieldState s_NextCFS_Slave;

s_ControlFieldState decodeControlField(u16 data);

void appendString(char **Outputstring,char *text);
void printTime(void);
int checkFrameDuration(void);
void printDataFrame(u32 dump,u32 data,char* Outputstring);
void identifyData(char** Outputstring,u16 Value);

struct Flags{
	u8 Master5FFF;
	u8 Slave5FFF;
	u16 ConstrolStateValue;
	u16 DataIdentifyValue;
	u16 DataFieldSizeValueMaster;
	u16 DataFieldSizeValueSlave;
	u16 DataIndexMaster;
	u16 DataIndexSlave;
	u16 TeamIndexMaster;
	u16 TeamIndexSlave;

};

struct Flags flags = {0,0,0,0,0,0,0,0,0,0};

u16 PokemonTeamMaster[6][50];
u16 PokemonTeamSlave[6][50];



u32 MasterDataframeCount = 0; //variable for counting the number of relevant  complete 9er frames !!!
u32 SlaveDataframeCount = 0; //variable for counting the number of relevant  complete 9er frames !!!

u32 frameCount = 1;		//count variable for formating the monitoring output and also used for interpreting data
//u32 data;			//variable for storing red in data

u32 previousData = 0;
s_SystemState s_NextState = TransitionState;	//FSM state vaiable



XTime tExecution,tStart= 0;
double frameDuration;

void N_updateFSM(u32 data,u32 dump){
	char* Outputstring = "";

	//if(checkFrameDuration()==1){return;}

	switch(s_NextState){
	case IdleState:{
		if ( (dump & 0x1) == 1){
			xil_printf("%04x:%04x  ",GET_MASTERDATA(data),GET_SLAVEDATA(data));
			printTime();
			xil_printf("\n");
		}
		if(decodeControlField(GET_MASTERDATA(data)) == FlowControl8FFF){
			s_NextState = TradingState;
			frameCount = 0;
			flags.Master5FFF = 0;flags.Slave5FFF =0;
			flags.ConstrolStateValue =0;flags.DataIdentifyValue =0;flags.DataFieldSizeValueMaster =0;
			flags. DataFieldSizeValueSlave=0;flags.DataIndexMaster=0;flags.DataIndexSlave=0;
			flags.TeamIndexMaster=0;flags.TeamIndexSlave = 0;
			xil_printf("Entering Trading State\n");
		}
	}break;
	case TradingState:{
		switch(frameCount){
				case 0:{
					if ( GET_MASTERDATA(data) != GET_SLAVEDATA(data) ){
						appendString(&Outputstring, "Checksum not matching\n");
					}
				}break;//case0 framecount


				case 1:{
					//-----------------
					//Check control frames which are sent. F.E 2222,BBBB,CCCC,5FFF,2FFE,CAFE,
					s_NextCFS_Master = decodeControlField(GET_MASTERDATA(data));
					s_NextCFS_Slave = decodeControlField(GET_SLAVEDATA(data));
					switch(s_NextCFS_Master){
						case syncState:{
							appendString(&Outputstring, "\nHandshake State");
							frameCount = 8;
							s_NextState = IdleState;
						}break;
						case FlowControl8FFF:{
							appendString(&Outputstring, "Entering TradeInformationExchange State\n");
							frameCount = 8;
						}
						case ControlStateCheck:{
							appendString(&Outputstring, "Control Field Check\n");

						}break;
						case FlowControl5FFF:{
							flags.Master5FFF = 1;
							if((flags.Master5FFF && flags.Slave5FFF) == 1){
								s_NextState = TransitionState;
								appendString(&Outputstring, "Enter Transition State\n");
								flags.Master5FFF = 0;
								flags.Slave5FFF = 0;
							}
						}break;
						case DataIdentification:{//CCCC
							appendString(&Outputstring, "Data Identification\n");
						}break;
						case DataFieldSize:{//BBBB
							appendString(&Outputstring, "Data Field Size\n");
							flags.DataIndexMaster=0;

						}break;
						case DataFieldIndicator:{//8888
							//implement
							appendString(&Outputstring, "Data Field : \n");
						}break;
						case MovementIndication:{}break;
						case PaddingState:{
							flags.DataIndexMaster = 0;
						}break;
						default:break;
					}//end switch
					switch(s_NextCFS_Slave){
					case FlowControl5FFF:{
						flags.Slave5FFF = 1;
						if((flags.Master5FFF && flags.Slave5FFF) == 1){
							s_NextState = TransitionState;
							appendString(&Outputstring, "Enter Transition State\n");
							flags.Slave5FFF = 0;
							flags.Master5FFF = 0;
						}
					}break;
					case DataFieldSize:{
						if (s_NextCFS_Master != DataFieldSize){
							appendString(&Outputstring, "Data Field Size\n");
						}
						flags.DataIndexSlave=0;

					}break;
					case DataFieldIndicator:{
						//implement
						if(s_NextCFS_Master != DataFieldIndicator){
							appendString(&Outputstring, "Data Field :\n");
						}
					}break;
					case PaddingState:{
						flags.DataIndexSlave= 0;
					}break;
					default:break;
					}
				}break;//case1 framecount



				case 2:{
					switch(s_NextCFS_Master){
					case DataIdentification:{
						flags.DataIdentifyValue = GET_MASTERDATA(data);
						//xil_printf("DEbug");
						identifyData(&Outputstring, flags.DataIdentifyValue);
					}break;
					case ControlStateCheck:{
						flags.ConstrolStateValue = GET_MASTERDATA(data);
					}
					case DataFieldSize:{
						flags.DataFieldSizeValueMaster = GET_MASTERDATA(data);
					}break;
					case DataFieldIndicator:{
						//Write data into buffer
/*						if ( flags.DataIdentifyValue == 1){ // pokemon Data
							if (flags.DataIndexMaster < (flags.DataFieldSizeValueMaster/2)){

								PokemonTeamMaster[flags.TeamIndexMaster][flags.DataIndexMaster] = GET_MASTERDATA(data);
								flags.DataIndexMaster++;
							}

						}else if(flags.DataIdentifyValue ==2 ){ //Trainer Data

						}*/
					}break;

					default:break;
					}

					switch(s_NextCFS_Slave){
					case DataFieldSize:{
						flags.DataFieldSizeValueSlave = GET_SLAVEDATA(data);
					}break;
					case DataFieldIndicator:{
						//rite data into buffer
/*						if ( flags.DataIdentifyValue == 1){ // pokemon Data						if ( flags.DataIdentifyValue == 1){ // pokemon Data
							if (flags.DataIndexSlave < (flags.DataFieldSizeValueSlave/2)){

								PokemonTeamSlave[flags.TeamIndexSlave][flags.DataIndexSlave] = GET_SLAVEDATA(data);
								flags.DataIndexSlave++;
							}

						}else if(flags.DataIdentifyValue ==2 ){ //Trainer Data

						}
							if (flags.DataIndexSlave < (flags.DataFieldSizeValueSlave/2)){

								PokemonTeamSlave[flags.TeamIndexSlave][flags.DataIndexSlave] = GET_SLAVEDATA(data);
								flags.DataIndexSlave++;
							}

						}else if(flags.DataIdentifyValue ==2 ){ //Trainer Data

						}*/
					}break;
					default:break;
					}



				}break;//case2 framecount
				case 3 ... 8:{
					if (s_NextCFS_Master == DataFieldIndicator){
						if (flags.DataIndexMaster < (flags.DataFieldSizeValueMaster/2)){

							flags.DataIndexMaster++;
						}
					}
					if ( s_NextCFS_Slave == DataFieldIndicator){
						if (flags.DataIndexSlave <(flags.DataFieldSizeValueSlave/2)){

							flags.DataIndexSlave++;
						}
					}
				}break;
				default:break;
				}

				printDataFrame(dump,data,Outputstring);
	}break;
	case TransitionState:{
		if(decodeControlField(GET_MASTERDATA(data)) == syncState){
			s_NextState = IdleState;
			appendString(&Outputstring, "Entering Sync State\n");
			frameCount = 0;
		}
		xil_printf("In Transition ");
		printDataFrame(dump,data,Outputstring);
	}break;

	default:break;
	}//end switch NexState Variable


	return;
}

s_ControlFieldState decodeControlField(u16 data){
	switch(data){
	case (0xB9A0):{return syncState;}break;
	case (0x8FFF):{return FlowControl8FFF;}break;
	case (0x2222):{return ControlStateCheck;}break;
	case (0xCCCC):{return DataIdentification;}break;
	case (0xBBBB):{return DataFieldSize;}break;
	case (0xCAFE):{return MovementIndication;}break;
	case (0x2FFE):{return FlowControl2FFE;}break;
	case (0x5FFF):{return FlowControl5FFF;}break;
	case (0x8888):{return DataFieldIndicator;}break;
	default:{return PaddingState;}break;
	}//end switch
}

void appendString(char **Outputstring,char *text){
	char* blankspace = "                    \t\t";
	char* tempString = *Outputstring;
	*Outputstring = malloc(strlen(*Outputstring)+strlen(text)+(strlen(blankspace)* frameCount)+1);
	strcpy(*Outputstring,tempString);
	strcat(*Outputstring,text);
    for(int i = 0;i<frameCount;i++){
        strcat(*Outputstring,blankspace);
    }

	return;
}

void printTime(void){
#ifdef DEBUG_TIME_STAMPS
			XTime_GetTime(&tExecution);
			frameDuration = ((double)(tExecution -tStart) /(double)COUNTS_PER_SECOND);
			tStart = tExecution;
			printf("Frame duration : %f seconds",frameDuration);
#endif
}

int checkFrameDuration(void){
#ifndef USE_TEST_DATA

	if(frameDuration <= 0.01 || frameDuration >= 0.1){
		//s_NextState = IdleState;
		return 1;
		//Gameboys are turned off. Reset the system for new transaction
	}
	else{return 0;}
#else
	return 0;
#endif
}

void printDataFrame(u32 dump,u32 data,char* Outputstring){
	if ( (dump & 0x1) == 1){
#ifdef DEBUG_STATES
		xil_printf("%s",Outputstring);
#endif
		xil_printf("%04x:%04x \t",GET_MASTERDATA(data),GET_SLAVEDATA(data));
	}
	if ((frameCount == 1) || (frameCount == 0)){xil_printf("|\t");}
	if ((frameCount % 8 == 0) && (frameCount != 0)){
		if ( (dump & 0x1) == 1){
			printTime();
			xil_printf("\n");
		}
		frameCount = 0;
	}
	else{
		frameCount++;
	}
	return;
}
void identifyData(char** Outputstring,u16 Value){
	if(Value == 1){
		appendString(Outputstring, "Pokemondata\n");
	}
	else if( Value == 2){
		appendString(Outputstring, "Trainer Data\n");
	}
	else{
		appendString(Outputstring, "Data not yet identified\n");
	}
	return;
}

