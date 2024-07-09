
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

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

uint32_t get_pers_value(uint32_t trainerId, uint16_t nature, uint16_t shiny);

struct Pokemon_Info parsingTextFile(char *filename);

void removeLastSpace(char *str);

int generatePokemonStructure(char *filename, uint16_t *structure_addr);
void convertStructureintoBuffer(uint16_t * TransmitBuffer,size_t Buffersize,char *filename);