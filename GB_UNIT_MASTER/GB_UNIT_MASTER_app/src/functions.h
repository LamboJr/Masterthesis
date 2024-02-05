
#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include "Link.h"
#include <stdlib.h>
#include "structs.h"

u16 DecodeDataField(u16 checksum,u32 personality,u32 trainer_id,u32 *DataField);

struct Pokemon decode_Pokemon_structure(u16 *Pokemonbuffer);


#define CC(c , h) case (h): return c; break;

char char_conv( u8 character,u16 language);

#endif
