#include "../Includes/Link.h"
#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/structs.h"

#include "../Includes/functions.h"

u16 DecodeDataField(u16 checksum,u32 personality,u32 trainer_id,u32 *DataField);

struct Pokemon decode_Pokemon_structure(u16 *Pokemonbuffer);

#define CC(c , h) case (h): return c; break;
// Coverts the ASCII Format into Pokemon Letter format
char char_conv( u8 character,u16 language){
	if(language != LANGUAGE_GERMAN){return 1;}
		switch(character){
	    CC(' ', 0x00);
	    CC('&', 0x2d);
	    CC('+', 0x2e);
	    CC('=', 0x35);
	    CC('%', 0x5b);
	    CC('(', 0x5c);
	    CC(')', 0x5d);
	    CC('0', 0xa1);
	    CC('1', 0xa2);
	    CC('2', 0xa3);
	    CC('3', 0xa4);
	    CC('4', 0xa5);
	    CC('5', 0xa6);
	    CC('6', 0xa7);
	    CC('7', 0xa8);
	    CC('8', 0xa9);
	    CC('9', 0xaa);
	    CC('!', 0xab);
	    CC('?', 0xac);
	    CC('.', 0xad);
	    CC('-', 0xae);
	    CC(',', 0xb8);
	    CC('/', 0xba);
	    CC('A', 0xbb);
	    CC('B', 0xbc);
	    CC('C', 0xbd);
	    CC('D', 0xbe);
	    CC('E', 0xbf);
	    CC('F', 0xc0);
	    CC('G', 0xc1);
	    CC('H', 0xc2);
	    CC('I', 0xc3);
	    CC('J', 0xc4);
	    CC('K', 0xc5);
	    CC('L', 0xc6);
	    CC('M', 0xc7);
	    CC('N', 0xc8);
	    CC('O', 0xc9);
	    CC('P', 0xca);
	    CC('Q', 0xcb);
	    CC('R', 0xcc);
	    CC('S', 0xcd);
	    CC('T', 0xce);
	    CC('U', 0xcf);
	    CC('V', 0xd0);
	    CC('W', 0xd1);
	    CC('X', 0xd2);
	    CC('Y', 0xd3);
	    CC('Z', 0xd4);
	    CC('a', 0xd5);
	    CC('b', 0xd6);
	    CC('c', 0xd7);
	    CC('d', 0xd8);
	    CC('e', 0xd9);
	    CC('f', 0xda);
	    CC('g', 0xdb);
	    CC('h', 0xdc);
	    CC('i', 0xdd);
	    CC('j', 0xde);
	    CC('k', 0xdf);
	    CC('l', 0xe0);
	    CC('m', 0xe1);
	    CC('n', 0xe2);
	    CC('o', 0xe3);
	    CC('p', 0xe4);
	    CC('q', 0xe5);
	    CC('r', 0xe6);
	    CC('s', 0xe7);
	    CC('t', 0xe8);
	    CC('u', 0xe9);
	    CC('v', 0xea);
	    CC('w', 0xeb);
	    CC('x', 0xec);
	    CC('y', 0xed);
	    CC('z', 0xee);
	    CC(':', 0xf0);
	    CC('\n', 0xfe);
	    CC('\0', 0xff);
		default:return 0;
		}
}
#undef CC

