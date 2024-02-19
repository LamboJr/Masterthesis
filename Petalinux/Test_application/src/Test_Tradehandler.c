#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "Includes/DataTypes.h"

#include "Includes/TestDataTrade.h"
#include "Includes/TradeHandler.h"

#include "Includes/decode_Pokemon_structure.h"
#include "TCP_Server.h"

#define MODE_TRADE 0
#define MODE_MONITOR 1

#define TESTDATA_ARRAY TestDataTradePokemonExchange

extern FILE *PokemonFp;

int main_TradeHandlerTest()
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
		return 1;
	}
	// Write output to file
	//fprintf(fp, "This is the output that will be written to the file.\n");
	printf("Write Extracted Data into datalog.txt\n");
	PokemonFp = fopen("/home/petalinux/datalog.txt","w");
	if(PokemonFp == NULL){
		printf("Error opening file datalog.txt.\n");
		return 1;
	}


	printf("Staring Test Tradehanlder Program\n");

	u16 data;
	u16 returnvalue = 0;
	for(int i = 0; i < sizeof(TESTDATA_ARRAY);i++){
		data = (u16)((TESTDATA_ARRAY[i] >> 16) & 0xFFFF);
		returnvalue = TradeHandler(data,0x0);
	}
	extern u16 ReceivedTeam[6][50];
	for(int u = 0;u <6;u++){
		decode_Pokemon_data(ReceivedTeam[u]);
	}

	return 0;
}
