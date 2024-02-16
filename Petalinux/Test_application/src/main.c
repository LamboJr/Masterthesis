#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Includes/DataTypes.h"
#include "Hardwarehandler.h"
#include "Includes/MonitorTrade.h"
#include "Includes/TradeHandler.h"

#include "TCP_Server.h"

#define MODE_TRADE 0
#define MODE_MONITOR 1

#define USE_TEST_DATA


int main(int argc, char *argv[])
{
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
	u32 PsToPlValue = MODE_TRADE; // == Mode Trading
	u32 OutputData = 0x0000;
	u32 Inputdata;
	u32 dump = 1;
	u32 PLtoPSBuffer_Value = ReadPltoPsBuffer(PltoPSbufferptr);

	printf("Setting to Output to 0x0000...\n");
	WriteToRingbuffer(Ringbufferptr,0x0000, 0);

	TCP_Server_Init();
	//Configure Trading or Monitoring mode
	printf("Configure Trading or Monitoring Mode\n");
	WritePStoPLBffer(PStoPLbufferptr,PsToPlValue);


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
				MonitorHandler(Inputdata, dump);
			}
		}
	}

	return 0;
}
