#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <assert.h>

#include "../Includes/Link.h"

#include "../Includes/DataTypes.h"

#include "../Includes/TradeHandler.h"


#include "../Includes/Ringbuffer.h"

#include "../Includes/ConCat.h"
#include "../Includes/DecodePokemonData.h"
#include "../TCP_Server.h"
#include "TestDataTrade.h"

u32 TestArrayPokemon1Data[100] = {
		 0x24FD    ,0x5F6B    ,0xC371    ,0x60AC    ,0xC3CD    ,0xCDC7    ,0xC6BB
		,0xFFBB    ,0x0000    ,0x0205    ,0xD5BE    ,0xDDE2    ,0xE0D9    ,0x00FF
		,0xA206    ,0x0000    ,0xE7CD    ,0x3FC7    ,0x8869    ,0x3FC5    ,0xA88C
		,0x3FC7    ,0x6A8C    ,0x2D70    ,0x63BC    ,0xA224    ,0xE78C    ,0x3FC7
		,0xE78C    ,0x3FC6    ,0xE78C    ,0x3FC7    ,0xE78C    ,0x3FC7    ,0xE7D2
		,0x3ED7    ,0xE774    ,0x3F53    ,0xED86    ,0x2BC8    ,0x0000    ,0x0000
		,0xFF37    ,0x0086    ,0x0086    ,0x003C    ,0x0031    ,0x008C    ,0x00BB
		,0x006A
		,0x3D3F
		,0xBFCA    ,0xC371    ,0x60AC    ,0xBFC1    ,0xC1C8    ,0xCCBB    ,0x00FF
		,0x0000    ,0x0205    ,0xD5BE    ,0xDDE2    ,0xE0D9    ,0x00FF    ,0x5992
		,0x0000    ,0xFF0B    ,0xDFEC    ,0xFE8C    ,0xDF91    ,0xF15A    ,0xD063
		,0xFE10    ,0xDF66    ,0xA3FD    ,0xDF67    ,0xB54E    ,0xDF66    ,0x4B4E
		,0xCDC8    ,0x3CF4    ,0xDB11    ,0xFE4E    ,0xDF66    ,0xFE4E    ,0xDF66
		,0xFE4E    ,0xDF66    ,0xFE4E    ,0xDF66    ,0x0000    ,0x0000    ,0xFF2E
		,0x007B    ,0x007B    ,0x004A    ,0x0049    ,0x0065    ,0x007F    ,0x004A
};



#define MODE_TRADE 0
#define MODE_MONITOR 1

#define TESTDATA_ARRAY TestDataTradePokemonExchange

extern FILE *PokemonFp;

extern u16 Magikarp_pokemonbuffer[50];

void StartFunctionTestPrint(const char *FuncName){
	printf("Start test of function --%s--\n",FuncName);
	return;
}
void EndFunctionTestPrint(const char *FuncName){
	printf("Test of function --%s-- successful\n",FuncName);
	return;
}

void main_TradeHandlerTest()
{
	printf("Test for Tradehandler\n");

	u32 OPMode = MODE_TRADE;

	printf("Set Mode to Trading Mode\n");
	//Configure Trading or Monitoring mode

	FILE *fp;
	printf("Write Monitor Output to file log.txt\n");
	// Open file in write mode
	fp = fopen("/home/petalinux/log.txt", "w");
	 // Check if file opened successfully
	if (fp == NULL) {
		printf("Error opening file.\n");
		return;
	}
	// Write output to file
	//fprintf(fp, "This is the output that will be written to the file.\n");
	printf("Write Extracted Data into datalog.txt\n");
	PokemonFp = fopen("/home/petalinux/datalog.txt","w");
	if(PokemonFp == NULL){
		printf("Error opening file datalog.txt.\n");
		return;
	}


	printf("Staring Test Tradehanlder Program\n");

	u16 data;
	u16 returnvalue = 0;
	for(int i = 0; i < sizeof(TESTDATA_ARRAY);i++){
		data = (u16)((TESTDATA_ARRAY[i] >> 16) & 0xFFFF);
		returnvalue = MainTradeHandler(data,0x0);
	}
/*	extern u16 ReceivedTeam[6][50];
	for(int u = 0;u <6;u++){
		printf("ReceivedTeam[%d] = \n",u);
		for (int i = 0; i < 50; i++){
			printf("0x%04x ,",ReceivedTeam[u][i]);
			if((i%7)==0){
				printf("\n");
			}
		}
		printf("\n");
		//decode_Pokemon_data(ReceivedTeam[u]);
	}*/

	return;
}
void ConCatTest(){
	StartFunctionTestPrint("ConCatTest");
	u16 resultbuffer[100];
	extern u16 Magikarp_pokemonbuffer[50];
	size_t Buffersize = sizeof(Magikarp_pokemonbuffer)/sizeof(u16);
	concat(Magikarp_pokemonbuffer, Magikarp_pokemonbuffer, resultbuffer,Buffersize,Buffersize);
	for (int i = 0; i < Buffersize;i++){
		assert(resultbuffer[i] == Magikarp_pokemonbuffer[i]);
		assert(resultbuffer[i+ Buffersize] == Magikarp_pokemonbuffer[i]);
	}
	EndFunctionTestPrint("ConCatTest");
}

