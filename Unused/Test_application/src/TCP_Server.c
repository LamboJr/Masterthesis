

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Includes/DataTypes.h"
#include "TCP_Server.h"
#include "Includes/Link.h"
#include "Includes/ConCat.h"
#include <pthread.h>

#define PORT 8080
#define MAX_PENDING_CONNECTIONS 5
#define TEXT_BUFFER_SIZE 1024
#define DATA_BUFFER_SIZE 50


#define ADDR_OFFSET_EMPTY 1

int server_fd, new_socket;


int TCP_Server_Init() {


    //int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    //u16 buffer[DATA_BUFFER_SIZE] = {0};
    char textbuffer[TEXT_BUFFER_SIZE] = {0};
    const char *hello = "Hello from server";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket address settings
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to localhost and the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    printf("Listing for new connection ... \n");
    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Accept the incoming connection \n");
    printf("Please connect Client PC to the Board and start the software\n");
    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("New socket accepted\n");



    // Read data from the client
    read(new_socket,textbuffer, TEXT_BUFFER_SIZE);
    /*read(new_socket, buffer, sizeof(buffer));
    for(int i = 0; i < DATA_BUFFER_SIZE;i++){
    	//printf("Index[%d] = %04x\n",i,buffer[i]);
    }*/
    printf("Detected Client with message : %s\n",textbuffer);

    // Send data to the client
    send(new_socket, hello, strlen(hello), 0);
    printf("Connected to Client via TCP \n");

    return 0;
}

void *ExchangePokemonBufferTCP(void *ptr){
    // Accept an incoming connection
	extern ThreadStatus Threadstatus;
	ThreadData *Tdata= (ThreadData *)ptr;
	u8 ID = Tdata->Connect_ID;
	u8 TeamIndex = Tdata->TeamIndex;
	Threadstatus = Running;printf("Tread is running\n");
	extern u16 PokemonTeamBuffer[3][100];
	extern TradeHandler s_TradeHandlerMaster;
	extern u16 ReceivedTeam[6][50];
	SendID(ID);
	u8 RecvID = ReceiveID();
	if(ID == RecvID){
	//if(*IdPtr == RecvID){
		Tdata->MatchingID = SameID;
		printf("Correct ID \n");
	}else{
		Tdata->MatchingID = DifferentID;
		printf("Wrong ID Out of Sync\n");
	}
	SendTeamIndex(TeamIndex);
	SendBufferTCP(s_TradeHandlerMaster.PokemonTeam[TeamIndex], sizeof(s_TradeHandlerMaster.PokemonTeam[1]));
	SendBufferTCP(s_TradeHandlerMaster.PokemonTeam[(TeamIndex)+1], sizeof(s_TradeHandlerMaster.PokemonTeam[1]));

	ReceiveBufferTCP(ReceivedTeam[TeamIndex],sizeof(ReceivedTeam[1]));
	ReceiveBufferTCP(ReceivedTeam[(TeamIndex)+1],sizeof(ReceivedTeam[1]));

	concat(ReceivedTeam[TeamIndex],ReceivedTeam[(TeamIndex)+1], PokemonTeamBuffer[(TeamIndex)/2],sizeof(ReceivedTeam[1]), sizeof(ReceivedTeam[1]));
	printf("100 Byte Buffer:\n");
//	for(int i = 0;i < 100;i++){
//		printf("%04x ",s_TradeHandlerMaster.PokemonTeamBuffer[TeamIndex/2][i]);
//		if(i%9 == 0){printf("\n");}
//	}
	Threadstatus = Finished;printf("Thread is finished\n");

	pthread_exit(NULL);
}
void *ExchangeBufferTCP(void *arg){
	extern ThreadStatus Threadstatus;
	Threadstatus = Running;
	printf("Tread is running\n");
	ThreadData *Tdata= (ThreadData *)arg;
	u8 ID = Tdata->Connect_ID;
	SendID(ID);
	u8 RecvID = ReceiveID();
	if(ID == RecvID){
			Tdata->MatchingID = SameID;printf("Correct ID \n");
		}else{
			Tdata->MatchingID = DifferentID;printf("Wrong ID Out of Sync\n");
	}
	size_t BufferSize = Tdata->ArgBufferSize;
	SendBufferTCP(Tdata->ArgBufferPtr, BufferSize);
	extern u16 ReceiveBuffer[110];
	ReceiveBufferTCP(ReceiveBuffer, BufferSize);
	printf("Sent %d Bytes\n",BufferSize);

	Threadstatus = Finished;printf("Thread is finished\n");

	pthread_exit(NULL);
}