u16 DecodeDataField(u16 checksum,u32 personality,u32 trainer_id,u32 *DataField){

	u32 order = personality % 24;
    // Find data ordering
    /*
    00. GAEM	06. AGEM	12. EGAM	18. MGAE
    01. GAME	07. AGME	13. EGMA	19. MGEA
    02. GEAM	08. AEGM	14. EAGM	20. MAGE
    03. GEMA	09. AEMG	15. EAMG	21. MAEG
    04. GMAE	10. AMGE	16. EMGA	22. MEGA
    05. GMEA	11. AMEG	17. EMAG	23. MEAG
    */
    //Decrypt data
    u32 key = personality ^ trainer_id;

    for(size_t i = 0; i < (DATA_LENGTH / sizeof(u32)); i++) {
        DataField[i] ^= key;
    }
    //Calc checksum
    u16 cksum = 0;
    for(size_t i = 0; i < (DATA_LENGTH / sizeof(u16)); i++) {
        cksum += ((u16 *) DataField)[i];
    }
    //Setting the variables for ordering teh datafield
    u8 growthslot,attacksslot,miscslot,conditionslot;

    switch(order) {
    case 0 ... 5:
        growthslot = 0;
        break;
    case 6 ... 11:
        attacksslot = 0;
        break;
    case 12 ... 17:
        conditionslot = 0;
        break;
    case 18 ...23 :
        miscslot = 0;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 1:
    case 14:
    case 15:
    case 20:
    case 21:
        attacksslot = 1;
        break;
    case 2:
    case 3:
    case 8:
    case 9:
    case 22:
    case 23:
        conditionslot = 1;
        break;
    case 4:
    case 5:
    case 10:
    case 11:
    case 16:
    case 17:
        miscslot = 1;
        break;
    case 6:
    case 7:
    case 12:
    case 13:
    case 18:
    case 19:
        growthslot = 1;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 5:
    case 6:
    case 11:
    case 19:
    case 21:
        conditionslot = 2;
        break;
    case 1:
    case 3:
    case 7:
    case 9:
    case 13:
    case 15:
        miscslot = 2;
        break;
    case 2:
    case 4:
    case 12:
    case 17:
    case 18:
    case 23:
        attacksslot = 2;
        break;
    case 8:
    case 10:
    case 14:
    case 16:
    case 20:
    case 22:
        growthslot = 2;
        break;
    default: abort();
    };

    switch(order) {
    case 0:
    case 2:
    case 6:
    case 8:
    case 12:
    case 14:
        miscslot = 3;
        break;
    case 1:
    case 4:
    case 7:
    case 10:
    case 18:
    case 20:
        conditionslot = 3;
        break;
    case 3:
    case 5:
    case 13:
    case 16:
    case 19:
    case 22:
        attacksslot = 3;
        break;
    case 9:
    case 11:
    case 15:
    case 17:
    case 21:
    case 23:
        growthslot = 3;
        break;
    default: abort();
    };
    //Array for stroing the sorted data temporary
    u32 DataSorted32[DATUM_PER_DATA][DATUM_LENGTH / sizeof(u32)];

    for(size_t i = 0;i < DATUM_LENGTH/sizeof(u32);i++){
        DataSorted32[0][i] = DataField[i+(growthslot * DATUM_LENGTH/sizeof(u32))];
        DataSorted32[1][i] = DataField[i+(attacksslot * DATUM_LENGTH/sizeof(u32))];
        DataSorted32[2][i] = DataField[i+(conditionslot * DATUM_LENGTH/sizeof(u32))];
        DataSorted32[3][i] = DataField[i+(miscslot * DATUM_LENGTH/sizeof(u32))];
        //printf("DataSorted[0][%ld] = %04x\n",i,DataSorted[0][i]);
    }
    for(size_t i = 0; i < DATUM_PER_DATA;i++){
        for( size_t u = 0;u< (DATUM_LENGTH / sizeof(u32));u++){
            DataField[((DATUM_LENGTH / sizeof(u32))*i)+u] = DataSorted32[i][u];
            //printf("Datafield debug [%ld] : %08x\n",((DATUM_LENGTH / sizeof(u32))*i)+u,DataField[((DATUM_LENGTH / sizeof(u32))*i)+u]);
        }
    }
    return 0;
}


struct Pokemon decode_Pokemon_structure(u16 *Pokemonbuffer){
	struct Pokemon Pokemonstruct;
    u16* PokemonPtr = (u16* )&Pokemonstruct;
    //printf("Starraddresse Struct : %p\n",PokemonPtr);
    for (size_t i = 0;i <POKEMON_BUFFER_SIZE;i++){
        *PokemonPtr = Pokemonbuffer[i];
        //printf("Pokemonstruct addr : %p Value = %04x\n",PokemonPtr,*PokemonPtr);
        PokemonPtr++;
    }
    for(size_t i = 0; i < NICKNAME_LENGTH;i++){
        Pokemonstruct.nickname[i] = char_conv(Pokemonstruct.nickname[i],LANGUAGE_GERMAN);
    }
    for(size_t i = 0;i<TRAINER_NAME_LENGTH;i++){
        Pokemonstruct.trainer_name[i] = char_conv(Pokemonstruct.trainer_name[i],LANGUAGE_GERMAN);
    }
    u32* DataField = (u32* )Pokemonstruct.data;
    DecodeDataField(Pokemonstruct.checksum,Pokemonstruct.personality,Pokemonstruct.trainer_id,DataField);

    return Pokemonstruct;
}



