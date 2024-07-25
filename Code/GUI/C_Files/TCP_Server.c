#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h>

#include "TCP_Server.h"




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


int TCP_Client_Init(void){

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
    return sock;


}




int TCP_Server_Init(void) {

    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    //u16 buffer[DATA_BUFFER_SIZE] = {0};
    char textbuffer[BUFFER_SIZE] = {0};
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


    // Send data to the client
    send(new_socket, hello, strlen(hello), 0);
    printf("Connected to Client via TCP \n");


    // Read data from the client
    read(new_socket,textbuffer,BUFFER_SIZE);
    /*read(new_socket, buffer, sizeof(buffer));
    for(int i = 0; i < DATA_BUFFER_SIZE;i++){
    	//printf("Index[%d] = %04x\n",i,buffer[i]);
    }*/
    printf("Detected Client with message : %s\n",textbuffer);
    return new_socket;
}