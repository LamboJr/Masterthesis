#include "Includes/DataTypes.h"
#include <stdio.h>
#include "Includes/Link.h"
#include "Includes/linkcommand.h"
#include <stdlib.h>




#include "Includes/Pokemonbuffer.h"

#include "Includes/Ringbuffer.h"


#include <pthread.h>
#include "Includes/ConCat.h"
#include "Includes/TradeHandler.h"
#include "Includes/TCP_Server.h"


u16 ReceivedTeam[6][50];

u16 ReceiveBuffer[110];

extern struct BufferType s_DataBuffer;
// DEclare a buffer structure for a Buffer with Data to be send to Ringbuffer of HW
extern struct BufferType s_ControlBuffer;

TradeHandler s_TradeHandlerMaster;

ThreadStatus Threadstatus = NoThread;

ThreadData Threaddata;
//pthread_attr_t attr;
pthread_t thread1;
int rc;


u32 MainTradeHandler(u32 data,u32 PL_to_PS_buffer_value){

	static Syncstate s_syncstate = SearchState;

	static u32 FrameCounter = 2;

	u16 returnvalue = (u16)data ;
	//static pthread_t thread1;
	//int rc;

	//static struct TradeHandler s_TradeHandlerMaster;
	static __attribute__ ((unused)) TradeHandler s_TradeHandlerSlave;

	static RequestStatus SendTradeDataStatus=  NoRequest;

	static u8 __attribute__ ((unused)) Switches_State;
	static u8 __attribute__ ((unused)) Resetbutton_State;
	static u8 __attribute__ ((unused)) Tradebutton_State ;


	Switches_State = (u8)(PL_to_PS_buffer_value & SWITCH_BITMASK);
	static u16 SpotNumber;

	Resetbutton_State = (u8)((PL_to_PS_buffer_value & RESETBUTTON_STATE) >> 4);
	Tradebutton_State = (u8)((PL_to_PS_buffer_value & TRADEBUTTON_STATE) >> 5);
	//Reset
	if (DetectResetButtonPress(Resetbutton_State,"reset") == 1){
		s_syncstate = SearchState;
		ResetBuffer(&s_DataBuffer);
		ResetBuffer(&s_ControlBuffer);
		return 0;
	}

	if ((DetectTradeButtonPress(Tradebutton_State) == 1) && (SendTradeDataStatus == NoRequest)){
		SpotNumber = Switches_State % 6;

		InitTradeBuffer((void *) &SpotNumber);
		//rc = pthread_create( &thread1, NULL, InitTradeBuffer, (void *)Spotptr);
		SendTradeDataStatus = Active;
	}
    if (FrameCounter == FRAME_LENGTH){
		FrameCounter = 1;
	}else{
		FrameCounter++;
	}
    //printf("FC :%d ",FrameCounter);
	switch(s_syncstate){

	//if(s_syncstate == SearchState){
	case SearchState : {
		FrameCounter = FRAME_LENGTH;
		returnvalue = 0xFFF8;
		//Send ID and Connected Signal over TCP Thread
		if( data == 0xB9A0){
			if( Threadstatus == NoThread){
				Threadstatus = Running;
				Threaddata.Connect_ID = Searching;
				rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
			}
			else if (Threadstatus == Finished){
				if(Threaddata.MatchingID == SameID){
					s_syncstate =handshakeState;
					printf("Enter HAndshake State\n");
				}
				Threadstatus = NoThread;
			}
		}else{
			returnvalue = 0;
		}
	}break;
	// Calculate the frame counter depending on the  sysnc state
	case handshakeState :{
	//else if (s_syncstate == handshakeState){
		FrameCounter = FRAME_LENGTH;
		returnvalue = data;
		s_TradeHandlerMaster.DataHandlerstatus = NoData;
		s_TradeHandlerMaster.ExitValue = 0;
		s_TradeHandlerMaster.TeamIndex = 0;
		//If data is 8FFF the communication switches to data phase
		if (data == MASTER_HANDSHAKE){
			s_syncstate = DataState;
			returnvalue = 0xB9A0;
			printf("Enter Data State\n");
		}else if( data == 0xB9A0){
			s_syncstate = handshakeState;
		}else{
			s_syncstate = SearchState;
		}
	}break;
	//State to handle phase for

	case RequestExitState : {
		if(FrameCounter == 1){returnvalue = data;}
		else{
			switch(s_TradeHandlerMaster.DataHandlerstatus){
			case NoData : {printf("Should not be here \n");returnvalue = 0;}break;
			case CaptureData :{
				returnvalue = 0;
				if(CreateExitThread(FrameCounter) == Granted){
					s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
					printf("Wait for Thread to finish\n");
				}else{
					s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
				}

			}break;
			case WaitforThread :{
				returnvalue = 0;
				if( Threadstatus == Finished){
					if(FrameCounter == 9){
						Threadstatus = NoThread;
						WriteBuffer(0x5FFF, &s_ControlBuffer);
						WriteBuffer(s_TradeHandlerMaster.ExitValue, &s_DataBuffer);
						s_TradeHandlerMaster.DataHandlerstatus = SendData;
					}
				}else{
					s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
			}break;
			case SendData :{
				if(FrameCounter == 2){
					//printf("Send 5FFF mit FC : %d\n",FrameCounter);
					returnvalue = ReadBuffer(&s_ControlBuffer);
					return returnvalue;

				}else if( FrameCounter == 3){
					returnvalue = ReadBuffer(&s_DataBuffer);
					if(Threaddata.MatchingID == SameID){
						//printf("Exit Value = %04x\n",returnvalue);
					}
				}else if(FrameCounter == 9){
					returnvalue = 0;
					Threadstatus = NoThread;
					s_syncstate = ExitState;
					s_TradeHandlerMaster.DataHandlerstatus = NoData;
				}else{
					returnvalue = 0;
				}
			}break;
			}
			default : {}break;
			}//end switch dataHandlerstatus
		}

	}break;

	case ExitState : {
		returnvalue = data;
		if (data == 0xB9A0){
			printf("In exitstate enter handshake\n");
			s_syncstate = handshakeState;
		}
	}break;
	case BlockRequestHandler : {

		if( FrameCounter == 1){returnvalue = data;}
		else{
				switch(s_TradeHandlerMaster.DataHandlerstatus){
					case NoData :{ returnvalue = 0;}break;
						printf("Sollte hier nicht sein\n\n");
					case CaptureData : {
						returnvalue = 0;
						if(BlockRequestAnalyser(FrameCounter, data) == Granted){
							s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
							printf("Wait for Thread to finish\n");
						}else{
							s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
						}
					}break;

					case WaitforThread : {
						returnvalue = 0;
						if( Threadstatus == Finished){
							if(FrameCounter == 9){
								Threadstatus = NoThread;
								s_TradeHandlerMaster.DataHandlerstatus = SendData;
								GenerateBlockRequestResponse(s_TradeHandlerMaster.BlockRequestSize);
							}
						}else{
							s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
						}
					}break;

					case SendData : {

						if(FrameCounter == 2){
							returnvalue = ReadBuffer(&s_ControlBuffer);
							//printf("RV : %04x  ",returnvalue);
						}else{
							returnvalue = ReadBuffer(&s_DataBuffer);
							//printf("RV : %04x  ",returnvalue);
							if (FrameCounter == 9){
								//printf("\n");
								if (BufferEmpty(&s_ControlBuffer) == 1){
									s_TradeHandlerMaster.BlockRequest = NoRequest;
									s_syncstate = DataState;
									s_TradeHandlerMaster.DataHandlerstatus = NoData;
									printf("Successful Send Buffer\n");
								}
							}
						}

					}break;
					default : {}break;
					}//end switch
			}
		//return returnvalue;
	}break;

	case TradebuttonState : {

		if(FrameCounter == 1){
			returnvalue = data;
		}
		else if(FrameCounter == 2){
			returnvalue = ReadBuffer(&s_ControlBuffer);
		}else{
			returnvalue = ReadBuffer(&s_DataBuffer);
			if (FrameCounter == 9){
				if (BufferEmpty(&s_ControlBuffer) == 1){
					SendTradeDataStatus = NoRequest;
					s_syncstate = DataState;
					printf("Successful Send Trade Choice\n");
				}
			}
		}
		return returnvalue;
	}break;

	case InitBlockState : {
			if (FrameCounter ==1 ){
				returnvalue = data;
			}else{
				returnvalue = 0;
				switch(s_TradeHandlerMaster.DataHandlerstatus){
				case CaptureData :{
					if(InitBlockAnalyser(FrameCounter, data) == Granted){
						s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;

					}else{
						s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
					}
				}break;
				case WaitforThread :{
					returnvalue = 0;
						if(FrameCounter == 9){
							s_TradeHandlerMaster.DataHandlerstatus = SendData;
							s_TradeHandlerMaster.TradeCommand = s_TradeHandlerMaster.TradeCommandBuffer[0];
							switch (s_TradeHandlerMaster.TradeCommand){
							case LINKCMD_SET_MONS_TO_TRADE : {
								s_TradeHandlerMaster.ReceivedSpotNumber = s_TradeHandlerMaster.TradeCommandBuffer[1];

							}break;//0xDDDD
							case LINKCMD_START_TRADE :{}break; // 0xCCDD
							case LINKCMD_PLAYER_CANCEL_TRADE :{}break; //DDEE

							case LINKCMD_CONFIRM_FINISH_TRADE : {

							}break;//DCBA
							default : {
								GenerateTradeCommandBlock(s_TradeHandlerMaster.TradeCommand, s_TradeHandlerMaster.TradeCommandBuffer[1]);
								printf("Send Init Block\n");
							}break;
							}//end switch

						}else{
							s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
					}
				}break;

				case SendData : {
					if(FrameCounter == 2){
						returnvalue = ReadBuffer(&s_ControlBuffer);
						//printf("RV C : %04x ",returnvalue);
					}else{
						returnvalue = ReadBuffer(&s_DataBuffer);
						//printf("RV : %04x ",returnvalue);
						if (FrameCounter == 9){
							//printf("\n");
							if (BufferEmpty(&s_ControlBuffer) == 1){
								s_syncstate = DataState;
								s_TradeHandlerMaster.DataHandlerstatus = NoData;
								printf("Successful Send Buffer\n");
							}
						}
					}
				}break;
				default : {

				}break;
				}//end switch
				//s_TradeHandlerMaster.InitBlockSize
			}

		}break;

	case DataState : {

		switch(FrameCounter){
		case 1: {
			if ((SendTradeDataStatus == Active)){
				s_syncstate = TradebuttonState;
			}
			returnvalue = data;
		}break;
		case 2: {
			s_TradeHandlerMaster.LinkCMD = data;
					switch(data){
						case 0xCCCC : { //REquest Data Block
							s_TradeHandlerMaster.BlockRequest = Active;
							s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
							printf("New BlockRequest\n");
							returnvalue = 0;
						}break;
						case 0xBBBB : { //Init Block
							s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
							returnvalue = 0;
//							s_TradeHandlerMaster.InitBlock = Active;
//							returnvalue = data;
						}break;
						case 0x8888 : { // Cont Data Block
							returnvalue = data;
						}break;
						case 0x2222 : { // Link CMD Type
							returnvalue = 0;
							s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
							//s_TradeHandlerMaster.SendLinkTypeHandler = CaptureData;
						}break;
						case 0xCAFE : { // Hold buttons
							returnvalue = 0xCAFE;
						}break;
						case MASTER_HANDSHAKE: { // 8FFF
							returnvalue = 0xB9A0;
						}break;
						case 0x5FFF : { // Ready Close Link
							//returnvalue = data;
							returnvalue = 0;
						}break;
						case SLAVE_HANDSHAKE : {
							//Detects if the master is still in handshake mode
							returnvalue = 0xB9A0;
						}break;
						case 0x0000 :{
							returnvalue = data;
						}break;
						default : {}break;
					}//end switch data
		}break;
		case 3: {
			returnvalue = data;
			switch(s_TradeHandlerMaster.LinkCMD){

				case 0x0000 : {}break;//Do nothing
				case 0xB9A0 : {}break;//Do nothing
				case 0x8FFF : {}break;//Do nothing
				case 0x2222 : {// Determine the Link Type
					s_TradeHandlerMaster.LinkType = data;
					s_TradeHandlerMaster.BlockRequestSize = BLOCK_SIZE_60;
					s_syncstate = BlockRequestHandler;
					printf("Send Link Typ\n");
					returnvalue = 0;
				}break;
				case 0xCCCC : { // determine the requested Block Size
					s_TradeHandlerMaster.BlockRequestSize = data;
					s_syncstate = BlockRequestHandler;
					returnvalue = 0;
				}break;
				case 0xBBBB : {
					s_TradeHandlerMaster.InitBlockSize = data;
					s_syncstate = InitBlockState;
					returnvalue = 0;

					//returnvalue = 0;
				}break;
				case 0x8888 : {}break;
				case 0xCAFE : {// Determine pressed buttons from MAster
					s_TradeHandlerMaster.PressedButtons = data;
					switch(Switches_State){
						case 1 :{returnvalue = 0x0012;}break;
						case 2 :{returnvalue = 0x0013;}break;
						case 4 : {returnvalue = 0x0014;}break;
						case 8 : {returnvalue = 0x0015;}break;
						default : {returnvalue = data;}break;
					}
				}break;
				case 0x5FFF : {
					s_TradeHandlerMaster.ExitValue = data;
					s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
					//printf("EXITVALUE = %d\n",s_TradeHandlerMaster.ExitValue);
					Threadstatus = NoThread;
					returnvalue = 0;
					//returnvalue = data;
					s_syncstate = RequestExitState;
				}break;
				default : {}break;
			}// end switch LINKCMD
		}break;
		case 4 ... 9:{returnvalue = data;}break;
		default : {

		}break;
		}//end switch Framecounter

	}break;

	default : {returnvalue = data;}break;
	}///end switch syncstate
	//printf("FC : %d ",FrameCounter);
	return returnvalue;
}



