#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/Link.h"
#include "../Includes/linkcommand.h"
#include <stdlib.h>



#include "../Includes/TradeHandler.h"
#include "../Includes/Pokemonbuffer.h"

#include "../Includes/Ringbuffer.h"

#include "../Includes/decode_Pokemon_structure.h"

#include <pthread.h>
#include "../TCP_Server.h"

//extern u8 Switches_State;
/*struct TradeHandler{

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
};*/
u16 ReceivedTeam[6][50];
extern struct BufferType s_DataBuffer;
// DEclare a buffer structure for a Buffer with Data to be send to Ringbuffer of HW
extern struct BufferType s_ControlBuffer;

struct TradeHandler s_TradeHandlerMaster;

ThreadStatus Threadstatus = NoThread;

//pthread_attr_t attr;
pthread_t thread1;
int rc;


void *DecodePokemonToFile(void *ptr){
	u8* TeamIndexptr = (u8*) ptr;
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[*TeamIndexptr]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[*TeamIndexptr + 1]);

	return ptr;
}

void *DecodeReceivedPokemonToFile(void *ptr){
	extern FILE *PokemonFp;;
	printf("Write Extracted Data into datalog.txt\n");
	PokemonFp = fopen("/home/petalinux/datalog.txt","w");
	if(PokemonFp == NULL){
		printf("Error opening file datalog.txt.\n");
		exit(0);
	}
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[0]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[1]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[2]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[3]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[4]);
	decode_Pokemon_data(s_TradeHandlerMaster.PokemonTeam[5]);
	printf("Write Successful\n");
	fclose(PokemonFp);
	return ptr;
}

u16 GetBlockSize(BlockSizes BlockSize){
	switch(BlockSize){
	case BLOCK_REQ_SIZE_NONE : {return 200;}break; // Identical to 200
	case BLOCK_REQ_SIZE_200 : {return 200;}break;//xC8
	case BLOCK_REQ_SIZE_100 : {return 100;}break;//64
	case BLOCK_REQ_SIZE_220 : {return 220;}break;//DC
	case BLOCK_REQ_SIZE_40 : {return 40;}break;
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

u8 BlockRequestAnalyser(u32 Framecounter,u16 data){
	//Function which safes the received Pokemondata(Master) in a corresponding data structure
	static u8 bufferindex = 0;
	static u8 TeamIndex = 0;
	static u8 DataFieldActive =0;
	static u8 Teamindexptr[1];


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
			if ( bufferindex < 50 ){

				s_TradeHandlerMaster.PokemonTeam[TeamIndex][bufferindex++] = data;
			}else if ((bufferindex >= 50) && (bufferindex < 100)){
				s_TradeHandlerMaster.PokemonTeam[TeamIndex+1][(bufferindex++)-50] = data;
			}else{
				if( Framecounter == 9){
					DataFieldActive = 0;
					bufferindex = 0;
					*Teamindexptr = TeamIndex;
					Threadstatus = Running;
					rc = pthread_create( &thread1, NULL, NewConnectionServer,(void *)Teamindexptr);
					//rc = pthread_create( &thread1, NULL, DecodePokemonToFile, (void *)Teamindexptr);
					TeamIndex = (TeamIndex + 2);
					if(TeamIndex > 5){
						TeamIndex = 0;
						//rc = pthread_create( &thread1, NULL, DecodeReceivedPokemonToFile, NULL);
					}
					return 1;
				}
			}

		}
	}
	return 0;
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
	switch(Blocksize){
	case BLOCK_REQ_SIZE_NONE : {

	}break; // Identical to 200
	case BLOCK_REQ_SIZE_200 : {
		GenerateEmptyFrame();
		GenerateEmptyFrame();
		GenerateDataBlock(BlocksizeByte,PokemonTeamBuffer[TeamIndex++]);
		TeamIndex= TeamIndex%3;
	}break;//xC8
	case BLOCK_REQ_SIZE_100 : {

	}break;//64
	case BLOCK_REQ_SIZE_220 : {

	}break;//DC
	case BLOCK_REQ_SIZE_40 : {

	}break;
	default : {}
	}
	return;
}


