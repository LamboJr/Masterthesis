#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>
#include "PokemonDataStructure/Pokemonbuffer.h"
 #include "PokemonDataStructure/ReverseDecoding.h"


 #define u8 uint8_t
 #define u16 uint16_t
 #define u32 uint32_T

#define PORT 8080
#define BUFFER_SIZE 1024
//#define SERVER_ADDRESS "192.168.178.114"
#define SERVER_ADDRESS "10.0.0.2"

void SendBufferTCP(int new_socket,uint16_t *BufferArg,size_t BufferArgSize);
void ReceiveBufferTCP(int new_socket,uint16_t* BufferArg,size_t BufferArgSize);
uint8_t ReceiveTeamIndex(int new_socket);
void SendTeamIndex(int new_socket,uint8_t TeamIndex);
void SendID(int new_socket,u8 ID);
u8 ReceiveID(int new_socket);
int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "Hello from client Daniel ";

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_ADDRESS, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    send(sock, hello, strlen(hello), 0);

    //send(sock, Salamence_pokemonbuffer, sizeof(Salamence_pokemonbuffer),0);
    //printf("Buffer sent with %ld Bytes\n",sizeof(Salamence_pokemonbuffer));

    // Read data from server
    read(sock, buffer, BUFFER_SIZE);
    printf("Server: %s\n", buffer);

    u8 Received_iD;
    u8 TeamIndex;
     uint16_t ReceiveBuffer[6][50];
