#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Includes/DataTypes.h"
#include "Includes/Hardwarehandler.h"


/*
 * Initialises the  AXI GPIO  Pl to PS
 * It returns a pointer which points to the Address of the Axi Component
 * Used to read and write data from/to this AXi component
 *
 */


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
//Read from PltoPs AXI GPIO
u32 ReadPltoPsBuffer(void *ptr){
	return *((unsigned *)(ptr));
}


/*
 * Initialises the AXI GPIO PS to Pl
 * It returns a pointer which points to the Address of the Axi Component
 * Used to read and write data from/to this AXi component
 *
 */

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
//Writes value to Ps to Pl AXI GPIO
void WritePStoPLBffer(void *ptr,u32 value)
{
	*((unsigned *)(ptr))=value;
	return;
}

//Wirtes Value to the Ringbuffer Ps to PL  with the adressoffset specified (doesn't matter now)
void WriteToRingbuffer(void *Ringbufferptr,u32 value,u32 addressoffset){
	u32 page_offset = addressoffset*4;
	*((u32 *)(Ringbufferptr+page_offset))=value;
	return;
}
// Reads from Ringbuffer PL to PS
//addressoffset = 1 -> Reads teh empty flag of the ringbuffer
//addressoffset = 0 -> Redas the actual data currently present at the ringbuffer output
u32 ReadRingbuffer(void *Ringbufferptr,u32 addressoffset){
	u32 page_offset = addressoffset*4;//Page offset is byte addressable. but Ringbuffer is only 32 bit addressable.
	return *((u32 *)(Ringbufferptr+page_offset));
}

/*
 * Initialises the AXI BRAM controller component and returns a pointer for the corresponding address space
 * Used for Writing to ringbuffer PS to PL
 * and reading from ringbuffer Pl to PS
 */

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
