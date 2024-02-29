
#define PERSONALITY_VALUE_SHINY_ADAMANT 
#define PERSONALITY_VALUE_SHINY_MODEST 
#define PERSONALITY_VALUE_SHINY_CALM 

#define PERSONALITY_VALUE_SHINY_BASE 0x41F00000


//P mod 24 = nature but shiny formel s <8 must be true
#define PERS_VAL_SHINY_HARDY    0x41F40001
#define PERS_VAL_SHINY_LONELY   0x41F40002
#define PERS_VAL_SHINY_BRAVE    0x41F40003
#define PERS_VAL_SHINY_ADAMANT  0x41F20001
#define PERS_VAL_SHINY_NAUGHTY  0x41F20002
#define PERS_VAL_SHINY_BOLD     0x41F00000
#define PERS_VAL_SHINY_DOCILE   0x41F00001
#define PERS_VAL_SHINY_RELAXED  0x41F00002
#define PERS_VAL_SHINY_IMPISH   0x41F00003
#define PERS_VAL_SHINY_LAX      0x41F00004
#define PERS_VAL_SHINY_TIMID    0x41F00005
#define PERS_VAL_SHINY_HASTY    0x41F00006
#define PERS_VAL_SHINY_SERIOUS  0x41F00007
#define PERS_VAL_SHINY_JOLLY    0x41F30000
#define PERS_VAL_SHINY_NAIVE    0x41F30001
#define PERS_VAL_SHINY_MODEST   0x41F30002
#define PERS_VAL_SHINY_MILD     0x41F10000
#define PERS_VAL_SHINY_QUIET    0x41F10001
#define PERS_VAL_SHINY_BASHFUL  0x41F10002
#define PERS_VAL_SHINY_RASH     0x41F10003
#define PERS_VAL_SHINY_CALM     0x41F10004
#define PERS_VAL_SHINY_GENTLE   0x41F10005
#define PERS_VAL_SHINY_SASSY    0x41F10006
#define PERS_VAL_SHINY_CAREFUL  0x41F10007
#define PERS_VAL_SHINY_QUIRKY   0x41F40000

#include<stdint.h>
#include "natures.h"

uint32_t get_pers_value(uint32_t trainerId, uint16_t nature, uint16_t shiny){
  uint32_t pers_value;
  if (shiny == 0){
    pers_value = nature;
  }
  else{
    switch (nature)
    {
    case NATURE_HARDY : pers_value = PERS_VAL_SHINY_HARDY ;break;
    case NATURE_LONELY : pers_value = PERS_VAL_SHINY_LONELY ;break;
    case NATURE_BRAVE : pers_value = PERS_VAL_SHINY_BRAVE ;break;
    case NATURE_ADAMANT : pers_value = PERS_VAL_SHINY_ADAMANT ;break;
    case NATURE_NAUGHTY : pers_value = PERS_VAL_SHINY_NAUGHTY ;break;
    case NATURE_BOLD : pers_value = PERS_VAL_SHINY_BOLD ;break;
    case NATURE_DOCILE : pers_value = PERS_VAL_SHINY_DOCILE ;break;
    case NATURE_RELAXED : pers_value = PERS_VAL_SHINY_RELAXED ;break;
    case NATURE_IMPISH : pers_value = PERS_VAL_SHINY_IMPISH ;break; 
    case NATURE_LAX : pers_value = PERS_VAL_SHINY_LAX ;break;
    case NATURE_TIMID : pers_value = PERS_VAL_SHINY_TIMID ;break;  
    case NATURE_HASTY  : pers_value = PERS_VAL_SHINY_HASTY ;break; 
    case NATURE_SERIOUS : pers_value = PERS_VAL_SHINY_SERIOUS ;break;
    case NATURE_JOLLY : pers_value = PERS_VAL_SHINY_JOLLY ;break;
    case NATURE_NAIVE : pers_value = PERS_VAL_SHINY_NAIVE ;break;
    case NATURE_MODEST : pers_value = PERS_VAL_SHINY_MODEST ;break;
    case NATURE_MILD : pers_value = PERS_VAL_SHINY_MILD ;break;
    case NATURE_QUIET : pers_value = PERS_VAL_SHINY_QUIET ;break;
    case NATURE_BASHFUL : pers_value = PERS_VAL_SHINY_BASHFUL ;break;
    case NATURE_RASH : pers_value = PERS_VAL_SHINY_RASH ;break;
    case NATURE_CALM : pers_value = PERS_VAL_SHINY_CALM ;break;
    case NATURE_GENTLE : pers_value = PERS_VAL_SHINY_GENTLE ;break;
    case NATURE_SASSY : pers_value = PERS_VAL_SHINY_SASSY ;break;
    case NATURE_CAREFUL : pers_value = PERS_VAL_SHINY_CAREFUL ;break;
    case NATURE_QUIRKY : pers_value = PERS_VAL_SHINY_QUIRKY ;break;
    default:
      pers_value = 0;break;
      break;
    }
  }

  return pers_value;
};