#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/Link.h"
#include "../Includes/linkcommand.h"
#include <stdlib.h>



#include "../Includes/TradeHandler.h"
#include "../Includes/BackUpHandler.h"

#include "../Includes/Ringbuffer.h"

#include <pthread.h>
#include "../TCP_Server.h"
#include "../Includes/ConCat.h"
#include "../DatabaseHandler.h"
#include "../Includes/DecodePokemonData.h"


extern u16 ReceivedTeam[6][50];

extern u16 ReceiveBuffer[110];

extern struct BufferType s_DataBuffer;
// DEclare a buffer structure for a Buffer with Data to be send to Ringbuffer of HW
extern struct BufferType s_ControlBuffer;

extern TradeHandler s_TradeHandlerMaster;

const char* StoredPokemonName = "Scherox";

extern ThreadData Threaddata;
//pthread_attr_t attr;
extern pthread_t thread1;
extern int rc;


u32 BackupTradeHandler(u32 data,u32 PL_to_PS_buffer_value){

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

	extern u16 PokemonTeamBuffer[3][100];


	Switches_State = (u8)(PL_to_PS_buffer_value & SWITCH_BITMASK);
	//u8 Spotnumber = Switches_State%6;
	//static u8* Spotptr = &Switches_State;
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

		//InitTradeBuffer((void *) Spotptr);
		SpotNumber = Switches_State % 6;
		GenerateTradeCommandBlock(0xAABB, SpotNumber);
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
				s_syncstate =handshakeState;
				printf("Enter HAndshake State\n");
		}
	}break;
	// Calculate the frame counter depending on the  sysnc state
	case handshakeState :{
	//else if (s_syncstate == handshakeState){
		FrameCounter = FRAME_LENGTH;
		returnvalue = data;
		ResetBuffer(&s_DataBuffer);
		ResetBuffer(&s_ControlBuffer);
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
						if(BackupBlockRequestAnalyser(FrameCounter, data) == Granted){
							s_TradeHandlerMaster.DataHandlerstatus = WaitforThread;
						}else{
							s_TradeHandlerMaster.DataHandlerstatus = CaptureData;
						}
					}break;

					case WaitforThread : {
						returnvalue = 0;
							if(FrameCounter == 9){
								s_TradeHandlerMaster.DataHandlerstatus = SendData;
								printf("generate Data structure\n");
								BackupGenerateBlockRequestResponse(s_TradeHandlerMaster.BlockRequestSize);
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
							Threaddata.ArgBufferPtr = s_TradeHandlerMaster.PokemonTeam[s_TradeHandlerMaster.ReceivedSpotNumber];

							for (int i = 0;i<50;i++){
								Threaddata.ArgBufferPtr2[i] = PokemonTeamBuffer[(SpotNumber/2)][i+(50*(SpotNumber%2))];
							}
							rc = pthread_create( &thread1, NULL, DataBaseInsertThread,(void *)&Threaddata);
							for(int i = 0; i < 50;i++){
								//DataBaseInsertBuffer(StoredPokemonName,s_TradeHandlerMaster.PokemonTeam[s_TradeHandlerMaster.ReceivedSpotNumber]);
								PokemonTeamBuffer[(SpotNumber/2)][i+(50*(SpotNumber%2))] = s_TradeHandlerMaster.PokemonTeam[s_TradeHandlerMaster.ReceivedSpotNumber][i];

							}
							//Trade completed. Store or load to backup database

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
					printf("RV C : %04x ",returnvalue);
				}else{
					returnvalue = ReadBuffer(&s_DataBuffer);
					printf("RV : %04x ",returnvalue);
					if (FrameCounter == 9){
						printf("\n");
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
							returnvalue = data;
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
					returnvalue = data;
					s_syncstate = ExitState;
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

RequestStatus BackupBlockRequestAnalyser(u32 Framecounter,u16 data){
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
						printf("Block Analyser finished\n");
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
void BackupGenerateBlockRequestResponse(BlockSizes Blocksize){
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
		GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.TrainerCard);
	}break;//64
	case BLOCK_REQ_SIZE_220 : {
		GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.LetterBuffer);
	}break;//DC
	case BLOCK_REQ_SIZE_40 : {
		GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.RibbonsBuffer);
	}break;
	case BLOCK_SIZE_60 : {
		GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.InitialBuffer);
	}break;
	case BLOCK_SIZE_20 : {
		GenerateDataBlock(BlocksizeByte, s_TradeHandlerMaster.TradeCommandBuffer);
	}break;
	default : {}
	}
	return;
}


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