void *ExchangeIDTCP(void *arg){
	extern ThreadStatus Threadstatus;
	Threadstatus = Running;printf("Tread is running\n");
	ThreadData *Tdata= (ThreadData *)arg;
	u8 ID = Tdata->Connect_ID;
	SendID(ID);
	u8 RecvID = ReceiveID();
	if(ID == RecvID){
	//if(*IdPtr == RecvID){
		Tdata->MatchingID = SameID;
		printf("Correct ID \n");
	}else{
		Tdata->MatchingID = DifferentID;
		printf("Wrong ID Out of Sync\n");
	}
	Threadstatus = Finished;
	printf("Thread is finished\n");
	return NULL;
}

void SendID(u8 ID){
	u8 IDPtr[1] = {ID};
	printf("Send TCP Connection ID %d\n",*IDPtr);
	send(new_socket,IDPtr,sizeof(IDPtr),0);
	return;
}
u8 ReceiveID(){
	u8 RecvID[1] = {0};
	read(new_socket,RecvID,sizeof(RecvID));
	printf("Received ID : %d\n",*RecvID);
	return *RecvID;
}

void SendTeamIndex(u8 TeamIndex){
	u8 TeamIndexptr[1] = {TeamIndex};
	printf("Send Team Index %d of Size %d Bytes\n",TeamIndex,sizeof(TeamIndexptr));
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	s8 RecTeamIndex[1];
	read(new_socket,RecTeamIndex,sizeof(RecTeamIndex));
	printf("Client received Team Index %d \n",*RecTeamIndex);
	return;
}

u8 ReceiveTeamIndex(){
	u8 TeamIndexptr[1];
	read(new_socket,TeamIndexptr,sizeof(TeamIndexptr));
	printf("Received Team Index : %d of Size %d Bytes\n",*TeamIndexptr,sizeof(TeamIndexptr));
	printf("Send Team Index %d ACK\n",*TeamIndexptr);
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	return TeamIndexptr[0];
}

void SendBufferTCP(u16 *BufferArg,size_t BufferArgSize){

	printf("Send Buffer with size %d Bytes over Ethernet\n",BufferArgSize);
    for(int i= 0;i < BufferArgSize/sizeof(u16);i++){
        printf("%04x  ",BufferArg[i]);
        if(((i%10) == 0) && (i!=0)) {
            printf("\n");
        }
    }
    printf("\n");
	send(new_socket, BufferArg,BufferArgSize,0);

	s8 RecPokemonbuffer[1];
	read(new_socket,RecPokemonbuffer,sizeof(RecPokemonbuffer));
	printf("Client received Pokemon Buffer %d\n",*RecPokemonbuffer);


	return;
}

void ReceiveBufferTCP(u16* BufferArg,size_t BufferArgSize){

	read(new_socket, BufferArg, BufferArgSize);
	//read(new_socket, ReceiveBuffer, sizeof(ReceiveBuffer));
	for(int i = 0; i < BufferArgSize/2;i++){
    	printf("%04x ",BufferArg[i]);
    	if(((i%10)==0) && (i!=0)){
    		printf("\n");
    	}
	}printf("\n");
	u8 AckPtr[1];
	*AckPtr = 1;
	send(new_socket,AckPtr,sizeof(AckPtr),0);
	printf("Send Ack \n");

	printf("Finished receving Buffer\n");

}