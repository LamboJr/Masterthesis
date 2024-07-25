#ifndef GENERATE_DATA_STRUCTURE_HEADER
#define GENERATE_DATA_STRUCTURE_HEADER

//Functions for Creating Buffers out of the text files at Initialisation
void convertStructureintoBuffer(uint16_t * TransmitBuffer,size_t Buffersize,char *filename);

int generatePokemonStructure(char *filename, uint16_t *structure_addr);

//Functions for generation Buffers out of the Specs. from the GUI Process
int generatePokemonStructurefromGUI(uint16_t *structure_addr,struct PokemonStructureGUI *Info);

#endif