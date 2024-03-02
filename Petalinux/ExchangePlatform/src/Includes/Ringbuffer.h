#ifndef RINGBUFFER_HEADER
#define RINGBUFFER_HEADER

#define MAX_BUFFER_LENGTH 1024


struct BufferType{
	u16 Data[MAX_BUFFER_LENGTH];
	u16 WriteIndex;
	u16 ReadIndex;
	u8 Empty;
	u8 Full;
};

void ResetBuffer(struct BufferType *Buffer);
u16 ReadBuffer(struct BufferType *Buffer);
s8 WriteBuffer(u16 datainput, struct BufferType *Buffer);

u8 BufferEmpty(struct BufferType *Buffer);

u8 BufferFull(struct BufferType *Buffer);

#endif
