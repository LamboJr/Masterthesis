#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>

#include <string.h>

#include "Includes/DataTypes.h"
#include "Hardwarehandler.h"
#include "Includes/TradeHandler.h"
#include "Testing/Test_Tradehanlder.h"
#include "Includes/ConCat.h"
#include "Includes/structs.h"
#include <pthread.h>
#include "Includes/DecodePokemonData.h"


#define MODE_TRADE 0



//#define USE_TEST_DATA

FILE *PokemonFp;


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

//------------------------------------------------------------------------------------------
//----------------------Exchange Platform--------------------------------------------------
//------------------------------------------------------------------------------------------


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

	return 0;
}
