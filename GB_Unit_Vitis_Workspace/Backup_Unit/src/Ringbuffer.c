#include "Includes/DataTypes.h"
#include <stdio.h>

#include "Includes/Ringbuffer.h"

// DEclare a buffer structure for a Buffer with Data to be send to Ringbuffer of HW
struct BufferType s_DataBuffer;
// DEclare a buffer structure for a Buffer with Data to be send to Ringbuffer of HW
struct BufferType s_ControlBuffer;

// Reset bUFFER, buffer is specified in the arguments
void ResetBuffer(struct BufferType *Buffer){
	Buffer -> WriteIndex = 0;
	Buffer -> ReadIndex = 0;
	Buffer -> Empty = 1;
	Buffer -> Full = 0;
}
// Read next element in Ringbuffer, buffer is specified in the arguments
u16 ReadBuffer(struct BufferType *Buffer){
	u16 returnvalue = 0;
	Buffer->Full =0;
	//If Head == Tail Buffer is empty. return 0;
	if (Buffer->ReadIndex == Buffer->WriteIndex){
		return returnvalue;
	}else{
		returnvalue = Buffer->Data[Buffer->ReadIndex];
		if (Buffer->ReadIndex >= (MAX_BUFFER_LENGTH-1)){
			Buffer->ReadIndex = 0;
		}
		else{
			Buffer->ReadIndex++;
		}
		if (Buffer->ReadIndex == Buffer->WriteIndex){
			Buffer->Empty = 1;
		}else{
			Buffer->Empty = 0;
		}
		return returnvalue;
	}

}
// Write data to ringbuffer, buffer is specified in the arguments
s8 WriteBuffer(u16 datainput, struct BufferType *Buffer){
	Buffer->Empty = 0;
	if (Buffer->Full == 0){
		Buffer->Data[Buffer->WriteIndex] = datainput;
		if (Buffer->WriteIndex >= (MAX_BUFFER_LENGTH-1) ){
			Buffer->WriteIndex = 0;
		}
		else{
			Buffer->WriteIndex++;
		}
		//Check if full now
		if (Buffer->WriteIndex >= (MAX_BUFFER_LENGTH-1) ){
			if( Buffer->ReadIndex == 0){
				//Buffer Full
				Buffer->Full = 1;
			}
		}else{
			if ((Buffer->WriteIndex+1) == Buffer->ReadIndex){
				Buffer->Full = 1;
			}
		}
		return 0;
	}else{
		Buffer->Full = 1;
		printf("Buffer full now\n");
		return 1;
	}

}//Check if specified buffer is empty
u8 BufferEmpty(struct BufferType *Buffer){
	return Buffer->Empty;
	//Return 1 if Buffer empty
}
// Chekc if specifies Buffer is full
u8 BufferFull(struct BufferType *Buffer){
	return Buffer->Full;
}
