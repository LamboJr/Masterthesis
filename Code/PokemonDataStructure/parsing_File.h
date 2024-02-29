#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

#define MAX_LINE_LENGTH 100
#define MAX_NAME_LENGTH 50



struct Pokemon_Info{
    char nickname[NICKNAME_LENGTH];
    char Pokemon_name[20];
    char item[20];
    char gender;
    char ability[20];
    u_int16_t Level;
    u_int8_t shiny;
    u_int16_t happiness;
    char nature[20];
    u_int16_t ev_HP;
    u_int16_t ev_Attack;
    u_int16_t ev_Defense;
    u_int16_t ev_SpAttack;
    u_int16_t ev_SpDefense;
    u_int16_t ev_Speed;
    u_int16_t iv_HP;
    u_int16_t iv_Attack;
    u_int16_t iv_Defense;
    u_int16_t iv_SpAttack;
    u_int16_t iv_SpDefense;
    u_int16_t iv_Speed;
    char Moves[4][20];
} ;

struct Pokemon_Info parsingTextFile(void){
    struct Pokemon_Info PKMN_Info;

    FILE *file = fopen("data.txt", "r");
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