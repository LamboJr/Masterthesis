#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


#define ADDRESS_PL_TO_PS 0x41200000

#define ADDRESS_PS_TO_PL 0x41210000

#define BASE_ADDRESS_RINGBUFFER 0x40000000
#define ADDRESS_OFFSET_FREERINGBUFFER 0x1

void usage(char *prog)
{
	printf("usage: %s ADDR\n",prog);
	printf("\n");
	printf("ADDR may be specified as hex values\n");
}

uint32_t ReadPltoPsBuffer(int fd){
//	;
	void *ptr;
	uint32_t addr = ADDRESS_PL_TO_PS;
	unsigned page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	page_addr=(addr & ~(page_size-1));
	page_offset=addr-page_addr;
	ptr=mmap(NULL,page_size,PROT_READ,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		perror("mmap Error\n");
		exit(-1);
	}
	//printf("0x%08x\n",*((unsigned *)(ptr+page_offset)));
	return *((unsigned *)(ptr+page_offset));

}

void WritePStoPLBffer(uint32_t value,int fd)
{
	void *ptr;
	uint32_t addr = ADDRESS_PS_TO_PL;
	unsigned page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);

	page_addr=(addr & ~(page_size-1));
	page_offset=addr-page_addr;

	ptr=mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		perror("Error mmap WritePStoPl\n");
		exit(-1);
	}

	*((unsigned *)(ptr+page_offset))=value;
	return ;
}


void WriteToRingbuffer(void *Ringbufferptr,uint32_t value,uint32_t addressoffset){
	uint32_t page_offset = addressoffset*4;
	*((uint32_t *)(Ringbufferptr+page_offset))=value;
	return;
}

uint32_t ReadRingbuffer(void *Ringbufferptr,uint32_t addressoffset){
	uint32_t page_offset = addressoffset*4;//Page offset is byte addressable. but Ringbuffer is only 32 bit adressable.
	return *((uint32_t *)(Ringbufferptr+page_offset));
}

void* InitRingbufferMMap(int fd){
	void *Ringbufferptr;
	uint32_t addr = BASE_ADDRESS_RINGBUFFER;
	uint32_t page_addr, page_offset;
	uint32_t page_size=sysconf(_SC_PAGESIZE);
//	page_addr=(addr & ~(page_size-1));
//	page_offset=(addr-page_addr)*4;
	Ringbufferptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)Ringbufferptr==-1) {
		perror("Error mmap ReadtoRingbuffer =>");
		exit(-1);
	}
	return Ringbufferptr;
}

int main(int argc, char *argv[])
{
	int fd;
	fd=open("/dev/mem",O_RDWR);
		if(fd<1) {
			perror("Error open FD \n");
			exit(-1);
	}

	uint32_t PsToPlValue = 0x00000000; // == Mode Trading
	uint32_t OutputData = 0x0000;
	uint32_t Inputdata;

	uint32_t PLtoPSBuffer_Value = ReadPltoPsBuffer(fd);
	printf("PltoPSBuffer Value : %08x\n",PLtoPSBuffer_Value);

	WritePStoPLBffer(PsToPlValue,fd);

	void *Ringbufferptr = InitRingbufferMMap(fd);


	while(1){
		if (ReadRingbuffer(Ringbufferptr,1) == 0){
			Inputdata = ReadRingbuffer(Ringbufferptr,0);
			WriteToRingbuffer(Ringbufferptr,Inputdata,0);
			//printf("Ringbuffer data : %04x\n",Inputdata);
		}
	}

//	while(1){
//		if (ReadfromRingbuffer(fd, ADDRESS_OFFSET_FREERINGBUFFER) == 0){
//
//			Inputdata = ReadfromRingbuffer(fd,0);
//			printf(" => Ringbuffer data : %04x\n",Inputdata);
//		}
//
//	}


//	for(int i = 0;i < 3000;i++){
//		//Inputdata = ReadfromRingbuffer(fd,0);
//		printf("Empty : %08x => Ringbuffer data : %08x\n",ReadfromRingbuffer(fd,1),ReadfromRingbuffer(fd,0));
//	}

	return 0;
}
