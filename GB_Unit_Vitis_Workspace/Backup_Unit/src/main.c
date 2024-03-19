#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <string.h>

#include "Includes/DataTypes.h"
#include "Hardwarehandler.h"
#include "Includes/Link.h"
#include "Includes/TradeHandler.h"
#include "Includes/BackUpHandler.h"


#include "Testing/Test_BackupHandler.h"
#include "Includes/ConCat.h"
#include "Includes/structs.h"
#include <pthread.h>
#include "DatabaseHandler.h"
#include "Includes/DecodePokemonData.h"


#define MODE_TRADE 0

//#define USE_TEST_DATA

FILE *PokemonFp;

char ChoosePokemon[100];

int main(int argc, char *argv[])
{

#ifdef USE_TEST_DATA

	//RunAllTests();
	RunBackUpHandlerTests();
	printf("Tests finished\n");
	return 1;
#endif

//---------------------------------------------------------------
//-----Initialize Hardware components------------------------------

	printf("Initialize Hardware components ... \n");
	int fd;
	fd=open("/dev/mem",O_RDWR);
		if(fd<1) {
			perror("Error open FD \n");
			exit(-1);
	}

	//Get Memory Pointer
	void *Ringbufferptr = InitRingbufferMMap(fd);
	void *PltoPSbufferptr = InitPLtoPSBuffer(fd);
	void *PStoPLbufferptr = InitPStoPLBuffer(fd);

	printf("Finished\n");

	//Declare Variables
	u32 Inputdata = 0;
	u32 dump = 1;
	u32 PLtoPSBuffer_Value;
	u32 OPMode = MODE_TRADE;
	bool Valid = 0;

	//Configure Trading or Monitoring mode
	u32 PsToPlValue = OPMode; // == Mode Trading
	printf("Configure Trading or Monitoring Mode\n");
	WritePStoPLBffer(PStoPLbufferptr,PsToPlValue);

	FILE *fp;


	PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);

	printf("Setting Output to 0x0000...\n");
	WriteToRingbuffer(Ringbufferptr,0x0000, 0);

	//Clean buffer for clean start in communiccation
	printf("Clean Buffer \n");
	while( ReadRingbuffer(Ringbufferptr,1) == 0){
		Inputdata = ReadRingbuffer(Ringbufferptr,0);
	}
	printf("Finished cleaning buffer\n");


	printf("---------------------------------------------------------\n");
	printf("---------------BackUp Mode-------------------------------\n");
	printf("---------------------------------------------------------\n");

	extern u16 NoPokemon[50];
	extern u16 Magikarp_pokemonbuffer[50];
	u8 StartBackupProcess = 0;

	u16 DBReadBuffer[50];
	for(int i = 0;i< 50;i++){
		DBReadBuffer[i] = NoPokemon[i];
	}


	//DataBaseInsertBuffer("Brutalanda", Salamence_pokemonbuffer);


	while (StartBackupProcess == 0){
		print_table_names();
		printf("---------------------------------------------------------\n");
		printf("Choose Pokemon \n");
		printf("---------------------------------------------------------\n");
		scanf("%s",ChoosePokemon);
		while (validDataBaseEntry(ChoosePokemon) == 1){
			print_table_names();
			printf("Choose Pokemon \n");
			scanf("%s",ChoosePokemon);
		}

		DataBaseReadBuffer(DBReadBuffer, ChoosePokemon);
		for(int i = 1;i<= 50;i++){
			if((i%10) == 0){printf("\n");}
			printf("%04x ",DBReadBuffer[i-1]);
		}
		PrintDecodedPokemonData(DBReadBuffer);
		printf("---------------------------------------------------------\n");
		printf("Want to load this Pokemon into GameBoy ? yes/no \n");
		printf("---------------------------------------------------------\n");
		char Confirm[10];
		scanf("%s",Confirm);
		if (strcmp("yes",Confirm) == 0){
			StartBackupProcess = 1;
		}else{
			StartBackupProcess = 0;
		}
	}
	printf("\n Loaded from database\n");

	//extern TradeHandler s_TradeHandlerMaster;
	extern u16 PokemonTeamBuffer[3][100];
	size_t Buffersize = sizeof(Magikarp_pokemonbuffer)/sizeof(u16);
	//printf("Size of : %d\n",Buffersize);
	concat(Magikarp_pokemonbuffer,DBReadBuffer, PokemonTeamBuffer[0],Buffersize,Buffersize);
	concat(NoPokemon,NoPokemon, PokemonTeamBuffer[1],Buffersize,Buffersize);
	concat(NoPokemon,NoPokemon, PokemonTeamBuffer[2],Buffersize,Buffersize);
	while(1){
		PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);
		if (ReadRingbuffer(Ringbufferptr,1) == 0){
			Inputdata = ReadRingbuffer(Ringbufferptr,0);
			WriteToRingbuffer(Ringbufferptr, BackupTradeHandler(Inputdata,PLtoPSBuffer_Value), 0);
		}
	}

	return 0;
}