uint16_t ReceivegenericBuffer[110];

 while(1){
    Received_iD=ReceiveID(sock);
    //sleep(1);
    switch(Received_iD){
        case 0 : {//Start
            SendID(sock,Received_iD);
        }break;
        case 1 : {//Handshake
            SendID(sock,Received_iD);
        }break;
        case 2 : {//Pokemon Data
            SendID(sock,Received_iD);
            TeamIndex = ReceiveTeamIndex(sock);
            printf("RECV Team Index = %d\n",TeamIndex);
	        ReceiveBufferTCP(sock,ReceiveBuffer[TeamIndex],sizeof(ReceiveBuffer[1]));
	        ReceiveBufferTCP(sock,ReceiveBuffer[TeamIndex++],sizeof(ReceiveBuffer[1]));

            if(1==1){
                    SendBufferTCP(sock,Salamence_pokemonbuffer,sizeof(Salamence_pokemonbuffer));
                    SendBufferTCP(sock,Latios_pokemonbuffer,sizeof(Latios_pokemonbuffer));
            }else{
                SendBufferTCP(sock,NoPokemon,sizeof(NoPokemon));
                SendBufferTCP(sock,NoPokemon,sizeof(NoPokemon));
            }
        }break;
        case 3 : {//Trainer Data
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,100);
            SendBufferTCP(sock,TrainerCardBuffer,100);
            //SendBufferTCP(sock,ReceivegenericBuffer,100);
        }break;
        case 4 : {//Letter Data
            SendID(sock,Received_iD);
            ReceiveBufferTCP(sock,ReceivegenericBuffer,220);
  
            SendBufferTCP(sock,ReceivegenericBuffer,220);
        }break;
        case 5 : {//Ribbon Data
            SendID(sock,Received_iD);
                        ReceiveBufferTCP(sock,ReceivegenericBuffer,40);
  
            SendBufferTCP(sock,ReceivegenericBuffer,40);
        }break;
        case 6 ... 8: {//Exit
            sleep(5);
            SendID(sock,Received_iD);
        }break;
        case 9:{
            sleep(2);
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

void SendBufferTCP(int new_socket,uint16_t *BufferArg,size_t BufferArgSize){
	uint16_t Sendbuffer[50];
	for(int i = 0; i < 50;i++){
		Sendbuffer[i] = BufferArg[i];
	}
	printf("Send Buffer with size %ld Bytes over Ethernet\n",BufferArgSize);
    for(int i= 0;i < (BufferArgSize/sizeof(uint16_t));i++){
        printf("%04x  ",BufferArg[i]);
        if(((i%10) == 0) && (i!=0)) {
            printf("\n");
        }
    }
    printf("\n");
	//send(new_socket, Sendbuffer,sizeof(Sendbuffer),0);
    send(new_socket,BufferArg,BufferArgSize,0);
	uint8_t RecPokemonbuffer[1];
	read(new_socket,RecPokemonbuffer,sizeof(RecPokemonbuffer));
	printf("Client received Pokemon Buffer %d\n",*RecPokemonbuffer);


	return;
}

void ReceiveBufferTCP(int new_socket,uint16_t* BufferArg,size_t BufferArgSize){

	read(new_socket, BufferArg, BufferArgSize);
	//read(new_socket, ReceiveBuffer, sizeof(ReceiveBuffer));
	for(int i = 0; i < 50;i++){
    	printf("%04x ",BufferArg[i]);
    	if(((i%10)==0) && (i!=0)){
    		printf("\n");
    	}
	}printf("\n");
	uint8_t AckPtr[1];
	*AckPtr = 1;
	send(new_socket,AckPtr,sizeof(AckPtr),0);
	printf("Send Ack \n");

	printf("Finished receving Buffer\n");

}



void SendTeamIndex(int new_socket,uint8_t TeamIndex){
	uint8_t TeamIndexptr[1] = {TeamIndex};
	printf("Send Team Index %d of Size %ld Bytes\n",TeamIndex,sizeof(TeamIndexptr));
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	uint8_t  RecTeamIndex[1];
	read(new_socket,RecTeamIndex,sizeof(RecTeamIndex));
	printf("Client received Team Index %d \n",*RecTeamIndex);
	return;
}

uint8_t ReceiveTeamIndex(int new_socket){
	uint8_t TeamIndexptr[1];
	read(new_socket,TeamIndexptr,sizeof(TeamIndexptr));
	printf("Received Team Index : %d of Size %ld Bytes\n",*TeamIndexptr,sizeof(TeamIndexptr));
	printf("Send Team Index %d ACK\n",*TeamIndexptr);
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	return TeamIndexptr[0];
}
void SendID(int new_socket,u8 ID){
	u8 IDPtr[1] = {ID};
	printf("Send TCP Connection ID %d\n",*IDPtr);
	send(new_socket,IDPtr,sizeof(IDPtr),0);
	return;
}
u8 ReceiveID(int new_socket){
	u8 RecvID[1] = {0};
	read(new_socket,RecvID,sizeof(RecvID));
	printf("Received ID : %d\n",*RecvID);
	return *RecvID;
}
/* //-----------------------------------------------------------------------------------

    uint8_t SendBufferptr[1] = {0x1};
    //uint16_t* SendBufferptr = ((uint16_t* ) Inputdata);
	uint8_t ReceiveBufferptr[1] = {0x0};

    uint16_t ReceivedTeam[6][50];
    uint16_t ReceiveBuffer[50];
    for(int i = 0;i<50;i++){
        ReceiveBuffer[i] = 0;
    }

    read(sock, ReceiveBufferptr, sizeof(ReceiveBufferptr));
    printf("Received : %04x\n",*ReceiveBufferptr);

    send(sock,SendBufferptr,sizeof(SendBufferptr),0);
    printf("Send Teamindex ack\n");

    //printf("Sizeof ReceivedTeam[1] = %ld\n",sizeof(ReceivedTeam[*ReceiveBufferptr]));
    /*read(sock, ReceivedTeam[*ReceiveBufferptr],sizeof(ReceivedTeam[1]));
    for(int i= 0;i < 50;i++){
        printf("%04x  ",ReceivedTeam[*ReceiveBufferptr][i]);
        if(((i%10) == 0) && (i!=0)) {
            printf("\n");
        }
    }
    printf("Sizeof ReceiveBuffer = %ld\n",sizeof(ReceiveBuffer));
    read(sock, ReceiveBuffer,sizeof(ReceiveBuffer));
    for(int i= 0;i < 50;i++){
        printf("%04x  ",ReceiveBuffer[i]);
        if(((i%10) == 0) && (i!=0)) {
            printf("\n");
        }
    }
    *SendBufferptr = 3;
    printf("Send Buffer receive ack\n");
    send(sock,SendBufferptr,sizeof(SendBufferptr),0);
*/