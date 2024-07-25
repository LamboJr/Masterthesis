
#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50

uint32_t create_pers_value(uint32_t IDnumber, uint8_t nature, uint8_t shiny, uint8_t ability, uint8_t gender);
uint16_t check_pers_value_nature(uint32_t pers_value);
uint8_t check_pers_value_shiny(uint32_t pers_value,uint32_t IDnumber);
uint8_t check_pers_value_gender(uint32_t pers_value);
uint8_t check_pers_value_ability(uint32_t pers_value);


uint32_t get_pers_value(uint32_t trainerId, uint16_t nature, uint16_t shiny);

struct Pokemon_Info parsingTextFile(char *filename);

void removeLastSpace(char *str);

