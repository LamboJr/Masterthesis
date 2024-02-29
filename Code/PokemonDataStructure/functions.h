#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "structs.h"
#include "constants.h"
#include <math.h>


uint16_t calc_HP_stat (uint16_t HP_EV, uint16_t HP_IV, uint16_t HP_BASE_STAT, uint16_t LEVEL){
  uint16_t EV_GAIN = floor(HP_EV/4);
  uint16_t temp1 = (2* HP_BASE_STAT) + HP_IV + EV_GAIN;
  uint16_t temp2 = floor(temp1 * LEVEL / 100);
  return temp2 + LEVEL + 10;
};

uint16_t calc_Stat_Value (uint16_t EV_VALUE, uint16_t IV_VALUE, uint16_t BASE_STAT, uint16_t LEVEL, float_t NATURE ){
  uint16_t EV_GAIN = floor(EV_VALUE/4);
  uint16_t temp1 = (2*BASE_STAT) + IV_VALUE + EV_GAIN;
  uint16_t temp2 = floor((temp1*LEVEL/100)+5);
  //printf("%d %d %d %f\n",EV_GAIN,temp1,temp2,floor(temp2 * NATURE));
  return floor(temp2 * NATURE);
};
void removeLastSpace(char *str) {
    int len = strlen(str);
    
    if (len == 0) {
        return;  // Empty string
    }
    
    // Starting from the last character, find the last space
    for (int i = len - 1; i >= 0; i--) {
        if (str[i] == ' ') {
            str[i] = '\0';  // Replace the space with null terminator
            
        }
        else{
          return;
        }
    }
}




// Encrypt the pokémon structures together
uint16_t data_encrypt_to(
  void *dest,
  uint32_t personality,
  uint32_t trainer_id,
  struct Growth growth,
  struct Attacks attacks,
  struct Condition condition,
  struct Misc misc
) {
  uint8_t buf[DATA_LENGTH];
  void *datum[DATUM_PER_DATA];
  uint32_t order = personality % 24;

  // Find data ordering
  switch(order) {
  case 0 ... 5:
    datum[0] = &growth;
    break;
  case 6 ... 11:
    datum[0] = &attacks;
    break;
  case 12 ... 17:
    datum[0] = &condition;
    break;
  case 18 ...23:
    datum[0] = &misc;
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
    datum[1] = &attacks;
    break;
  case 2:
  case 3:
  case 8:
  case 9:
  case 22:
  case 23:
    datum[1] = &condition;
    break;
  case 4:
  case 5:
  case 10:
  case 11:
  case 16:
  case 17:
    datum[1] = &misc;
    break;
  case 6:
  case 7:
  case 12:
  case 13:
  case 18:
  case 19:
    datum[1] = &growth;
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
    datum[2] = &condition;
    break;
  case 1:
  case 3:
  case 7:
  case 9:
  case 13:
  case 15:
    datum[2] = &misc;
    break;
  case 2:
  case 4:
  case 12:
  case 17:
  case 18:
  case 23:
    datum[2] = &attacks;
    break;
  case 8:
  case 10:
  case 14:
  case 16:
  case 20:
  case 22:
    datum[2] = &growth;
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
    datum[3] = &misc;
    break;
  case 1:
  case 4:
  case 7:
  case 10:
  case 18:
  case 20:
    datum[3] = &condition;
    break;
  case 3:
  case 5:
  case 13:
  case 16:
  case 19:
  case 22:
    datum[3] = &attacks;
    break;
  case 9:
  case 11:
  case 15:
  case 17:
  case 21:
  case 23:
    datum[3] = &growth;
    break;
  default: abort();
  };

  // Assemble data
  for(size_t i = 0; i < DATUM_PER_DATA; i++) {
    memcpy(buf + (DATUM_LENGTH * i), datum[i], DATUM_LENGTH);
  }

  // Checksum data
  uint16_t cksum = 0;
  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint16_t)); i++) {
    cksum += ((uint16_t *) buf)[i];
  }

  // Encrypt data
  uint32_t key = personality ^ trainer_id;
  for(size_t i = 0; i < (DATA_LENGTH / sizeof(uint32_t)); i++) {
    ((uint32_t *) buf)[i] ^= key;
  }
  memcpy(dest, buf, DATA_LENGTH);
  return cksum;
}

