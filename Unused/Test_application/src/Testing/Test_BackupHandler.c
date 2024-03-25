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

#include "../TCP_Server.h"

#include "../Includes/Ringbuffer.h"

#include "../Includes/ConCat.h"
#include "../Includes/BackUpHandler.h"
#include "Test_Tradehanlder.h"
#include "Test_BackupHandler.h"
#include "../DatabaseHandler.h"
#include "../Includes/DecodePokemonData.h"


#define MODE_TRADE 0
#define MODE_MONITOR 1

#define TESTDATA_ARRAY TestDataTradePokemonExchange

extern FILE *PokemonFp;

extern u16 Magikarp_pokemonbuffer[50];
extern u16 Salamence_pokemonbuffer[50];
extern u16 Latios_pokemonbuffer[50];


void BackupTradeHandlerTest(){
	StartFunctionTestPrint(__func__);

	EndFunctionTestPrint(__func__);
	return;
};


void DataBaseTest(){
	char* TestTableName = "TestTable";
	ThreadData ThreadData;
	u16 Testbuffer[50];u16 TestReadBuffer[50];
	for (int i = 0;i <50;i++){
		Testbuffer[i] = i;
	}
	print_table_names();
	validDataBaseEntry(TestTableName);
	DataBaseInsertBuffer(TestTableName,Testbuffer);
	validDataBaseEntry(TestTableName);
	print_table_names() ;
	DataBaseReadBuffer(TestReadBuffer,TestTableName);

	for (int i = 0;i <50;i++){
			printf("%d ",TestReadBuffer[i]);
	}

	DataBaseDropTable(TestTableName);
	validDataBaseEntry(TestTableName);
	print_table_names() ;
	ThreadData.ArgBufferPtr = Latios_pokemonbuffer;
	for (int i = 0;i<50;i++){
		ThreadData.ArgBufferPtr2[i] = Latios_pokemonbuffer[i];
	}
	DataBaseInsertThread(&ThreadData);

}

void InitBlockAnalyserTest(){
	u32 Framecounter  =1 ;
	u16 TestDataField[]  = {
			 0x0000    ,0xBBBB    ,0x0014    ,0x0080    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0xBC4F    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x8888    ,0xCCDD    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x5565    ,0x8888    ,0x0000    ,0x0000    ,0x0000    ,0xFFFF    ,0xFFFF    ,0xFFFF    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
			,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000    ,0x0000
	};
	for (int i = 0; i < sizeof(TestDataField)/sizeof(u16);i++){
		if( InitBlockAnalyser(Framecounter, TestDataField[i]) == Granted){

		}else{

		}
		UpdateFrameCounter(&Framecounter);
	}
	extern TradeHandler s_TradeHandlerMaster;
	for (int i = 0; i < 10;i++){
		printf("%04x ",s_TradeHandlerMaster.TradeCommandBuffer[i]);
	}printf("\n");
	assert(s_TradeHandlerMaster.TradeCommandBuffer[0] == TestDataField[38]);
};


void RunBackUpHandlerTests(){
	printf("Start the test of BackupHandler functions\n");
	BackupTradeHandlerTest();
	InitBlockAnalyserTest();
	DataBaseTest();
	printf("Test successful\n");
	return;
}
