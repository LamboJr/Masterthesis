#ifndef DECODEPOKEMONSTRUCTURE_HEADER
#define DECODEPOKEMONSTRUCTURE_HEADER

#include "xil_types.h"
#include "functions.h"
#include "structs.h"
#include "xil_printf.h"


#define FrameMask 0xFFFF0000

#define DEBUG_PRINT

void decode_Pokemon_data(u16 *Pokemonbuffer);
#endif