//hexdump(&pkmn, sizeof(struct Pokemon), 0x03004360 + 100);
void hexdump_VHDL(void *addr, size_t len,char* PKMN_NAME) {
  uint16_t *_addr = (uint16_t *) addr;
  //printf("Debug %d \n",addr);
  //printf("Debug %d \n",len);
  char ascii_buf[17];
  size_t i;
  printf("constant %s_buffer : Pokemon_structure_buffer_type := (\n",PKMN_NAME);
  for(i = 0; i < len/2; ++i) {
    if((i % 8) == 0) {
      if(i != 0) {
        printf("\n");
      }
    }
    if (i == 49){
      printf(" x\"%04x\"", _addr[i]);
    }
    else{
      printf(" x\"%04x\",", _addr[i]);}
  };
  printf(");\n");
}

/*	u32 pokemonbuffer[50] = {0x0002, 0x41f3, 0x4753, 0x06a6, 0xedc5, 0xdbe3, 0xd9e6, 0xffff,
			 0xffff, 0x0205, 0xd5c6, 0xd6e1, 0xffe3, 0x00ff, 0x1089, 0x0000,
			 0x4706, 0x4616, 0x476b, 0x47e3, 0x4d5b, 0x4d5f, 0xb851, 0x2693,
			 0xbb4e, 0x78aa, 0x8e75, 0xc75a, 0x46c5, 0x4630, 0x5581, 0x4746,
			 0xb8ae, 0x4755, 0x4751, 0xbb55, 0x47ad, 0xb8aa, 0xb8ae, 0x47aa,
			 0x0000, 0x0000, 0xff64, 0x0155, 0x0155, 0x00b8, 0x00d8, 0x0117,
			 0x01b6, 0x013c};*/
//hexdump(&pkmn, sizeof(struct Pokemon), 0x03004360 + 100);
void hexdump_C(void *addr, size_t len,char* PKMN_NAME) {
  uint16_t *_addr = (uint16_t *) addr;
  //printf("Debug %d \n",addr);
  //printf("Debug %d \n",len);
  char ascii_buf[17];
  size_t i;
  printf("u32 %s_pokemonbuffer[50] = {\n",PKMN_NAME);
  for(i = 0; i < len/2; ++i) {
    if((i % 8) == 0) {
      if(i != 0) {
        printf("\n");
      }
    }
    if (i == 49){
      printf(" 0x%04x", _addr[i]);
    }
    else{
      printf(" 0x%04x,", _addr[i]);}
  };
  printf("};\n");
}


#define CCV(c, h) case (c): text[i] = (char)(h); break
bool pcsconv(char *text, size_t len, uint16_t language) {
  if(language != LANGUAGE_ENGLISH) return false;

  for(size_t i = 0; i < len; i++) {
    switch(text[i]) {
    CCV(' ', 0x00);
    CCV('&', 0x2d);
    CCV('+', 0x2e);
    CCV('=', 0x35);
    CCV('%', 0x5b);
    CCV('(', 0x5c);
    CCV(')', 0x5d);
    CCV('0', 0xa1);
    CCV('1', 0xa2);
    CCV('2', 0xa3);
    CCV('3', 0xa4);
    CCV('4', 0xa5);
    CCV('5', 0xa6);
    CCV('6', 0xa7);
    CCV('7', 0xa8);
    CCV('8', 0xa9);
    CCV('9', 0xaa);
    CCV('!', 0xab);
    CCV('?', 0xac);
    CCV('.', 0xad);
    CCV('-', 0xae);
    CCV(',', 0xb8);
    CCV('/', 0xba);
    CCV('A', 0xbb);
    CCV('B', 0xbc);
    CCV('C', 0xbd);
    CCV('D', 0xbe);
    CCV('E', 0xbf);
    CCV('F', 0xc0);
    CCV('G', 0xc1);
    CCV('H', 0xc2);
    CCV('I', 0xc3);
    CCV('J', 0xc4);
    CCV('K', 0xc5);
    CCV('L', 0xc6);
    CCV('M', 0xc7);
    CCV('N', 0xc8);
    CCV('O', 0xc9);
    CCV('P', 0xca);
    CCV('Q', 0xcb);
    CCV('R', 0xcc);
    CCV('S', 0xcd);
    CCV('T', 0xce);
    CCV('U', 0xcf);
    CCV('V', 0xd0);
    CCV('W', 0xd1);
    CCV('X', 0xd2);
    CCV('Y', 0xd3);
    CCV('Z', 0xd4);
    CCV('a', 0xd5);
    CCV('b', 0xd6);
    CCV('c', 0xd7);
    CCV('d', 0xd8);
    CCV('e', 0xd9);
    CCV('f', 0xda);
    CCV('g', 0xdb);
    CCV('h', 0xdc);
    CCV('i', 0xdd);
    CCV('j', 0xde);
    CCV('k', 0xdf);
    CCV('l', 0xe0);
    CCV('m', 0xe1);
    CCV('n', 0xe2);
    CCV('o', 0xe3);
    CCV('p', 0xe4);
    CCV('q', 0xe5);
    CCV('r', 0xe6);
    CCV('s', 0xe7);
    CCV('t', 0xe8);
    CCV('u', 0xe9);
    CCV('v', 0xea);
    CCV('w', 0xeb);
    CCV('x', 0xec);
    CCV('y', 0xed);
    CCV('z', 0xee);
    CCV(':', 0xf0);
    CCV('\n', 0xfe);
    CCV('\0', 0xff);
    default:
      return false;
    }
  }

  return true;
}
#undef CCV

