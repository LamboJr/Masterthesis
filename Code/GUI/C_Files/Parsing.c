#include "MainCHeader.h"

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

struct Pokemon_Info parsingTextFile(char *filename){
    struct Pokemon_Info PKMN_Info;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
    }

    //defaults
    PKMN_Info.Level = 100;
    PKMN_Info.shiny = 0;
    PKMN_Info.gender = 'M';
    PKMN_Info.happiness = 255;
    //memcpy(PKMN_Info.nature, "Adamant",20);
    memcpy(PKMN_Info.Moves[0], "Hidden Power" ,20);
    memcpy(PKMN_Info.Moves[1], "Protect", 20);
    memcpy(PKMN_Info.Moves[2], "None",20);
    memcpy(PKMN_Info.Moves[3], "None",20);
    PKMN_Info.ev_Attack = 0;
    PKMN_Info.ev_Defense = 0;
    PKMN_Info.ev_HP =  0;
    PKMN_Info.ev_SpAttack = 0;
    PKMN_Info.ev_SpDefense = 0;
    PKMN_Info.ev_Speed = 0;
    PKMN_Info.iv_Attack = 31;
    PKMN_Info.iv_Defense = 31;
    PKMN_Info.iv_HP = 31;
    PKMN_Info.iv_SpAttack = 31;
    PKMN_Info.iv_SpDefense = 31;
    PKMN_Info.iv_Speed =31;
    memcpy(PKMN_Info.item,"None",20);

    char nature_temp[20];
    char line[MAX_LINE_LENGTH];
    int temp1;
    int readLineCount = 0;
    int movecounter = 0;
    int tempEV;
    char *token;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        //removeLastSpace(line);
        switch (readLineCount)
        {
        case 0 :
            printf("Zeile : %s\n",line);
            token = strtok(line, " ");
            //Brutalanda (Salamence) (M) @ Lum Berry  
            while (token != NULL){
                //Case for Nickname or normal Name if pokemon has no nickname
                if ((token[0] != '(') && (token[0] != '@'))
                {   
                    memcpy(PKMN_Info.nickname, token, NICKNAME_LENGTH);
                    memcpy(PKMN_Info.Pokemon_name, token, 20);
                    //printf("Nickname : %s\t",PKMN_Info.nickname);
                }

                // case for Pokemon with Nickname (Real name is in brackets
                if ((token[0] == '(') && (token[2] != ')'))
                {
                    int i = 1;
                    while (token[i] != ')')
                    {   
                        PKMN_Info.Pokemon_name[i-1] = token[i];
                        i++;
                    }
                    PKMN_Info.Pokemon_name[i-1]= '\0';
                    //printf("Name : %s\t",PKMN_Info.Pokemon_name);
                }
                //case for gender specification
                if ((token[0] == '(') && (token[2] == ')'))
                {
                    PKMN_Info.gender = token[1];
;                   //printf("Gender : %c\t",PKMN_Info.gender); 
                }
                //case for item specification
                if (token[0] == '@'){
                    token = strtok(NULL, "\0");
                    memcpy(PKMN_Info.item, token, 20);
                    for (size_t i = 0; i < sizeof(PKMN_Info.item); i++)
                    {
                        if (PKMN_Info.item[i] == ' '){
                            ///check if letter is following 
                        }
                    }
                    removeLastSpace(PKMN_Info.item);
                    //printf("Item : %s\n",PKMN_Info.item);
                }
            
                token = strtok(NULL, " ");
            }
            break;

        default:
            printf("Zeile : %s\n",line);
            token = strtok(line, " ");
            while (token != NULL){
                if(!strcmp(token,"Ability:")){
                    token = strtok(NULL, "\0");
                    
                    memcpy(PKMN_Info.ability, token,20);
                    //printf("Ability : %s\n",PKMN_Info.ability);
                    token = strtok(NULL, " "); 
                }
                else if(!strcmp(token,"Level:")){
                    token = strtok(NULL, "\0");
                    PKMN_Info.Level = atoi(token);
                    //printf("Level : %d\n",PKMN_Info.Level);
                    token = strtok(NULL, " "); 
                }
                else if(!strcmp(token,"Shiny:")){
                    token = strtok(NULL, "\0");
                    token[strcspn(token, " ")] = '\0';
                    if(!strcmp(token,"Yes")){
                        PKMN_Info.shiny = 1;
                    }
                    else{
                        PKMN_Info.shiny = 0;
                    }
                    //printf("Shiny : %d\n",PKMN_Info.shiny);
                    token = strtok(NULL, " "); 
                }
                else if(!strcmp(token,"Happiness:")){
                    token = strtok(NULL, "\0");
                    PKMN_Info.happiness = atoi(token);
                    //printf("Happinies : %d\n",PKMN_Info.happiness);
                    token = strtok(NULL, " "); 
                }
                else if(!strcmp(token,"EVs:")){
                    
                    while (token != NULL)
                    {
                    token = strtok(NULL, " ");
                    //printf("Token : %s\t",token);
                    tempEV = atoi(token);
                    //printf("%d\t",tempEV);
                    token = strtok(NULL, " ");
                    //printf("Token : %s\t",token);
                    if (!strcmp(token,"HP")){PKMN_Info.ev_HP = tempEV;}
                    if (!strcmp(token,"Atk")){PKMN_Info.ev_Attack = tempEV;}
                    if (!strcmp(token,"Def")){PKMN_Info.ev_Defense = tempEV;}
                    if (!strcmp(token,"SpD")){PKMN_Info.ev_SpDefense = tempEV;}
                    if (!strcmp(token,"SpA")){PKMN_Info.ev_SpAttack = tempEV;}
                    if (!strcmp(token,"Spe")){PKMN_Info.ev_Speed = tempEV;} 
                    //printf("Values %d %d %d %d %d %d\n",PKMN_Info.ev_HP,PKMN_Info.ev_Attack,PKMN_Info.ev_Defense,PKMN_Info.ev_SpAttack,PKMN_Info.ev_SpDefense,PKMN_Info.ev_Speed);
                    token = strtok(NULL, " ");
                    //printf("Token : %s\n",token); 
                    }
                    printf("Values %d %d %d %d %d %d\n",PKMN_Info.ev_HP,PKMN_Info.ev_Attack,PKMN_Info.ev_Defense,PKMN_Info.ev_SpAttack,PKMN_Info.ev_SpDefense,PKMN_Info.ev_Speed);
                
                }
                else if(!strcmp(token,"IVs:")){
                     while (token != NULL)
                    {
                    token = strtok(NULL, " ");
                    //printf("Token : %s\t",token);
                    tempEV = atoi(token);
                    //printf("%d\t",tempEV);
                    token = strtok(NULL, " ");
                    //printf("Token : %s\t",token);
                    if (!strcmp(token,"HP")){PKMN_Info.iv_HP = tempEV;}
                    if (!strcmp(token,"Atk")){PKMN_Info.iv_Attack = tempEV;}
                    if (!strcmp(token,"Def")){PKMN_Info.iv_Defense = tempEV;}
                    if (!strcmp(token,"SpD")){PKMN_Info.iv_SpDefense = tempEV;}
                    if (!strcmp(token,"SpA")){PKMN_Info.iv_SpAttack = tempEV;}
                    if (!strcmp(token,"Spe")){PKMN_Info.iv_Speed = tempEV;} 
                    //printf("Values %d %d %d %d %d %d\n",PKMN_Info.iv_HP,PKMN_Info.iv_Attack,PKMN_Info.iv_Defense,PKMN_Info.iv_SpAttack,PKMN_Info.iv_SpDefense,PKMN_Info.iv_Speed);
                    token = strtok(NULL, " ");
                    //printf("Token : %s\n",token); 
                    }
                    printf("Values %d %d %d %d %d %d\n",PKMN_Info.iv_HP,PKMN_Info.iv_Attack,PKMN_Info.iv_Defense,PKMN_Info.iv_SpAttack,PKMN_Info.iv_SpDefense,PKMN_Info.iv_Speed);
                }
                else if( token[0] == '-'){
                    token = strtok(NULL, "\n");
                    memcpy(PKMN_Info.Moves[movecounter++], token,20);
                    removeLastSpace(PKMN_Info.Moves[movecounter-1]);
                    //printf("Move%d : %s \n",movecounter,PKMN_Info.Moves[movecounter-1]);
                    token = strtok(NULL, " "); 
                }
                else if( !strcmp(token,"Nature")){
                    memcpy(PKMN_Info.nature, nature_temp,20);
                    //printf("Real nature Value : %s\n",PKMN_Info.nature);
                    token = strtok(NULL, " "); 
                }
                else{
                    memcpy(nature_temp, token,20);
                    //printf("N Debug: %s\n",nature_temp);
                    //token = strtok(NULL, "\0");
                    token = strtok(NULL, " "); 
                }
            
            }
            break;
        }
        readLineCount++;
        
    }
    fclose(file);
    return PKMN_Info;
}

