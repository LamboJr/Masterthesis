#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "GenerateDataStructure.h"
#include "TCP_Server.h"

#define POKEMON1 "Pokemon_Data_Files/Blissey.txt"
#define POKEMON2 "Pokemon_Data_Files/Quaxo.txt"
#define POKEMON3 "Pokemon_Data_Files/Blissey.txt"
#define POKEMON4 "Pokemon_Data_Files/Quaxo.txt"
#define POKEMON5 "Pokemon_Data_Files/Quaxo.txt"
#define POKEMON6 "Pokemon_Data_Files/Quaxo.txt"

extern void convertStructureintoBuffer(uint16_t * TransmitBuffer,size_t Buffersize,char *filename);
extern uint16_t NoPokemon[50];

int main() {

    u8 Received_iD;
    u8 TeamIndex;
    

    uint16_t ReceiveBuffer[6][50];
    
    uint16_t ReceivegenericBuffer[110];

    uint16_t TransmitBuffer[6][50];

    convertStructureintoBuffer(TransmitBuffer[0],50,POKEMON1);
    convertStructureintoBuffer(TransmitBuffer[1],50,POKEMON2);
    convertStructureintoBuffer(TransmitBuffer[2],50,POKEMON3);
    convertStructureintoBuffer(TransmitBuffer[3],50,POKEMON4);
    convertStructureintoBuffer(TransmitBuffer[4],50,POKEMON5);
    convertStructureintoBuffer(TransmitBuffer[5],50,POKEMON6);



    extern uint16_t InitialBuffer[30];

    int  sock = TCP_Server_Init();

 while(1){
    Received_iD = ReceiveID(sock);
    //sleep(1);
    switch(Received_iD){
        case START_ID : {//Start
            SendID(sock,Received_iD);
        }break;
        case HANDSHAKE_ID : {//Handshake
            SendID(sock,Received_iD);
        }break;
        case EXCHANGE_POKEMON_DATA_ID : {//Pokemon Data
            SendID(sock,Received_iD);
            TeamIndex = ReceiveTeamIndex(sock);
            printf("Received Team Index = %d\n",TeamIndex);
	        ReceiveBufferTCP(sock,ReceiveBuffer[TeamIndex],sizeof(ReceiveBuffer[1]));
	        ReceiveBufferTCP(sock,ReceiveBuffer[TeamIndex+1],sizeof(ReceiveBuffer[1]));

            SendBufferTCP(sock,TransmitBuffer[TeamIndex],sizeof(TransmitBuffer[TeamIndex]));
            SendBufferTCP(sock,TransmitBuffer[TeamIndex+1],sizeof(TransmitBuffer[TeamIndex]));
            /*if(TeamIndex == 0){
                    

            SendBufferTCP(sock,Metagross_pokemonbuffer,sizeof(Metagross_pokemonbuffer));
            SendBufferTCP(sock,Blissey_pokemonbuffer,sizeof(Blissey_pokemonbuffer));
            }else{
                SendBufferTCP(sock,NoPokemon,sizeof(NoPokemon));
                SendBufferTCP(sock,NoPokemon,sizeof(NoPokemon));
            }*/
        }break;
        case EXCHANGE_TRAINER_DATA_ID : {//Trainer Data
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,100);
            //SendBufferTCP(sock,TrainerCardBuffer,100);
            SendBufferTCP(sock,ReceivegenericBuffer,100);
        }break;
        case EXCHANGE_LETTER_DATA_ID : {//Letter Data
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,220);
  
            SendBufferTCP(sock,ReceivegenericBuffer,220);
        }break;
        case  EXCHANGE_RIBBON_DATA_ID : {//Ribbon Data
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,40);
  
            SendBufferTCP(sock,ReceivegenericBuffer,40);
        }break;
        case EXCHANGE_EXIT_0_ID :
        case EXCHANGE_EXIT_20_ID : 
        case EXCHANGE_EXIT_C_ID : {
            //Exit
            //sleep(5);
            SendID(sock,Received_iD);
        }break;
        case SEND_INITIAL_BLOCK_ID :{
            //sleep(2);
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,60);
            for(int i = 0;i < 30;i++){
                if( i == 18){

                }else{
                    ReceivegenericBuffer[i] = InitialBuffer[i];
                }
            }
            SendBufferTCP(sock,ReceivegenericBuffer,60);
        }break;
        case SEND_TRADE_COMMAND_ID : {
            SendID(sock,Received_iD);
        }break;
        default :{
                sleep(2);
            SendID(sock,Received_iD);
        }break;

    }


 }
    for(int i =0;i< 6;i++){
        //DecodeBuffer(ReceiveBuffer[i]);
    }
    return 0;
   
/*
	printf("REC Team Index = %d\n",ReceiveTeamIndex(sock));
    SendTeamIndex(sock,2);
    uint16_t ReceiveBuffer[50];
	ReceiveBufferTCP(sock,ReceiveBuffer,sizeof(ReceiveBuffer));
    //SendBufferTCP(sock,Salamence_pokemonbuffer,sizeof(Salamence_pokemonbuffer));
    SendBufferTCP(sock,ReceiveBuffer,sizeof(ReceiveBuffer));

    return 0;*/
}


