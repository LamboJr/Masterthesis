#ifndef C_CODE_H
#define C_CODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MainCHeader.h"
void c_function(const char* text, int value);

void createTransmitbuffer(uint16_t BufferIndex, struct PokemonStructureGUI* TransferStructurePtr);

void InitTransmitBuffer(void);

//int main_C_function(struct PokemonStructureGUI* TransferStructurePtr);
int main_C_function(void);
#ifdef __cplusplus
}
#endif

#endif // C_CODE_H