void ButtonPressTest(){
	printf("Test of function --DetectButtonPress--\n");

	char *Buttontype = "Reset";
	u8 ButtonState = 0;
	assert(DetectTradeButtonPress(ButtonState) == 0);
	ButtonState = 1;
	assert(DetectTradeButtonPress(ButtonState) == 1);
	assert(DetectTradeButtonPress(ButtonState) == 0);
	assert(DetectTradeButtonPress(ButtonState) == 0);
	ButtonState = 0;
	assert(DetectTradeButtonPress(ButtonState) == 0);
	assert(DetectTradeButtonPress(ButtonState) == 0);
	ButtonState = 1;
	assert(DetectTradeButtonPress(ButtonState) == 1);
	ButtonState = 0;
	assert(DetectTradeButtonPress(ButtonState) == 0);
	printf("Test of function --DetectButtonPress-- successful\n");
	return;
}

void  UpdateFramecounterTest(){
	printf("Test of function --UpdataFrameCounter--\n");
	u32 FrameCounter = 1;
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 2);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 3);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 4);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 5);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 6);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 7);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 8);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 9);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 1);
	UpdateFrameCounter(&FrameCounter);	assert(FrameCounter == 2);

	printf("Test of function --UpdateFrameCounter-- succesful\n");
	return;
}


void GenerateBlockInitTest(){

	StartFunctionTestPrint("GenerateBlockInit");
	extern struct BufferType s_DataBuffer;
	extern struct BufferType s_ControlBuffer;
	ResetBuffer(&s_ControlBuffer);
	ResetBuffer(&s_DataBuffer);
	u16 Blocksize = 0xC8;
	u16 BlockInitArray[8] = {0xBBBB,Blocksize,0x0081,0,0,0,0,0};
	GenerateBlockInit(Blocksize);
	assert(ReadBuffer(&s_ControlBuffer) == BlockInitArray[0]);
	for(int i = 1;i < FRAME_LENGTH-1;i++){
		assert(ReadBuffer(&s_DataBuffer) == BlockInitArray[i]);
	}
	EndFunctionTestPrint("GenerateBlockInit");
	return;
}