u32 TradeHandler(u32 data,u32 PL_to_PS_buffer_value){

	static Syncstate s_syncstate = handshakeState;

	static u32 FrameCounter = 2;

	static Frametype s_Frametype = Identification;

	u16 returnvalue = (u16)data ;
	//static pthread_t thread1;
	//int rc;

	//static struct TradeHandler s_TradeHandlerMaster;
	static __attribute__ ((unused)) struct TradeHandler s_TradeHandlerSlave;

	static RequestStatus SendTradeDataStatus=  NoRequest;

	static u8 __attribute__ ((unused)) Switches_State;
	static u8 __attribute__ ((unused)) Resetbutton_State;
	static u8 __attribute__ ((unused)) Tradebutton_State ;


	Switches_State = (u8)(PL_to_PS_buffer_value & SWITCH_BITMASK);
	//u8 Spotnumber = Switches_State%6;
	static u8* Spotptr = &Switches_State;
	Resetbutton_State = (u8)((PL_to_PS_buffer_value & RESETBUTTON_STATE) >> 4);
	Tradebutton_State = (u8)((PL_to_PS_buffer_value & TRADEBUTTON_STATE) >> 5);
	//Reset
	if (DetectResetButtonPress(Resetbutton_State,"reset") == 1){
		s_syncstate = handshakeState;
		ResetBuffer(&s_DataBuffer);
		ResetBuffer(&s_ControlBuffer);
		return 0;
	}

	if ((DetectTradeButtonPress(Tradebutton_State) == 1) && (SendTradeDataStatus == NoRequest)){

		Spotptr = &Switches_State;
		InitTradeBuffer((void *) Spotptr);
		//rc = pthread_create( &thread1, NULL, InitTradeBuffer, (void *)Spotptr);
		SendTradeDataStatus = Active;
	}

	// Calculate the frame counter depending on the  sysnc state
	if (s_syncstate == handshakeState){
		FrameCounter = FRAME_LENGTH;
		returnvalue = 0xB9A0;
		s_TradeHandlerMaster.DataHandlerstatus = NoData;
		//If data is 8FFF the communication switches to data phase
		if (data == MASTER_HANDSHAKE){
			s_syncstate = DataState;
		}
	}
	else if(s_syncstate == ExitState){
		UpdateFrameCounter(&FrameCounter);
		returnvalue = data;
		if(data == 0xB9A0){
			s_syncstate = handshakeState;
		}
	}
	else{
		//Update FrameCounter
		UpdateFrameCounter(&FrameCounter);

		//update The Frametyp which is now to be dealt with
		if (FrameCounter == 1){
			//printf("\n");
			if ((SendTradeDataStatus == Active)){
				s_Frametype = Tradebutton;
				return data;
			}
//			printf("BR : %d \n",s_TradeHandlerMaster.BlockRequest);
			if (s_TradeHandlerMaster.BlockRequest == Active){
				s_Frametype = BlockRequestHandler;
				return data;
			}
			s_Frametype = Identification;
			return data;
		}

		switch(s_Frametype){

		// Handler for Tradebutton pressed
		case Tradebutton : {

			if(FrameCounter == 2){
				returnvalue = ReadBuffer(&s_ControlBuffer);
			}else{
				returnvalue = ReadBuffer(&s_DataBuffer);
				if (FrameCounter == 9){
					if (BufferEmpty(&s_ControlBuffer) == 1){
						SendTradeDataStatus = NoRequest;
						printf("Successful Send Trade Choice\n");
					}
				}
			}
			return returnvalue;
		}break;

		case BlockRequestHandler : {
			if (s_TradeHandlerMaster.BlockRequestSize == BLOCK_REQ_SIZE_200){
				switch(s_TradeHandlerMaster.DataHandlerstatus){
				case NoData :{ returnvalue = 0;}break;

				case CaptureData : {
					returnvalue = 0;
					if(BlockRequestAnalyser(FrameCounter, data) == 1){
						s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
						printf("Wait for Thread to finish\n");
					}else{
						s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
					}

				}break;

				case WaitforThread : {
					if( Threadstatus == Finished){
						if(FrameCounter == 9){
							Threadstatus = NoThread;
							s_TradeHandlerMaster.DataHandlerstatus = SendData;
							GenerateBlockRequestResponse(BLOCK_REQ_SIZE_200);
						}
					}else{
						s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
					}

				}break;

				case SendData : {
					if(FrameCounter == 2){
						returnvalue = ReadBuffer(&s_ControlBuffer);
					}else{
						returnvalue = ReadBuffer(&s_DataBuffer);
						if (FrameCounter == 9){
							if (BufferEmpty(&s_ControlBuffer) == 1){
								s_TradeHandlerMaster.BlockRequest = NoRequest;
								s_TradeHandlerMaster.DataHandlerstatus = NoData;
								printf("Successful Send 100 Bytes PokemonBuffer\n");
							}
						}
					}

				}break;

				}//end switch
			}
			else{//For other request, currently only pokemon data captured
				s_TradeHandlerMaster.BlockRequest = NoRequest;
				returnvalue = data;
			}
			return returnvalue;
		}break;

		case Identification : {
			switch(FrameCounter){
			case 1 :{ /// Start of new Frame. Checksum.
				returnvalue = data;
			}break;
			case 2 :{
				s_TradeHandlerMaster.LinkCMD = data;
				switch(data){
				case 0xCCCC : { //REquest Data Block
					s_TradeHandlerMaster.BlockRequest = Active;
					s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
					returnvalue = 0;
				}break;

				case 0xBBBB : { //Init Block
					s_TradeHandlerMaster.InitBlock = Active;
					returnvalue = data;
				}break;

				case 0x8888 : { // Cont Data Block
					returnvalue = data;
				}break;

				case 0x2222 : { // Link CMD Type
					returnvalue = 0;
				}break;
				case 0xCAFE : { // Hold buttons
					returnvalue = 0xCAFE;
				}break;

				case MASTER_HANDSHAKE: { // 8FFF
					returnvalue = 0xB9A0;
				}break;

				case 0x5FFF : { // Ready Close Link
					returnvalue = data;
					s_syncstate = ExitState;
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

			case 3 ... 9:{
				returnvalue = data;
				switch(s_TradeHandlerMaster.LinkCMD){

				case 0x0000 : {}break;//Do nothing
				case 0xB9A0 : {}break;//Do nothing
				case 0x8FFF : {}break;//Do nothing
				case 0x2222 : {// Determine the Link Type
				if (FrameCounter == 3){s_TradeHandlerMaster.LinkType = data;}
					returnvalue = 0;
				// Print Link Type
				}break;
				case 0xCCCC : { // determine the requested Block Size
				if ((FrameCounter == 3) && (s_TradeHandlerMaster.BlockRequest == Active)){
					s_TradeHandlerMaster.BlockRequestSize = data;
				}
					returnvalue = 0;
				}break;
				case 0xBBBB : {
					if (FrameCounter == 3){s_TradeHandlerMaster.InitBlockSize = data;}
					//returnvalue = 0;
				}break;
				case 0x8888 : {}break;
				case 0xCAFE : {// Determine pressed buttons from MAster
					if (FrameCounter == 3){s_TradeHandlerMaster.PressedButtons = data;
						switch(Switches_State){
						case 1 :{returnvalue = 0x0012;}break;
						case 2 :{returnvalue = 0x0013;}break;
						case 4 : {returnvalue = 0x0014;}break;
						case 8 : {returnvalue = 0x0015;}break;
						default : {returnvalue = data;}break;
						}
					}
				}break;
				default : {}break;
				}// end switch LINKCMD

				}break;
				default:{
					returnvalue = 0;
				}
			}//end switch Framecounter

		}break;

		default : {}break;

		}//end swithc Frametype


	}///end if syncstate
	return returnvalue;
}
