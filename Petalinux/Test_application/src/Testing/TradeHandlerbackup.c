/*
#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/Link.h"
#include "../Includes/linkcommand.h"
#include <stdlib.h>

#include "../Includes/TradeHandler.h"
#include "../Includes/Pokemonbuffer.h"

extern u8 Switches_State;
struct TradeHandler{
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
};
u16 ReceivedTeam[6][50];


void UpdateFrameCounter(u32* FrameCounterPtr){
    if (*FrameCounterPtr == FRAME_LENGTH){
		*FrameCounterPtr = 1;
	}else{
		(*FrameCounterPtr)++;
	}
    return;
}

u8 DetectButtonPress(u8 Button,char *ButtonType){
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


s8 ReadytoTrade(u32 FrameCounter,u32 data,u16* returnvalue,u8 SpotNumber){
	//Returnvalue = 0 if Sequence not finished, = 1 if Tradesequence is finished and normal Operation is to be continued
	static u8 FrameNumber = 0;
	//If Framecounter = 1 return sended data
	if (FrameCounter == 1){

		*returnvalue = (u16)data;
	}
	else if (FrameCounter == 2){
		if (FrameNumber == 0){
			*returnvalue = (u16)data;
		}
		else if (FrameNumber == 1){
			*returnvalue = LINKCMD_INIT_BLOCK;
		}
		else if (FrameNumber == 2){
			*returnvalue = LINKCMD_CONT_BLOCK;
		}else if(FrameNumber == 3){
			*returnvalue = LINKCMD_CONT_BLOCK;
		}
		else{
			*returnvalue = 0;
		}
		return 0;
	}
	else if (FrameCounter == 3){
		if (FrameNumber == 0){
			*returnvalue = (u16)data;
		}
		else if (FrameNumber == 1){
			*returnvalue = 0x0014; //
		}
		else if (FrameNumber == 2){
			*returnvalue = LINKCMD_READY_TO_TRADE; //AABB
		}
		else{
			*returnvalue = 0;
		}
	}
	else if (FrameCounter == 4){
		if (FrameNumber == 0){
			*returnvalue = (u16)data;
		}
		else if (FrameNumber == 1){
			*returnvalue = 0x0081;
		}
		else if (FrameNumber == 2){
			if (SpotNumber > 5){
				*returnvalue = 5;
			}
			else{
				*returnvalue = SpotNumber;
			}

		}
		else{
			*returnvalue = 0;
		}
	}
	else if (FrameCounter == 9){
		if (FrameNumber == 0){
			FrameNumber++;
			*returnvalue = (u16)data;
		}
		else if (FrameNumber == 3){
			*returnvalue = 0;
			FrameNumber = 0; // since static variable it is set to zero before return 1
			return 1; // Return to normal operation mode
		}
		else{
			FrameNumber++;
			*returnvalue = 0;
		}
	}
	else{
		if (FrameNumber == 0){
			*returnvalue = (u16)data;
		}
		else{
			*returnvalue = 0;
		}
	}
	if ((FrameNumber < 0) && (FrameNumber > 3)){
		return -1; // Error
	}
	else{
		return 0;
	}
	//Wait till FrameCounter is 1 to Send data sequence

}

void BlockRequestAnalyser(u32 Framecounter,u16 data){
	//Function which safes the received Pokemondata(Master) in a corresponding data structure
	static u8 bufferindex = 0;
	static u8 TeamIndex = 0;
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
			if ( bufferindex < 50 ){
				ReceivedTeam[TeamIndex][bufferindex++] = data;
			}else if ((bufferindex >= 50) && (bufferindex < 100)){
				ReceivedTeam[TeamIndex+1][(bufferindex++)-50] = data;
			}else{
				DataFieldActive = 0;
				bufferindex = 0;
				//decode_Pokemon_data(ReceivedTeam[TeamIndex]);
				//decode_Pokemon_data(ReceivedTeam[TeamIndex+1]);
				TeamIndex = (TeamIndex + 2) % 6;

			}

		}
		else{
			//do nothing
		}
	}
}

//Generates an Array buffer with the data to send back to master for initilizing a data Block Transfer from the Slave
//The Array to be filles with data is passed by reference in the function arguments
void GenerateBlockInitArray(u16 Blocksize,u16 *BlockInitArray){
	size_t elementsize = sizeof(u16);
	size_t Arraysize = sizeof(BlockInitArray) / elementsize;
	*(BlockInitArray) = 0xBBBB;
	*(BlockInitArray+1) = Blocksize;
	*(BlockInitArray+2) = 0x0081; //Dont know what this means exactly but master sens 0x0080
	for(int i = 3; i < Arraysize;i++){
		*(BlockInitArray+i) = 0;
	}
	return;
}
//Generates an Array buffer for data to send back to master for actually transfering the corresponding data
//The Array to be filles with data is passed by reference in the function arguments
void GenerateDataBlockArray(u16 Blocksize,u16 *Databuffer, u16 *DataBlockArray){
	return;
}


s8 BlockRequestResponse(u32 FrameCounter, u16* returnvalue){
	static u16 BlockInitArray[8];

	static u8 bufferindex = 0;
	static u8 FrameCount = 0;

	static u8 TeamIndex = 0;
	switch(FrameCount){
	case 0 ... 1 : {
		*returnvalue = 0;
	}break;

	case 2 : {
		*returnvalue = BlockInitArray[FrameCounter-2];
	}break;

	case 3 ... 4 : {
		*returnvalue = 0;
	}break;
	case 5 ... 19 : {
		if (FrameCounter == 2){
			*returnvalue = 0x8888;
		}else{

			if (bufferindex < 50){
				*returnvalue = PokemonTeam[TeamIndex][bufferindex++];
				if (bufferindex == 50){
					TeamIndex++;
				}
				//*returnvalue = Cyndaquil_pokemonbuffer[bufferindex++];
			}else if ((bufferindex >= 50) && (bufferindex < 100)){
				*returnvalue = PokemonTeam[TeamIndex][(bufferindex++) - 50];
				if (bufferindex == 100){
					TeamIndex++;

				}
				//*returnvalue = Totodile_pokemonbuffer[(bufferindex++) - 50];
			}
			else{
				*returnvalue = 0;
			}
		}
	}break;
	case 20 : {
		*returnvalue = 0;
		if (FrameCounter == 9){
			bufferindex = 0;
			FrameCount = 0;
			TeamIndex = TeamIndex % 6;
			return 0;
		}

	}break;
	default : {
		*returnvalue = 0;
	}break;
	}//end switch Framecount
	if (FrameCounter == 9){
		FrameCount++;
	}
	return 1;
}




u32 TradeHandler(u32 data,u32 PL_to_PS_buffer_value){

	static Syncstate s_syncstate = handshakeState;

	static u32 FrameCounter = 2;

	static Frametype s_Frametype = Identification;

	static s8 Check = 0;

	//static u8 index = 0;

	u16 returnvalue = (u16)data ;

	static struct TradeHandler s_TradeHandlerMaster;
	static __attribute__ ((unused)) struct TradeHandler s_TradeHandlerSlave;

	static u8 SendTradeDataStatus= 0;

	static u8 __attribute__ ((unused)) Switches_State;
	static u8 __attribute__ ((unused)) Resetbutton_State;
	static u8 __attribute__ ((unused)) Tradebutton_State ;
	Switches_State = (u8)(PL_to_PS_buffer_value & SWITCH_BITMASK);
	Resetbutton_State = (u8)((PL_to_PS_buffer_value & RESETBUTTON_STATE) >> 4);
	Tradebutton_State = (u8)((PL_to_PS_buffer_value & TRADEBUTTON_STATE) >> 5);
	//Reset
	if (DetectButtonPress(Resetbutton_State, "Reset") == 1){
		s_syncstate = handshakeState;
		return 0;
	}

	if ((DetectButtonPress(Tradebutton_State,"Trade") == 1) && (SendTradeDataStatus == 0)){
		SendTradeDataStatus = 1;
	}

	// Calculate the frame counter depending on the  sysnc state
	if (s_syncstate == handshakeState){
		FrameCounter = FRAME_LENGTH;
		returnvalue = 0xB9A0;
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
			if ((SendTradeDataStatus == 1)){
				s_Frametype = Tradebutton;
				return data;
			}
//			printf("BR : %d \n",s_TradeHandlerMaster.BlockRequest);
			if (s_TradeHandlerMaster.BlockRequest == 1){
				s_Frametype = BlockRequestHandler;
				return data;
			}
			s_Frametype = Identification;
			return data;
		}

		switch(s_Frametype){
		// Handler for Tradebutton pressed
		case Tradebutton : {
			Check = ReadytoTrade(FrameCounter, data, &returnvalue, Switches_State);
			if( Check == -1){
				printf("Error in Ready_to_Trade\n");
				abort();
			}else if(Check == 1){
				SendTradeDataStatus = 0;
				printf("Successful send Trade Choice\n");
				return returnvalue;
			}
			else{//Check = 0
				return returnvalue;
			}
		}break;


		case BlockRequestHandler : {

			if (s_TradeHandlerMaster.BlockRequestSize == 1){// Inddicates Pokemon structure send

				s_TradeHandlerMaster.BlockRequest = BlockRequestResponse(FrameCounter, &returnvalue);
				BlockRequestAnalyser(FrameCounter, data);
			}else{
				//printf("Debug block request size ungleich 1\n");
				s_TradeHandlerMaster.BlockRequest = 0;
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
					s_TradeHandlerMaster.BlockRequest = 1;
					s_TradeHandlerSlave.BlockRequest = 1;
					returnvalue = 0;
				}break;

				case 0xBBBB : { //Init Block
					s_TradeHandlerMaster.DataIndex = 0;
					s_TradeHandlerMaster.InitBlock = 1;
					returnvalue = data;
				}break;

				case 0x8888 : { // Cont Data Block

					s_TradeHandlerMaster.ContBlock = 1;
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
					s_TradeHandlerMaster.DataIndex = 0;
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
				if ((FrameCounter == 3) && (s_TradeHandlerMaster.BlockRequest == 1)){
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
*/
/*
s8 WriteBuffer(u16 datainput, struct BufferType *Buffer){
	Buffer->Data[Buffer->WriteIndex] = datainput;
	if (Buffer->WriteIndex >= (MAX_BUFFER_LENGTH-1) ){
		Buffer->WriteIndex = 0;
	}
	else{
		Buffer->WriteIndex++;
	}
	if (Buffer->WriteIndex == (Buffer->ReadIndex){
		Buffer->Full = 1;
		printf("Buffer full now\n");
		return 1;
	}
	return 0;
}*/