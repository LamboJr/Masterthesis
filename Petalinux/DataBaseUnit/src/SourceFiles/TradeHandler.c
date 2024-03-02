#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/Link.h"
#include "../Includes/linkcommand.h"
#include <stdlib.h>




#include "../Includes/Pokemonbuffer.h"

#include "../Includes/Ringbuffer.h"



#include <pthread.h>
#include "../Includes/ConCat.h"
#include "../Includes/TradeHandler.h"
#include "../Includes/BackUpHandler.h"

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