u16 GetBlockSize(BlockSizes BlockSize){
	switch(BlockSize){
	case BLOCK_REQ_SIZE_NONE : {return 200;}break; // Identical to 200
	case BLOCK_REQ_SIZE_200 : {return 200;}break;//xC8
	case BLOCK_REQ_SIZE_100 : {return 100;}break;//64
	case BLOCK_REQ_SIZE_220 : {return 220;}break;//DC
	case BLOCK_REQ_SIZE_40 : {return 40;}break;
	case BLOCK_SIZE_60 : {return 60;}break;
	case BLOCK_SIZE_20 : {return 20;}break;
	default : {return 0;}
	}
	return 0;
}

void UpdateFrameCounter(u32* FrameCounterPtr){
    if (*FrameCounterPtr == FRAME_LENGTH){
		*FrameCounterPtr = 1;
	}else{
		(*FrameCounterPtr)++;
	}
    return;
}

u8 DetectResetButtonPress(u8 Button,char *ButtonType){
	static u8 previousState = 0;
	if ((previousState == 0) && (Button == 1) == 1){
		//detect of rising edge
		previousState = Button;
		printf("%sButton pressed\n",ButtonType);
		return 1;
	}
	else if ((previousState == 1) && (Button == 0)){
		previousState = Button;
		printf("%sButton released\n",ButtonType);
		return 0;
	}
	else{
		return 0;
	}

}
u8 DetectTradeButtonPress(u8 Button){
	static u8 previousState = 0;
	if ((previousState == 0) && (Button == 1) == 1){
		//detect of rising edge
		previousState = Button;
		printf("Button pressed\n");
		return 1;
	}
	else if ((previousState == 1) && (Button == 0)){
		previousState = Button;
		printf("Button released\n");
		return 0;
	}
	else{
		return 0;
	}
}

