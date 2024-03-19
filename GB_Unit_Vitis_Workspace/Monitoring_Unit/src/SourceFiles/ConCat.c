#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Includes/DataTypes.h"

/*ConCat is a simple function to Concatinate two integer arrays to a bigger one
  The size of the resulting Array is sizeof(Array1) plus sizeof(Array2)
 */



void concat(u16 *Buffer1, u16 *Buffer2,u16 *ResultBuffer,size_t Buffer1size,size_t Buffer2size) {
	//printf("Size of Buffer 1: %d and Buffer2 : %d",Buffer1size,Buffer2size);
	memcpy(ResultBuffer,Buffer1,Buffer1size*sizeof(u16));
	memcpy(ResultBuffer+Buffer1size, Buffer2, Buffer2size*sizeof(u16));

	/*for (int i = 0; i< (Buffer1size+Buffer2size);i++){
		printf("Resultbuffer[%d] = %04x\n",i,ResultBuffer[i]);
	}*/
	//
	return;
}

