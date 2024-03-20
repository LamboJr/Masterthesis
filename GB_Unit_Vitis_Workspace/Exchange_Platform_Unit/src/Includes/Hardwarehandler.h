#define ADDRESS_PL_TO_PS 0x41200000

#define ADDRESS_PS_TO_PL 0x41210000

#define BASE_ADDRESS_RINGBUFFER 0x40000000
#define ADDRESS_OFFSET_FREERINGBUFFER 0x1



void* InitPLtoPSBuffer(int fd);
void* InitPStoPLBuffer(int fd);
void* InitRingbufferMMap(int fd);

u32 ReadPltoPsBuffer(void *ptr);

void WritePStoPLBffer(void *ptr,u32 value);

void WriteToRingbuffer(void *Ringbufferptr,u32 value,u32 addressoffset);
u32 ReadRingbuffer(void *Ringbufferptr,u32 addressoffset);