void *InitTradeBuffer(void* ptr){
	u8* Spotnumber = (u8*) ptr;
	u16 Blocksize = 0x0014;
	u16 Databuffer[10] = {0xAABB,*Spotnumber,0,0,0,0,0,0,0,0};
	printf("init Trade Buffer with spot %d\n",*Spotnumber);
	GenerateBlockInit(Blocksize);
	GenerateDataBlock(Blocksize, Databuffer);
	printf("Finsihed\n");
	return ptr;
}

void GenerateTradeCommandBlock(u16 TradeCommand,u16 TradeCommandData){
	u16 Blocksize = 0x0014;
	u16 Databuffer[10] = {TradeCommand,TradeCommandData,0,0,0,0,0,0,0,0};
	printf("Generate TradeCommand Block with Command : %04x and Data : %04x\n",TradeCommand,TradeCommandData);
	GenerateBlockInit(Blocksize);
	GenerateEmptyFrame();GenerateEmptyFrame();GenerateEmptyFrame();
	GenerateDataBlock(Blocksize, Databuffer);
	printf("Finsihed\n");
	return;
}


RequestStatus BlockRequestAnalyser(u32 Framecounter,u16 data){
	//Function which safes the received Pokemondata(Master) in a corresponding data structure
	static u8 bufferindex = 0;
	static u8 DataFieldActive =0;

	if (Framecounter == 1){
		//ignore
	}
	else if( Framecounter == 2){
		if( data == 0x8888){
			DataFieldActive = 1;
		}
		else{
			DataFieldActive = 0;
		}
	}
	else{
		if ( DataFieldActive == 1){

			switch(s_TradeHandlerMaster.BlockRequestSize){

			case BLOCK_REQ_SIZE_200 :{
				if ( bufferindex < 50 ){
					s_TradeHandlerMaster.PokemonTeam[s_TradeHandlerMaster.TeamIndex][bufferindex++] = data;
				}else if ((bufferindex >= 50) && (bufferindex < 100)){
					s_TradeHandlerMaster.PokemonTeam[s_TradeHandlerMaster.TeamIndex+1][(bufferindex++)-50] = data;
				}else{
					if( Framecounter == 9){
						DataFieldActive = 0;
						bufferindex = 0;
						Threaddata.Connect_ID = ExchangePokemonData;
						Threaddata.TeamIndex = s_TradeHandlerMaster.TeamIndex;
						Threadstatus = Running;
						rc = pthread_create( &thread1, NULL, ExchangePokemonBufferTCP,(void *)&Threaddata);
						s_TradeHandlerMaster.TeamIndex = (s_TradeHandlerMaster.TeamIndex + 2);
						if(s_TradeHandlerMaster.TeamIndex > 5){
							s_TradeHandlerMaster.TeamIndex = 0;
						}
						printf("Block Analyser finished\n");
						return Granted;
					}
				}
			}break;
			case BLOCK_REQ_SIZE_100 : {
				if ( bufferindex < 50 ){
					s_TradeHandlerMaster.TrainerCard[bufferindex++] = data;
				}else{
					if( Framecounter == 9){
						DataFieldActive = 0;
						bufferindex = 0;
						Threaddata.Connect_ID = ExchangeTrainerData;
						Threaddata.ArgBufferSize = GetBlockSize(s_TradeHandlerMaster.BlockRequestSize);
						Threaddata.ArgBufferPtr = s_TradeHandlerMaster.TrainerCard;
						Threadstatus = Running;
						rc = pthread_create( &thread1, NULL, ExchangeBufferTCP,(void *)&Threaddata);
						//rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
						printf("Block Analyser finished\n");
						return Granted;
					}
				}
			}break;

			case BLOCK_REQ_SIZE_220 : {
				if(bufferindex < 110){
					s_TradeHandlerMaster.LetterBuffer[bufferindex++] = data;
				}else{
					if( Framecounter == 9){
						DataFieldActive = 0;
						bufferindex = 0;
						Threaddata.Connect_ID = ExchangeLetterData;
						Threaddata.ArgBufferSize = GetBlockSize(s_TradeHandlerMaster.BlockRequestSize);
						Threaddata.ArgBufferPtr = s_TradeHandlerMaster.LetterBuffer;
						Threadstatus = Running;
						rc = pthread_create( &thread1, NULL, ExchangeBufferTCP,(void *)&Threaddata);
						//rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
						printf("Block Analyser finished\n");
						return Granted;
					}
				}
			}break;
			case BLOCK_REQ_SIZE_40 : {
				if(bufferindex < 20){
					s_TradeHandlerMaster.RibbonsBuffer[bufferindex++] = data;
				}
				else{
					if( Framecounter == 9){
						DataFieldActive = 0;
						bufferindex = 0;
						Threaddata.Connect_ID = ExchangeRibbonData;
						Threaddata.ArgBufferSize = GetBlockSize(s_TradeHandlerMaster.BlockRequestSize);
						Threaddata.ArgBufferPtr = s_TradeHandlerMaster.RibbonsBuffer;
						Threadstatus = Running;
						rc = pthread_create( &thread1, NULL, ExchangeBufferTCP,(void *)&Threaddata);
						//rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
						printf("Block Analyser finished\n");
						return Granted;
					}
				}
			}
			case BLOCK_SIZE_60 : {
				if(bufferindex < 30){
					s_TradeHandlerMaster.InitialBuffer[bufferindex++] = data;
				}
				else{
					if(Framecounter == 9){
						DataFieldActive = 0;
						bufferindex = 0;
						Threaddata.Connect_ID = SendInitialBlock;
						Threaddata.ArgBufferSize = GetBlockSize(s_TradeHandlerMaster.BlockRequestSize);
						Threaddata.ArgBufferPtr = s_TradeHandlerMaster.InitialBuffer;
						Threadstatus = Running;
						rc = pthread_create( &thread1, NULL, ExchangeBufferTCP,(void *)&Threaddata);
						//rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
						printf("Received Initial Block\n");
						return Granted;
					}
				}
			}break;
			default : {

			}break;
			}//end switch;
		}
	}
	return Pending;
}

