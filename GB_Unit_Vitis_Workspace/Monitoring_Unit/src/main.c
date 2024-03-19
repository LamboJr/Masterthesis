#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#include "Includes/DataTypes.h"
#include "Hardwarehandler.h"
#include "Includes/MonitorTrade.h"

#include "Includes/structs.h"
#include "Includes/DecodePokemonData.h"
#include "Testing/MonitorUnitTest.h"

#define MODE_MONITOR 1


//#define UNIT_TEST

FILE *PokemonFp;


int main(int argc, char *argv[])
{

#ifdef UNIT_TEST

	RunAllMonitorTests();
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
	u8 OPMode = MODE_MONITOR;

	//Configure Trading or Monitoring mode
	u32 PsToPlValue = OPMode; // == Mode Trading
	printf("Configure HW to Monitoring Mode\n");
	WritePStoPLBffer(PStoPLbufferptr,PsToPlValue);

	FILE *fp;

//--------------------------------------------------------------------------
//--------------------Monitoring Unit---------------------------------------
//--------------------------------------------------------------------------
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
	printf("--------------------------------------------------------------------------\n");
	printf("--------------------Monitoring Unit---------------------------------------\n");
	printf("--------------------------------------------------------------------------\n");
	while(1){
		if (ReadRingbuffer(Ringbufferptr,1) == 0){
			Inputdata = ReadRingbuffer(Ringbufferptr,0);
			MonitorHandler(Inputdata, dump,fp);
		}
	}

	return 0;
}