void removeLastSpace(char *str){
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


uint32_t create_pers_value(uint32_t IDnumber, uint8_t nature, uint8_t shiny, uint8_t ability, uint8_t gender){
uint32_t pers_value = 0;
    for (uint32_t i = 0; i < 0xFFFFFFFF;i++){
        pers_value = i;
        if (check_pers_value_ability(pers_value) != ability){
            continue;
        }
        if (check_pers_value_gender(pers_value) != gender) {
            continue;
        }

        if(check_pers_value_nature(pers_value) != nature){
            continue;
        }
        if (check_pers_value_shiny(pers_value,IDnumber) != shiny){
            continue;
        }
        break; 

    }


return pers_value;
}
uint8_t check_pers_value_shiny(uint32_t pers_value,uint32_t IDnumber){
    uint16_t SecretID, TrainerID;
    TrainerID = IDnumber & 0x0000FFFF;
    SecretID = (IDnumber >> 16) & 0xFFFF;

    uint16_t p1,p2;
    p1 = (pers_value >> 16) & 0xFFFF;
    p2 = pers_value & 0xFFFF;

    uint16_t Res = TrainerID ^ SecretID ^ p1 ^ p2;
    if (Res < 8){
        return 1;
    }else{
        return 0;
    }

}
uint8_t check_pers_value_gender(uint32_t pers_value){
    uint32_t masked_gender_value = 0x000000FF & pers_value; // Last 8 Bits of the pers_value
    if (masked_gender_value >= 225){
        //Safe Male Pokemon
        return 0;
    }else if (masked_gender_value < 31)
    {
        //Safe Female
        return 1;
    }else{
        // Depending on threshold
        return 2;
    }
    
}

uint8_t check_pers_value_ability(uint32_t pers_value){
return (pers_value % 2);
//returns 0 if Pokemon has first ability (LSB = 0) or 1 if Second Ability (LSB = 1)
}

uint16_t check_pers_value_nature(uint32_t pers_value){

return (pers_value % 25);

}