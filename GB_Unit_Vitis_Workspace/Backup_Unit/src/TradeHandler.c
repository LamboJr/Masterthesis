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
#include "Includes/BackUpHandler.h"

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


/*This Function converts the Block Request Size ( 1-4)
 * into the actuall data block size in Bytes
 * The Block request Size is send after a 0xCCCC control field
 *
 * */

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

/*
 * This function updates the FrameCounter.
 *
 * */

void UpdateFrameCounter(u32* FrameCounterPtr){
    if (*FrameCounterPtr == FRAME_LENGTH){
		*FrameCounterPtr = 1;
	}else{
		(*FrameCounterPtr)++;
	}
    return;
}

/*
 * This function detects if the reset button is pressed and released
 *
 * */

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

/*
 * This function detects if the trade button is pressed
 */

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

/*
 * This function is used to create a data block which contains the Tradecommand 0xAABB
 * which indicates that a trade choice is made by the slave
 * The Spotnumber specifies the Teammember which is traded to the Master( physical GBA)
 *
 * */

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
/*
 * Generates a datablock which is send to the master containing 20 Byte , so 10 16-Bit data packets
 * The datablock contains a Tradecommand followed by the Tradecommand data
 * This function calls subfunction to fill the Controlfield buffer and DataField buffer
 *
 */
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
//Creates a typical 		0xBBBB (Blocksize) 0x0081 0x0000 0x0 0x0 x0 0x0 structure
// see the 0xBBBB Frame meaning in the thesis
//Fills the corresponding Controlfield buffer and data field buffer
//The Buffer to be filled with data is passed by reference in the function arguments

void GenerateBlockInit(u16 Blocksize){
	WriteBuffer(0xBBBB, &s_ControlBuffer);
	WriteBuffer(Blocksize, &s_DataBuffer);
	WriteBuffer(0x0081, &s_DataBuffer);
	for(int i = 4;i < FRAME_LENGTH;i++){
		WriteBuffer(0, &s_DataBuffer);
	}

	return;
}
/*
 * Creates an empty 9er frame to send to the master
 * Fills the corresponding Controlfield buffer and data field buffer with 0x0 Values
 *
 * */
void GenerateEmptyFrame(){
	WriteBuffer(0, &s_ControlBuffer);
	for (int i = 0;i < 7;i++){
		WriteBuffer(0, &s_DataBuffer);
	}
}
//Generates an data block to send back to master for actually transferring the corresponding data
//Fills the corresponding Controlfield buffer and data field buffer with data to be send to the master.
// It determines the amount of 9er frame structures and the amount of 0x8888 control fields it needs to generate
// The function fills the data field of the generated 9er frame structure  with the data to be transmitted
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

/*
 * This function calls subfunctions to generate a complete  Data block
 * sturcture:
 * empty frame
 * empty frame
 * 0xBBBB frame with the corresponding Blocksize
 * empty frame
 * empty frame
 * specific amount of 0x8888 frames containing user data depending on the Block Request Size
 * DEpending on the Block Request Size, different data structures are used to genarte the data pakets used to send.
 * For example: Block_REQUEST_SIZE_200 generates a Data block containing two Pokemon data structures
 *
 */

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


