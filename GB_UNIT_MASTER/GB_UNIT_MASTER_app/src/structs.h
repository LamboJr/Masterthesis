#ifndef STRUCT_HEADER
#define STRUCT_HEADER


#include "constant.h"
struct PPBonus {
  u8 move1:2;
  u8 move2:2;
  u8 move3:2;
  u8 move4:2;
};

struct Growth {
  u16 species;
  u16 held_item;
  u32 experience;
  struct PPBonus pp_bonus;
  u8 friendship;
  u16 unknown;
};

struct Attacks {
  u16 moves[4];
  u8 pp[4];
};

struct Condition {
  u8 hp_ev;
  u8 attack_ev;
  u8 defense_ev;
  u8 speed_ev;
  u8 special_attack_ev;
  u8 special_defense_ev;
  u8 coolness;
  u8 beauty;
  u8 cuteness;
  u8 smartness;
  u8 toughness;
  u8 feel;
};

struct Origins {
  u16 level_met:7;
  u16 game_met:4;
  u16 pokeball_type:4;
  u16 trainer_gender:1;
};


struct Pokerus {
  u8 days_remaining:4;
  u8 strain:4;
};

struct IVs {
  u32 hp:5;
  u32 attack:5;
  u32 defense:5;
  u32 speed:5;
  u32 special_attack:5;
  u32 special_defense:5;
  u32 egg:1;
  u32 ability:1;
};

struct Ribbons {
  u32 cool:3;
  u32 beauty:3;
  u32 cute:3;
  u32 smart:3;
  u32 tough:3;
  u32 champion:1;
  u32 winning:1;
  u32 victory:1;
  u32 artist:1;
  u32 effort:1;
  u32 special1:1;
  u32 special2:1;
  u32 special3:1;
  u32 special4:1;
  u32 special5:1;
  u32 special6:1;
  u32 padding:5;
  u32 obedience:1;
};

struct Misc {
  struct Pokerus pokerus;
  u8 met_location;
  struct Origins origins;
  struct IVs ivs;
  struct Ribbons ribbons;
};

struct Status {
  u32 sleep:3;
  u32 poisoned:1;
  u32 burnt:1;
  u32 frozen:1;
  u32 paralyzed:1;
  u32 bad_poisoned:1;
  u32 zero_filler:24;
};


struct SpeciesInfo
{
    u16 baseHP;
    u16 baseAttack;
    u16 baseDefense;
    u16 baseSpeed;
    u16 baseSpAttack;
    u16 baseSpDefense;
};
struct Pokemon {
  u32 personality;
  u32 trainer_id;
  char nickname[NICKNAME_LENGTH];
  u16 language;
  char trainer_name[TRAINER_NAME_LENGTH];
  u8 markings;
  u16 checksum;
  u16 unknown;
  u8 data[DATA_LENGTH];
  //struct Status status;
  u32 status;
  u8 level;
  u8 pokerus;
  u16 current_health;
  u16 max_health;
  u16 attack;
  u16 defense;
  u16 speed;
  u16 special_attack;
  u16 special_defense;
};


struct TrainerCard
{
    /*0x00*/ uint8_t gender;
    /*0x01*/ uint8_t stars;
    /*0x02*/ uint8_t hasPokedex;
    /*0x03*/ uint8_t caughtAllHoenn;
    /*0x04*/ uint8_t hasAllPaintings;
    /*0x06*/ uint16_t hofDebutHours;
    /*0x08*/ uint16_t hofDebutMinutes;
    /*0x0A*/ uint16_t hofDebutSeconds;
    /*0x0C*/ uint16_t caughtMonsCount;
    /*0x0E*/ uint16_t trainerId;
    /*0x10*/ uint16_t playTimeHours;
    /*0x12*/ uint16_t playTimeMinutes;
    /*0x14*/ uint16_t linkBattleWins;
    /*0x16*/ uint16_t linkBattleLosses;
    /*0x18*/ uint16_t battleTowerWins;
    /*0x1A*/ uint16_t battleTowerStraightWins;
    /*0x1C*/ uint16_t contestsWithFriends;
    /*0x1E*/ uint16_t pokeblocksWithFriends;
    /*0x20*/ uint16_t pokemonTrades;
    /*0x24*/ uint32_t money;
    /*0x28*/ uint16_t easyChatProfile[TRAINER_CARD_PROFILE_LENGTH];
    /*0x30*/ uint8_t playerName[PLAYER_NAME_LENGTH + 1];
    /*0x38*/ uint8_t version;
    /*0x3A*/ uint16_t linkHasAllFrontierSymbols;
    /*0x3C*/ union {
                uint32_t berryCrush;
                uint32_t frontier;
             } linkPoints; // This field is used differently by FRLG vs Emerald
    /*0x40*/ uint32_t unionRoomNum;
    /*0x44*/ uint8_t filler[8];
    /*0x4C*/ uint8_t shouldDrawStickers; // FRLG only
    /*0x4D*/ uint8_t unused;
    /*0x4E*/ uint8_t monIconTint; // FRLG only
    /*0x4F*/ uint8_t unionRoomClass;
    /*0x50*/ uint8_t stickers[TRAINER_CARD_STICKER_TYPES]; // FRLG only
    /*0x54*/ uint16_t monSpecies[PARTY_SIZE]; // FRLG only
             // Note: Link players use linkHasAllFrontierSymbols, not the field below,
             // which they use for a Wonder Card flag id instead (see CreateTrainerCardInBuffer)
    /*0x60*/ uint16_t hasAllFrontierSymbols;
    /*0x62*/ uint16_t frontierBP;
};
#endif