void GenerateDataBlockTest(){
	StartFunctionTestPrint("GenerateDataBlock");
	extern struct BufferType s_DataBuffer;
	extern struct BufferType s_ControlBuffer;
	ResetBuffer(&s_ControlBuffer);
	ResetBuffer(&s_DataBuffer);
	u16 Blocksize = 0xC8;
	u16 ArgBuffer[100];
	extern u16 Magikarp_pokemonbuffer[50];
	size_t Buffersize = sizeof(Magikarp_pokemonbuffer)/sizeof(u16);
	concat(Magikarp_pokemonbuffer, Magikarp_pokemonbuffer, ArgBuffer,Buffersize,Buffersize);
	assert((s_DataBuffer.WriteIndex == 0));
	GenerateDataBlock(Blocksize,ArgBuffer);
	assert((s_DataBuffer.WriteIndex == 105)); //
	for (int i = 0; i < 15;i++){
		assert(ReadBuffer(&s_ControlBuffer) == 0x8888);
	}
	for (int i = 0; i < 100;i++){
		assert(ReadBuffer(&s_DataBuffer)== ArgBuffer[i]);
	}
	assert(ReadBuffer(&s_ControlBuffer) == 0);
	ResetBuffer(&s_ControlBuffer);
	ResetBuffer(&s_DataBuffer);
	u16 TestBuffer[14];
	Buffersize = sizeof(TestBuffer)/sizeof(u16);
	for(int i = 0; i< Buffersize;i++){
		TestBuffer[i] = i;
	}
	Blocksize = 28;
	assert((s_DataBuffer.WriteIndex == 0));
	GenerateDataBlock(Blocksize, TestBuffer);
	assert((s_DataBuffer.WriteIndex == 14));
	assert((ReadBuffer(&s_ControlBuffer) == 0x8888) && (ReadBuffer(&s_ControlBuffer) == 0x8888) && (ReadBuffer(&s_ControlBuffer) == 0));
	for(int i = 0; i< Buffersize;i++){
		assert((ReadBuffer(&s_DataBuffer) == i));
	}
	EndFunctionTestPrint("GenerateDataBlock");
	return;
}


void ReadBufferTest(){
	StartFunctionTestPrint("ReadBuffer");
	u16 returnvalue;
	struct BufferType s_Testbuffer;
	for(int i = 0;i <20 ;i++){
		s_Testbuffer.Data[i] = i+10;
	}
	ResetBuffer(&s_Testbuffer);

	returnvalue = ReadBuffer(&s_Testbuffer);
	assert(returnvalue == 0);
	s_Testbuffer.WriteIndex = 10;

	for(int i = 0; i<10;i++){
		assert((s_Testbuffer.ReadIndex == i) && (ReadBuffer(&s_Testbuffer) == (i+10)) && (s_Testbuffer.ReadIndex == i+1));
	}
	//WRiteindex should equal RedaIndex
	assert((ReadBuffer(&s_Testbuffer) == 0) && (s_Testbuffer.ReadIndex == 10));
	assert((ReadBuffer(&s_Testbuffer) == 0) && (s_Testbuffer.ReadIndex == 10));

	//Test wrap around the buffer
	s_Testbuffer.ReadIndex = MAX_BUFFER_LENGTH-1;
	s_Testbuffer.WriteIndex = 0;
	returnvalue = ReadBuffer(&s_Testbuffer);
	assert((s_Testbuffer.ReadIndex == 0));

	EndFunctionTestPrint("ReadBuffer");
	return;
}

void WriteBufferTest(){
	StartFunctionTestPrint("WriteBuffer");
	struct BufferType s_Testbuffer;
	ResetBuffer(&s_Testbuffer);
	for(int i = 0;i< 20;i++){

		assert((s_Testbuffer.WriteIndex == i) && (WriteBuffer(i, &s_Testbuffer) == 0) && (s_Testbuffer.WriteIndex == i+1));
	}
	s_Testbuffer.WriteIndex = MAX_BUFFER_LENGTH-1;
	s_Testbuffer.ReadIndex = 1;
	assert((s_Testbuffer.WriteIndex == (MAX_BUFFER_LENGTH-1)) && (WriteBuffer(100, &s_Testbuffer) == 0) && (s_Testbuffer.WriteIndex == 0));
	assert((s_Testbuffer.WriteIndex == 0) && (WriteBuffer(200, &s_Testbuffer) == 1) && (s_Testbuffer.WriteIndex == 0));
	EndFunctionTestPrint("WriteBuffer");
	return;
}

void ResetBufferTest(){
	StartFunctionTestPrint("ResetBuffer");
	struct BufferType s_TestBuffer;
	ResetBuffer(&s_TestBuffer);
	assert((s_TestBuffer.WriteIndex == 0) && (s_TestBuffer.ReadIndex == 0));
	assert((s_TestBuffer.Empty == 1) && (s_TestBuffer.Full == 0));
	EndFunctionTestPrint("ResetBuffer");
	return;
}

