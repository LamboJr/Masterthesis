#ifndef DECODEPOKEMONSTRUCTURE_HEADER
#define DECODEPOKEMONSTRUCTURE_HEADER


#define FrameMask 0xFFFF0000

#define DEBUG_PRINT

void decode_Pokemon_data(u16 *Pokemonbuffer);
struct Pokemon PrintDecodedPokemonData(u16 *Pokemonbuffer);
#endif