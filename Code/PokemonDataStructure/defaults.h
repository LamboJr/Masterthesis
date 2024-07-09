

#define SPECIES SPECIES_BULBASAUR
#define ITEM_INDEX ITEM_RARE_CANDY
#define EXPERIENCE 1250000

//Moves specifer
#define MOVE1_INDEX MOVE_HIDDEN_POWER
#define MOVE2_INDEX MOVE_NONE
#define MOVE3_INDEX MOVE_NONE
#define MOVE4_INDEX MOVE_NONE

//Actual PP for the Moves
#define PP_MOVE1 5
#define PP_MOVE2 5
#define PP_MOVE3 5
#define PP_MOVE4 5

//PP Bonuses 
#define PP_BONUS_MOVE1 3
#define PP_BONUS_MOVE2 3
#define PP_BONUS_MOVE3 3
#define PP_BONUS_MOVE4 3

#define FRIENDSHIP 0xFF  //MAX Friendship, does not matter because of trade

//Contest Values
#define COOLNESS 0
#define BEAUTY 0
#define CUTENESS 0
#define SMARTNESS 0
#define TOUGHNESS 0
#define FEEL 0

//Effort Values
#define HP_EV 0
#define ATTACK_EV 0
#define DEFENSE_EV 0
#define SPEED_EV 0
#define SPEZ_ATTACK_EV 0
#define SPEZ_DEFENSE_EV 0

//Inherited Values
#define HP_IV 31
#define ATTACK_IV 31
#define DEFENSE_IV 31
#define SPEED_IV 31
#define SPEZ_ATTACK_IV 31
#define SPEZ_DEFENSE_IV 31

//Ribbons

#define COOL_RIBBON 0
#define BEAUTY_RIBBON 0
#define CUTE_RIBBON 0
#define SMART_RIBBON 0
#define TOUGH_RIBBON 0
#define CHAMPION_RIBBON 0
#define WINNING_RIBBON 0
#define VICTORY_RIBBON 0
#define ARTIST_RIBBON 0
#define EFFORT_RIBBON 0
#define SPECIAL1_RIBBON 0
#define SPECIAL2_RIBBON 0
#define SPECIAL3_RIBBON 0
#define SPECIAL4_RIBBON 0
#define SPECIAL5_RIBBON 0
#define SPECIAL6_RIBBON 0
#define PADDING_RIBBON 0
#define OBEDIENCE_RIBBON 0


#define MET_GAME GAME_LEAFGREEN
#define POKEBALL_CAUGHT POKEBALL_STANDARD
#define MET_LANGUAGE LANGUAGE_GERMAN

#define MET_LOCATION 0x58

#define MET_LEVEL 0   //0 = EGG
#define LEVEL 100


#define NATURE 0

#define SECRET_ID_LAMBO 0x06A6
#define SECRET_ID_KEVIN 0x392B

#define TRAINER_ID_LAMBO 0x4753
#define TRAINER_ID_KEVIN 0x8D44

#define SECRET_ID_DANIEL 0x60AC
#define OT_TRAINER_ID_DANIEL 0xC371
#define TRAINER_ID_DANIEL 0x60ACC371 //Leafgreen Daniel

#define SECRET_ID SECRET_ID_DANIEL
#define OT_TRAINER_ID OT_TRAINER_ID_DANIEL
//#define TRAINER_ID 0x06A64753

//#define TRAINER_ID 0x60ACC371 //Leafgreen Daniel
#define TRAINER_ID (SECRET_ID << 16) + OT_TRAINER_ID

#define TRAINER_NAME "Daniel"
#define NICKNAME "Pokemon"
