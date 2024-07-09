#include <getopt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "PokemonHeader/defaults.h"
#include "PokemonHeader/constants.h"

#include "PokemonHeader/structs.h"

#include "PokemonHeader/pokemon_generation_functions.h"

uint16_t calc_HP_stat (uint16_t hp_ev, uint16_t hp_iv, uint16_t hp_base_stat, uint16_t level){
  uint16_t ev_gain  = floor(hp_ev/4);
  uint16_t temp1 = (2* hp_base_stat) + hp_iv + ev_gain ;
  uint16_t temp2 = floor(temp1 * level / 100);
  return temp2 + level + 10;
};

uint16_t calc_Stat_Value (uint16_t ev_value, uint16_t iv_value, uint16_t base_stat, uint16_t level, float_t nature ){
  uint16_t ev_gain  = floor(ev_value/4);
  uint16_t temp1 = (2*base_stat) + iv_value + ev_gain ;
  uint16_t temp2 = floor((temp1*level/100)+5);
  //printf("%d %d %d %f\n",ev_gain ,temp1,temp2,floor(temp2 * nature));
  return floor(temp2 * nature);
};


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