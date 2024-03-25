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
#include "Includes/MonitorTrade.h"
#include "Includes/TradeHandler.h"
#include "Includes/BackUpHandler.h"

#include "TCP_Server.h"

#include "Testing/Test_Tradehanlder.h"
#include "Testing/Test_BackupHandler.h"
#include "Includes/ConCat.h"
#include "Includes/structs.h"
#include <pthread.h>
#include "DatabaseHandler.h"
#include "Includes/DecodePokemonData.h"


#define MODE_TRADE 0
#define MODE_MONITOR 1

#define MODE_BACKUP 0
#define MODE_ETHERNET_TRADE 1

//#define USE_TEST_DATA

FILE *PokemonFp;

u8 TradeOPMode;

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




	while(!Valid){
	printf ("Enter Trading Mode (0) or Monitoring Mode (1) : ");
	scanf ("%d",&OPMode);
	printf(" OP Mode = %d\n",OPMode);
	if((OPMode == 0 )||( OPMode == 1)){
		printf("valid Value \n");
		Valid = 1;
	}else{
		printf("Invalid Value \n");
		Valid = 0;
	}
	}
	//Configure Trading or Monitoring mode
	u32 PsToPlValue = OPMode; // == Mode Trading
	printf("Configure Trading or Monitoring Mode\n");
	WritePStoPLBffer(PStoPLbufferptr,PsToPlValue);

	FILE *fp;

//--------------------------------------------------------------------------
//--------------------Monitoring Unit---------------------------------------
//--------------------------------------------------------------------------
	if (OPMode == MODE_MONITOR){
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
		//Clean buffer for clean start in communiccation
		printf("Clean Buffer \n");
		while( ReadRingbuffer(Ringbufferptr,1) == 0){
			Inputdata = ReadRingbuffer(Ringbufferptr,0);
		}
		printf("Finished cleaning buffer\n");

		printf("Staring Monitoring Program\n");
		printf("--------------------------------------------------------------------------\n--------------------Monitoring Unit---------------------------------------\n--------------------------------------------------------------------------");
		while(1){
			PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);
			if (ReadRingbuffer(Ringbufferptr,1) == 0){
				Inputdata = ReadRingbuffer(Ringbufferptr,0);
				MonitorHandler(Inputdata, dump,fp);
			}
		}
//------------------------------------------------------------------------------------------
//----------------------Exchange Platform--------------------------------------------------
//------------------------------------------------------------------------------------------
	}else{
		Valid = 0;
		while(!Valid){
			printf ("Enter BAckUp Mode (0) or Trade over IP Mode (1) : ");
			scanf ("%hhd",&TradeOPMode);
			printf("TradeOP Mode = %d\n",TradeOPMode);
			if((TradeOPMode == 0 )||( TradeOPMode == 1)){
				printf("valid Value \n");
				Valid = 1;
			}else{
				printf("Invalid Value \n");
				Valid = 0;
			}
		}

		PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);

		printf("Setting Output to 0x0000...\n");
		WriteToRingbuffer(Ringbufferptr,0x0000, 0);

		//Clean buffer for clean start in communiccation
		printf("Clean Buffer \n");
		while( ReadRingbuffer(Ringbufferptr,1) == 0){
			Inputdata = ReadRingbuffer(Ringbufferptr,0);
		}
		printf("Finished cleaning buffer\n");


		if (TradeOPMode == 0){//BackUp
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

		}else{//Tcp Trading
			printf("---------------------------------------------------------\n");
			printf("---------------TCP Trading-------------------------------\n");
			printf("---------------------------------------------------------\n");
			extern int __attribute__ ((unused)) new_socket;
			TCP_Server_Init();
			while(1){
				PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);
				if (ReadRingbuffer(Ringbufferptr,1) == 0){
					Inputdata = ReadRingbuffer(Ringbufferptr,0);
					WriteToRingbuffer(Ringbufferptr, MainTradeHandler(Inputdata,PLtoPSBuffer_Value), 0);
				}
			}



		}//end TradeMode
	}//end OPMODE

	return 0;
}