void BufferTest(){
	StartFunctionTestPrint("BufferTest");
	struct BufferType s_TestBuffer;
	ResetBuffer(&s_TestBuffer);
	for (int i = 0;i < 100;i++){
		assert( (WriteBuffer(i, &s_TestBuffer) == 0) && (BufferEmpty(&s_TestBuffer)==0) && (ReadBuffer(&s_TestBuffer) == i) &&(s_TestBuffer.Empty == 1) && (s_TestBuffer.ReadIndex == s_TestBuffer.WriteIndex));
	}
	assert(BufferEmpty(&s_TestBuffer) == 1);
	s_TestBuffer.WriteIndex = 50;
	s_TestBuffer.ReadIndex = 52;
	//WriteBuffer(0x4444, &s_TestBuffer);
	assert((WriteBuffer(0x4444, &s_TestBuffer) == 0) && (s_TestBuffer.Full == 1));
	assert(BufferFull(&s_TestBuffer)== 1);

	assert((WriteBuffer(0x4444, &s_TestBuffer) == 1) && (s_TestBuffer.Full == 1));
	ReadBuffer(&s_TestBuffer);
	assert(s_TestBuffer.Full == 0);
	assert(BufferFull(&s_TestBuffer) == 0);

	EndFunctionTestPrint("BufferTest");
	return;
}

void InitTradeBufferTest(){
	StartFunctionTestPrint("InitTradeBuffer");
	extern struct BufferType s_DataBuffer;
	extern struct BufferType s_ControlBuffer;
	ResetBuffer(&s_ControlBuffer);
	ResetBuffer(&s_DataBuffer);
	u8 Spotnumber = 0;
	u16 Blocksize = 0x0014;
	u16 Databuffer[17] = {Blocksize,0x0081,0,0,0,0,0,     0xAABB,Spotnumber,0,0,0,0,0,0,0,0};
	u8 *Spotptr = &Spotnumber;

	InitTradeBuffer((void*)Spotptr);

	assert(ReadBuffer(&s_ControlBuffer) == 0XBBBB);
	assert(ReadBuffer(&s_ControlBuffer) == 0X8888);
	assert(ReadBuffer(&s_ControlBuffer) == 0X8888);

	for(int i = 0; i < 17;i++){
		assert(ReadBuffer(&s_DataBuffer) == Databuffer[i]);
	}

	EndFunctionTestPrint("InitTradeBuffer");
	return;
}

void SendTradeChoiceTest(){
	StartFunctionTestPrint("SendTradeChoice");
	u32 PLtoPSBuffer_Value = 0;
	u32 Inputdata = 0x8FFF;
	u16 returnvalue;
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	PLtoPSBuffer_Value = 0x22;
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	PLtoPSBuffer_Value = 0;
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	Inputdata = 0x1234;
	for(int i = 0;i < 100;i++){
		returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		//printf("value = %08x\n",returnvalue);
	}

	EndFunctionTestPrint("SendTradeChoice");
	return;
}

void ExitStateTest(){
	StartFunctionTestPrint("ExitState");
	TCP_Server_Init();
	u32 PLtoPSBuffer_Value = 0;
	u32 Inputdata = 0xB9A0;
	u16 returnvalue = 0;
	while (returnvalue != 0xB9A0){
		returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		//printf("value = %08x\n",returnvalue);
	}
	for(int i = 0;i < 10;i++){
			returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
			printf("value = %04x\n",returnvalue);
		}
	Inputdata = 0x8FFF;printf("MASTER HANDSHAKE\n");
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	printf("value = %04x\n",returnvalue);
	Inputdata = 0x1234;
	for(int i = 0;i < 9;i++){
		returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		printf("value = %04x\n",returnvalue);
	}
	Inputdata = 0x7890;
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	printf("value = %04x\n",returnvalue);
	Inputdata = 0x5FFF;printf("Check\n");
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);printf("value = %04x\n",returnvalue);
	Inputdata = 0xC;
	returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);printf("value = %04x\n",returnvalue);
	Inputdata = 0x1234;
	for(int i = 0;i < 9;i++){
		returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		printf("value = %04x\n",returnvalue);
	}
	while(returnvalue != 0x5FFF){
		returnvalue = MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		printf("value = %04x\n",returnvalue);
	}
	printf("Received 5fff\n");
	returnvalue = MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
	printf("value = %04x\n",returnvalue);
	for(int i = 0;i < 10;i++){
		returnvalue= MainTradeHandler(Inputdata,PLtoPSBuffer_Value);
		//printf("value = %04x\n",returnvalue);
	}

	EndFunctionTestPrint("ExitState");
}