RequestStatus CreateExitThread(u32 Framecounter){
	if(Framecounter == 9){
		Threadstatus = Running;
		printf("Exitvalue = %d\n",s_TradeHandlerMaster.ExitValue);
		switch(s_TradeHandlerMaster.ExitValue){
			case 0 :{Threaddata.Connect_ID = Exit_0;} break;
			case 0x20 :{Threaddata.Connect_ID = Exit_20;} break;
			case 0xC :{Threaddata.Connect_ID = Exit_C;} break;
			default : {Threaddata.Connect_ID = Exit_0;}break;
		}
		printf("THREAD ID : %d\n",Threaddata.Connect_ID);
		//ExchangeIDTCP((void *)&Threaddata);
		rc = pthread_create( &thread1, NULL, ExchangeIDTCP,(void *)&Threaddata);
		return Granted;
	}else{
		return Pending;
	}
}


//Generates an Array buffer with the data to send back to master for initilizing a data Block Transfer from the Slave
//The Array to be filles with data is passed by reference in the function arguments

void GenerateBlockInit(u16 Blocksize){
	WriteBuffer(0xBBBB, &s_ControlBuffer);
	WriteBuffer(Blocksize, &s_DataBuffer);
	WriteBuffer(0x0081, &s_DataBuffer);
	for(int i = 4;i < FRAME_LENGTH;i++){
		WriteBuffer(0, &s_DataBuffer);
	}

	return;
}

