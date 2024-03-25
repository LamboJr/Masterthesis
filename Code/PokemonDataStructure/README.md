# Code
This folder contains source code which is used by multiple applications
## PokemonDataStructure
This folder contains header and soruce files written in the c language.
### decodeTrainer.c
This source file has functions to decode a u16 Array[50] into the trainer card structure
### Decoding.c
This soruce file has function to decode a u16 Array[50] into the corresponding Pokemon data structure
### pkmn_data_structure.c
This source file has function to create a u16 Array[50] out of specified pokemon attributes
This allows you to create your own Pokemon data structure depending on your wishes
### FileBasedGeneration.c.
This source file contains functions that create a u16 Array[50] out of Pokemon attributes speciefied in a text file
The format ist used from the pokemon showdown website


# Build
See the make file to compile the different programs. 

# instruction
## pkmn_data_structure
 Type ./gen --help for usage instructions

## FileBasedGeneration
use ./textgen to generate Pokemon data structure from textfiel data.txt