void GenerateBlockRequestResponseTest(){
	StartFunctionTestPrint("GenerateBlockRequestResponseTest");
	extern struct BufferType s_DataBuffer;
	extern struct BufferType s_ControlBuffer;
	ResetBuffer(&s_ControlBuffer);
	ResetBuffer(&s_DataBuffer);
	BlockSizes Blocksize = BLOCK_REQ_SIZE_200;
	GenerateBlockRequestResponse(Blocksize);
	assert(ReadBuffer(&s_ControlBuffer) == 0x0000);
	assert(ReadBuffer(&s_ControlBuffer) == 0x0000);
	assert(ReadBuffer(&s_ControlBuffer) == 0xBBBB);
	assert(ReadBuffer(&s_ControlBuffer) == 0x0000);
	assert(ReadBuffer(&s_ControlBuffer) == 0x0000);
	for (int i = 0; i < 15;i++){
		assert(ReadBuffer(&s_ControlBuffer) == 0x8888);
	}
	EndFunctionTestPrint("GenerateBlockRequestResponseTest");
	return;
}


void TestTCPServer(){
	StartFunctionTestPrint("TestTCPServer");
	TCP_Server_Init();
	u16 TestTeam[6][50];
	for(int i = 0;i< 6;i++){
		for(int u = 0; u< 50;u++){
			TestTeam[i][u] = (i*50)+u;
		}
	}
	ThreadData TData;
	TData.Connect_ID = ExchangePokemonData;
	ExchangeIDTCP((void*) &TData);
	printf("result = %d\n",TData.result);

	SendTeamIndex(3);
	//printf("REC Team Index = %d\n",ReceiveTeamIndex());

	SendBufferTCP(TestTeam[1],sizeof(TestTeam[1]));
	SendBufferTCP(TestTeam[2],sizeof(TestTeam[2]));

	printf("Now receive new buffer\n\n");
	ReceiveBufferTCP(TestTeam[3],sizeof(TestTeam[2]));
	ReceiveBufferTCP(TestTeam[4],sizeof(TestTeam[2]));

	EndFunctionTestPrint("TestTCPServer");
	return;
}

void ExchangeBufferTCPTest(){
	StartFunctionTestPrint("ExchangeBufferTCPTest");
	TCP_Server_Init();
	u16 Testbuffer[50];
	u16 TestbufferSize = sizeof(Testbuffer); printf("Testbuffersize = %d\n",TestbufferSize);
	for(int i = 0;i < TestbufferSize/2;i++){
		Testbuffer[i] = i;
	}
	ThreadData TData;
	TData.Connect_ID = ExchangeTrainerData;
	TData.ArgBufferSize =TestbufferSize;
	TData.ArgBufferPtr = Testbuffer;
	extern u16 ReceiveBuffer[110];
	ExchangeBufferTCP((void *)&TData);


	EndFunctionTestPrint("ExchangeBufferTCPTest");
	return;
}



void RunRingbufferTests(){
	ResetBufferTest();
	ReadBufferTest();
	WriteBufferTest();
	BufferTest();
	return;
}
void RunAllTests(){
//	ExitStateTest();
//
//	ConCatTest();
//	UpdateFramecounterTest();
//	ButtonPressTest();
//	RunRingbufferTests();
//
//	GenerateBlockInitTest();
//
//	GenerateDataBlockTest();
//
//	InitTradeBufferTest();
//
//	SendTradeChoiceTest();
//
//	GenerateBlockRequestResponseTest();

	//TestTCPServer();

	ExchangeBufferTCPTest();
	//main_TradeHandlerTest();
	return;
}