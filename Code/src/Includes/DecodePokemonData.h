#ifndef DECODEPOKEMONSTRUCTURE_HEADER
#define DECODEPOKEMONSTRUCTURE_HEADER


#define FrameMask 0xFFFF0000

#define DEBUG_PRINT

void PrintDecodedPokemonDataToFile(u16 *Pokemonbuffer);
struct Pokemon PrintDecodedPokemonData(u16 *Pokemonbuffer);
#endif
