#include <math.h>
/*
 *Calculates the actual Stat Value from the Level and the Base Stat, IV and EV Values
*/
uint16_t calc_HP_stat (uint16_t hp_ev, uint16_t hp_iv, uint16_t hp_base_stat, uint16_t level);

uint16_t calc_Stat_Value (uint16_t ev_value, uint16_t iv_value, uint16_t base_stat, uint16_t level, float_t nature);



// Encrypt the pokémon structures together
uint16_t data_encrypt_to(
  void *dest,
  uint32_t personality,
  uint32_t trainer_id,
  struct Growth growth,
  struct Attacks attacks,
  struct Condition condition,
  struct Misc misc
);

//hexdump(&pkmn, sizeof(struct Pokemon), 0x03004360 + 100);
void hexdump_C(void *addr, size_t len,char* PKMN_NAME);


// This function converts normal ASCII character into the Pokemon Letter format
bool pcsconv(char *text, size_t len, uint16_t language) ;

//This function convert an Pokemon Letter format 8Bit character into the ASCII Format
char char_conv( uint8_t character,uint16_t language);
