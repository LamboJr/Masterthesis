#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <arpa/inet.h>

#include "Includes/DataTypes.h"
#include "Hardwarehandler.h"
#include "Includes/MonitorTrade.h"
#include "Includes/TradeHandler.h"

#include "TCP_Server.h"

#include "Test_Tradehanlder.h"

#define MODE_TRADE 0
#define MODE_MONITOR 1

//#define USE_TEST_DATA

FILE *PokemonFp;

int main(int argc, char *argv[])
{

#ifdef USE_TEST_DATA
	main_TradeHandlerTest();
	printf("Test finished\n");
	return 1;
#endif

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

	}

	//u32 OutputData = 0x0000;
	u32 Inputdata = 0;
	u32 dump = 1;
	u32 PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);

	printf("Setting Output to 0x0000...\n");
	WriteToRingbuffer(Ringbufferptr,0x0000, 0);

	extern int new_socket;
	TCP_Server_Init();



	//Clean buffer for clean start in communiccation
	printf("Clean Buffer \n");
	while( ReadRingbuffer(Ringbufferptr,1) == 0){
		Inputdata = ReadRingbuffer(Ringbufferptr,0);
	}
	printf("Finished cleaning buffer\n");

	printf("Staring Main Program\n");


	while(1){
		PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);

		if (ReadRingbuffer(Ringbufferptr,1) == 0){

			Inputdata = ReadRingbuffer(Ringbufferptr,0);
			if (OPMode == MODE_TRADE){

				WriteToRingbuffer(Ringbufferptr, TradeHandler(Inputdata,PLtoPSBuffer_Value), 0);
			}else{
				MonitorHandler(Inputdata, dump,fp);
			}
		}
	}

	return 0;
}
