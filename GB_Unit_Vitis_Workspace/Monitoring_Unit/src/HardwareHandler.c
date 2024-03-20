#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Includes/DataTypes.h"
#include "Includes/Hardwarehandler.h"



void* InitPLtoPSBuffer(int fd){
	void *ptr;
	u32 addr = ADDRESS_PL_TO_PS;
	//unsigned page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);

//	page_addr=(addr & ~(page_size-1));
//	page_offset=addr-page_addr;
	ptr=mmap(NULL,page_size,PROT_READ,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		perror("mmap Error\n");
		exit(-1);
	}
	return ptr;
}

u32 ReadPltoPsBuffer(void *ptr){
	return *((unsigned *)(ptr));
}

void* InitPStoPLBuffer(int fd){
	void *ptr;
	u32 addr = ADDRESS_PS_TO_PL;
	//unsigned page_addr, page_offset;
	unsigned page_size=sysconf(_SC_PAGESIZE);
//	page_addr=(addr & ~(page_size-1));
//	page_offset=addr-page_addr;
	ptr=mmap(NULL,page_size,PROT_WRITE,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)ptr==-1) {
		perror("Error mmap WritePStoPl\n");
		exit(-1);
	}
	return ptr;
}
void WritePStoPLBffer(void *ptr,u32 value)
{
	*((unsigned *)(ptr))=value;
	return;
}


void WriteToRingbuffer(void *Ringbufferptr,u32 value,u32 addressoffset){
	u32 page_offset = addressoffset*4;
	*((u32 *)(Ringbufferptr+page_offset))=value;
	return;
}

u32 ReadRingbuffer(void *Ringbufferptr,u32 addressoffset){
	u32 page_offset = addressoffset*4;//Page offset is byte addressable. but Ringbuffer is only 32 bit adressable.
	return *((u32 *)(Ringbufferptr+page_offset));
}

void* InitRingbufferMMap(int fd){
	void *Ringbufferptr;
	u32 addr = BASE_ADDRESS_RINGBUFFER;
	//u32 page_addr, page_offset;
	u32 page_size=sysconf(_SC_PAGESIZE);
//	page_addr=(addr & ~(page_size-1));
//	page_offset=(addr-page_addr)*4;
	Ringbufferptr = mmap(NULL,page_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,(addr & ~(page_size-1)));
	if((int)Ringbufferptr==-1) {
		perror("Error mmap ReadtoRingbuffer =>");
		exit(-1);
	}
	return Ringbufferptr;
}