void GenerateEmptyFrame(){
	WriteBuffer(0, &s_ControlBuffer);
	for (int i = 0;i < 7;i++){
		WriteBuffer(0, &s_DataBuffer);
	}
}
//Generates an Array buffer for data to send back to master for actually transfering the corresponding data
//The Array to be filles with data is passed by reference in the function arguments
void GenerateDataBlock(u16 Blocksize,u16 *Databuffer){
	u16 AmountControlFields = 0;
	u16 Buffersize = Blocksize/2;
	u8 remainder = Buffersize%7;
	if(Blocksize > 0 ){
		if (  remainder == 0){
			AmountControlFields = Buffersize/7;
		}else{
			AmountControlFields = (Buffersize/7)+1;
		}
		//printf("Amount = %d Buffersize = %d Blcoksize = %d\n",AmountControlFields,Buffersize,Blocksize);
	}
	for(int i = 0; i < AmountControlFields;i++){
		WriteBuffer(0x8888, &s_ControlBuffer);
	}
	for(int i = 0;i < Buffersize;i++){
		WriteBuffer(Databuffer[i], &s_DataBuffer);
	}

	if ( remainder != 0){
		for (int i = 0; i < (7-remainder);i++){
			WriteBuffer(0, &s_DataBuffer);
			//printf("debug : %d Added\n",i);
		}
	}

	return;
}


