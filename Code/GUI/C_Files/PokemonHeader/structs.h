
struct PPBonus {
  uint8_t move1:2;
  uint8_t move2:2;
  uint8_t move3:2;
  uint8_t move4:2;
};

struct Growth {
  uint16_t species; 
  uint16_t held_item;
  uint32_t experience;
  struct PPBonus pp_bonus;
  uint8_t friendship;
  uint16_t unknown;
};

struct Attacks {
  uint16_t moves[4];
  uint8_t pp[4];
};

struct Condition {
  uint8_t hp_ev;
  uint8_t attack_ev;
  uint8_t defense_ev;
  uint8_t speed_ev;
  uint8_t special_attack_ev;
  uint8_t special_defense_ev;
  uint8_t coolness;
  uint8_t beauty;
  uint8_t cuteness;
  uint8_t smartness;
  uint8_t toughness;
  uint8_t feel;
};

struct Origins {
  uint16_t level_met:7;
  uint16_t game_met:4;
  uint16_t pokeball_type:4;
  uint16_t trainer_gender:1;
};


struct Pokerus {
  uint8_t days_remaining:4;
  uint8_t strain:4;
};

struct IVs {
  uint32_t hp:5;
  uint32_t attack:5;
  uint32_t defense:5;
  uint32_t speed:5;
  uint32_t special_attack:5;
  uint32_t special_defense:5;
  uint32_t egg:1;
  uint32_t ability:1;
};

struct Ribbons {
  uint32_t cool:3;
  uint32_t beauty:3;
  uint32_t cute:3;
  uint32_t smart:3;
  uint32_t tough:3;
  uint32_t champion:1;
  uint32_t winning:1;
  uint32_t victory:1;
  uint32_t artist:1;
  uint32_t effort:1;
  uint32_t special1:1;
  uint32_t special2:1;
  uint32_t special3:1;
  uint32_t special4:1;
  uint32_t special5:1;
  uint32_t special6:1;
  uint32_t padding:5;
  uint32_t obedience:1;
};

struct Misc {
  struct Pokerus pokerus;
  uint8_t met_location;
  struct Origins origins;
  struct IVs ivs;
  struct Ribbons ribbons;
};

struct Status {
  uint32_t sleep:3;
  uint32_t poisoned:1;
  uint32_t burnt:1;
  uint32_t frozen:1;
  uint32_t paralyzed:1;
  uint32_t bad_poisoned:1;
  uint32_t zero_filler:24;
};

/*
struct SpeciesInfo
{
    uint16_t baseHP;
    uint16_t baseAttack;
    uint16_t baseDefense;
    uint16_t baseSpeed;
    uint16_t baseSpAttack;
    uint16_t baseSpDefense;
};*/

struct SpeciesInfo
{
 /* 0x00 */ uint8_t baseHP;
 /* 0x01 */ uint8_t baseAttack;
 /* 0x02 */ uint8_t baseDefense;
 /* 0x03 */ uint8_t baseSpeed;
 /* 0x04 */ uint8_t baseSpAttack;
 /* 0x05 */ uint8_t baseSpDefense;
 /* 0x06 */ uint8_t types[2];
 /* 0x08 */ uint8_t catchRate;
 /* 0x09 */ uint8_t expYield;
 /* 0x0A */ uint16_t evYield_HP:2;
 /* 0x0A */ uint16_t evYield_Attack:2;
 /* 0x0A */ uint16_t evYield_Defense:2;
 /* 0x0A */ uint16_t evYield_Speed:2;
 /* 0x0B */ uint16_t evYield_SpAttack:2;
 /* 0x0B */ uint16_t evYield_SpDefense:2;
 /* 0x0C */ uint16_t itemCommon;
 /* 0x0E */ uint16_t itemRare;
 /* 0x10 */ uint8_t genderRatio;
 /* 0x11 */ uint8_t eggCycles;
 /* 0x12 */ uint8_t friendship;
 /* 0x13 */ uint8_t growthRate;
 /* 0x14 */ uint8_t eggGroups[2];
 /* 0x16 */ uint8_t abilities[2];
 /* 0x18 */ uint8_t safariZoneFleeRate;
 /* 0x19 */ uint8_t bodyColor : 7;
            uint8_t noFlip : 1;
};


struct Pokemon {
  uint32_t personality;
  uint32_t trainer_id;
  char nickname[NICKNAME_LENGTH];
  uint16_t language;
  char trainer_name[TRAINER_NAME_LENGTH];
  uint8_t markings;
  uint16_t checksum;
  uint16_t unknown;
  uint8_t data[DATA_LENGTH];
  struct Status status;
  //uint32_t status;
  uint8_t level;
  uint8_t pokerus;
  uint16_t current_health;
  uint16_t max_health;
  uint16_t attack;
  uint16_t defense;
  uint16_t speed;
  uint16_t special_attack;
  uint16_t special_defense;
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


struct Pokemon_Info{
    char nickname[NICKNAME_LENGTH];
    char Pokemon_name[20];
    char item[20];
    char gender;
    char ability[20];
    uint16_t Level;
    uint8_t shiny;
    uint16_t happiness;
    char nature[20];
    uint16_t ev_HP;
    uint16_t ev_Attack;
    uint16_t ev_Defense;
    uint16_t ev_SpAttack;
    uint16_t ev_SpDefense;
    uint16_t ev_Speed;
    uint16_t iv_HP;
    uint16_t iv_Attack;
    uint16_t iv_Defense;
    uint16_t iv_SpAttack;
    uint16_t iv_SpDefense;
    uint16_t iv_Speed;
    char Moves[4][20];
} ;

struct PokemonStructureGUI{
  char nickname[20];
  char species[20];
  uint8_t shiny; 
  char item[20]; 
  uint8_t genderValue;
  uint8_t ability; 
  uint16_t level;
  char nature[20]; 
  uint16_t ev_HP; 
  uint16_t ev_Attack; 
  uint16_t ev_Defense; 
  uint16_t ev_SpAttack; 
  uint16_t ev_SpDefense;
  uint16_t ev_Speed; 
  uint16_t iv_HP; 
  uint16_t iv_Attack; 
  uint16_t iv_Defense; 
  uint16_t iv_SpAttack; 
  uint16_t iv_SpDefense; 
  uint16_t iv_Speed; 
  char Move1[20]; 
  char Move2[20]; 
  char Move3[20]; 
  char Move4[20];
};