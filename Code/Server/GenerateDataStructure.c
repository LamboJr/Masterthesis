#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>

#

#include "PokemonHeader/Pokemonbuffer.h"

#include "GenerateDataStructure.h"


void generateBufferStructure(uint16_t * TransmitBuffer,size_t Buffersize,uint8_t TeamIndex){
    
    switch (TeamIndex)
    {
    case 1:{}
    break;
    case 2:{}
    break;
    case 3:{}
    break;
    case 4:{}
    break;
    case 5:{}
    break;
    case 6:{}
    break;    
    default:{
        printf("Wrong TeamIndex specifed\n");
    }break;
    }

    uint16_t *PokemonBufferPtr;

    for(int i = 0; i < (Buffersize/2) ;i++){

        TransmitBuffer[i] = Metagross_pokemonbuffer[i];
    }

    return;
}