void GenerateBlockRequestResponse(BlockSizes Blocksize){
	u16 BlocksizeByte = GetBlockSize(Blocksize);
	static u8 TeamIndex = 0;
	extern u16 PokemonTeamBuffer[3][100];
	GenerateEmptyFrame();
	GenerateEmptyFrame();
	GenerateBlockInit(BlocksizeByte);
	GenerateEmptyFrame();
	GenerateEmptyFrame();
	switch(Blocksize){
	case BLOCK_REQ_SIZE_NONE : {

	}break; // Identical to 200
	case BLOCK_REQ_SIZE_200 : {
		GenerateDataBlock(BlocksizeByte,PokemonTeamBuffer[TeamIndex++]);
		TeamIndex= TeamIndex%3;
	}break;//xC8
	case BLOCK_REQ_SIZE_100 : {
		//GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.TrainerCard);
		GenerateDataBlock(BlocksizeByte, ReceiveBuffer);
	}break;//64
	case BLOCK_REQ_SIZE_220 : {
		//GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.LetterBuffer);
		GenerateDataBlock(BlocksizeByte, ReceiveBuffer);
	}break;//DC
	case BLOCK_REQ_SIZE_40 : {
		//GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.RibbonsBuffer);
		GenerateDataBlock(BlocksizeByte, ReceiveBuffer);
	}break;
	case BLOCK_SIZE_60 : {
		//GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.InitialBuffer);
		GenerateDataBlock(BlocksizeByte, ReceiveBuffer);
	}break;
	default : {}
	}
	return;
}
RequestStatus InitBlockAnalyser(u32 Framecounter, u16 data){
	static u8 bufferindex = 0;
	static u8 DataFieldActive =0;
	if (Framecounter == 1){}
	else if( Framecounter == 2){
		if( data == 0x8888){
			DataFieldActive = 1;
		}
		else{
			DataFieldActive = 0;
		}
	}else{
		if ( DataFieldActive == 1){
			if ( bufferindex < 10 ){
				s_TradeHandlerMaster.TradeCommandBuffer[bufferindex++] = data;
			}else{
				if( Framecounter == 9){
					DataFieldActive = 0;
					bufferindex = 0;
					printf("Init Block Analyser Finished\n");
					return Granted;
				}
			}
		}
	}
	return Pending;


}