void test_dump(void *addr, size_t len){
  uint8_t *_addr = (uint8_t *) addr;
  size_t i;
  for ( i = 0; i < len; i++)
  {
    printf(" %02x   ", _addr[i]);
  }
  
}


void hexdump_original(void *addr, size_t len, size_t offset) {
  uint8_t *_addr = (uint8_t *) addr;
  //printf("Debug %d \n",addr);
  //printf("Debug %d \n",len);
  char ascii_buf[17];
  size_t i;

  for(i = 0; i < len; ++i) {
    if((i % 16) == 0) {
      if(i != 0) {
        //printf("  %s\n", ascii_buf);
        printf("\n");
      }

      printf("%08lx:", offset + i);
    }

    printf(" %02x", _addr[i]);

    if((_addr[i] < 0x20) || (_addr[i] > 0x7e)) {
      ascii_buf[i % 16] = '.';
    } else {
      ascii_buf[i % 16] = (char) _addr[i];
    }
    ascii_buf[(i % 16) + 1] = '\0';
  };

  for(; (i % 16) != 0; ++i) {
    printf("   ");
  }

  printf("  %s\n", ascii_buf);
}

void hexdump(void *addr, size_t len, char* PKMN_NAME) {
  uint16_t *_addr = (uint16_t *) addr;
  //printf("Debug %d \n",addr);
  //printf("Debug %d \n",len);
  char ascii_buf[17];
  size_t i;
  printf("constant %s : PKMN_structure := (",PKMN_NAME);
  for(i = 0; i < len/2; ++i) {
    if((i % 8) == 0) {
      if(i != 0) {
        //printf("\n");
      }
    }
    
    switch(i)
    {
    case 0: printf("\nPERS_VAL => (");break;
    case 1: printf(",");break;
    case 2: printf("),\nOT_ID => (");break;
    case 3: printf(",");break;
    case 4: printf("),\nNickname => (");break;
    case 5 ... 8 :  printf(",");break;
    case 9: printf("),\nEGG_LANGUAGE=> ");break;
    case 10:printf(",\nOT_NAME_MARKINGS=> (");break;
    case 11 ... 13 : printf(",");break;
    case 14:printf("),\nChecksum => ");break;
    case 15:printf(",\nZERO_FIELD => ");break;
    case 16:printf(",\nDATA_FIELD => (");break;
    case 17 ... 23 : printf(",");break;
    case 24:printf("\n\t\t");break;
    case 25 ... 31 : printf(",");break;
    case 32:printf("\n\t\t");break;
    case 33 ... 39 : printf(",");break;
    case 40 : printf("),\nSTATUS => (");break;
    case 41 : printf(",");break;
    case 42:printf("),\nPokerus_Level => ");break;
    case 43:printf(",\nCurrent_HP => ");break;
    case 44:printf(",\nMAX_HP => ");break;
    case 45:printf(",\nATTACK => ");break;
    case 46:printf(",\nDefense=> ");break;
    case 47:printf(",\nSpeed=> ");break;
    case 48:printf(",\nSP_ATTACK=> ");break;
    case 49:printf(",\nSP_Defense=> ");break;
    default: break;
    }

    printf(" x\"%04x\"", _addr[i]);
  };
  printf("\n);\n");
}


#define CC(c , h) case (h): return c; break;

char char_conv( uint8_t character,uint16_t language){
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
