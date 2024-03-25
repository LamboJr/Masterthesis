



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "Includes/DataTypes.h"
#include "Includes/Link.h"
#include "Includes/ConCat.h"
#include <pthread.h>
#include "Includes/TCP_Server.h"


#define PORT 8080
#define MAX_PENDING_CONNECTIONS 5
#define TEXT_BUFFER_SIZE 1024
#define DATA_BUFFER_SIZE 50
#define SERVER_ADDRESS "10.0.0.4"

#define ADDR_OFFSET_EMPTY 1

int server_fd, new_socket;
/*
 * This function initialises the Apllication as a Tcp Client.
 * The function tries to establish a connection to the IP address 10.0.0.4 at Port 8080
 */

int TCP_Client_Init(){
    //int new_socket = 0;
    struct sockaddr_in serv_addr;
    char buffer[TEXT_BUFFER_SIZE] = {0};
    const char *hello = "Hello from client Zybo Board ";

    // Create socket
    if ((new_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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
    if (connect(new_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // Send data to server
    send(new_socket, hello, strlen(hello), 0);

    //send(sock, Salamence_pokemonbuffer, sizeof(Salamence_pokemonbuffer),0);
    //printf("Buffer sent with %ld Bytes\n",sizeof(Salamence_pokemonbuffer));

    // Read data from server
    read(new_socket, buffer, TEXT_BUFFER_SIZE);
    printf("Server: %s\n", buffer);
    return 0;
}


/*
 * Initialises the Application as a TCP Server, reachable at IP address 10.0.0.2 at Port 8080
 */
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
/*
 * This function is design to run in a separate thread
 * It manages the exchnage of the two Pokemon data structures send within one data block request with a 0xCCCC frame.
 * It Sends and Receives the assigned ID of the Exchange und Send a 200 Byte data buffer and received a 200 Byte data buffer viva TCP
 *
 */
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

//Similiar function to the one befor,
// Can be used for any kind of other datablocks.
//The data size of the exchanged data buffers is configurable
//It also exchanges IDs
//Designed to be run as a separate thread

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

/*
 * Function exchange assigned IDs without exchanging a buffer
 * Threaded function
 */
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


//Send ID.
//Not meant to be threadeed
//testing purpuse only

void SendID(u8 ID){
	u8 IDPtr[1] = {ID};
	printf("Send TCP Connection ID %d\n",*IDPtr);
	send(new_socket,IDPtr,sizeof(IDPtr),0);
	return;
}
//receive ID, not meant to be threated, testing only
u8 ReceiveID(){
	u8 RecvID[1] = {0};
	read(new_socket,RecvID,sizeof(RecvID));
	printf("Received ID : %d\n",*RecvID);
	return *RecvID;
}

// Sendthe Teamindex of the chosen Pokemon for th trade, with acknowledgement
//not meant to be threated, testing only
void SendTeamIndex(u8 TeamIndex){
	u8 TeamIndexptr[1] = {TeamIndex};
	printf("Send Team Index %d of Size %d Bytes\n",TeamIndex,sizeof(TeamIndexptr));
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	s8 RecTeamIndex[1];
	read(new_socket,RecTeamIndex,sizeof(RecTeamIndex));
	printf("Client received Team Index %d \n",*RecTeamIndex);
	return;
}
// Receive the Teamindex of the chosen Pokemon for the trade, with acknowledgment
//not meant to be threaded, testing only
u8 ReceiveTeamIndex(){
	u8 TeamIndexptr[1];
	read(new_socket,TeamIndexptr,sizeof(TeamIndexptr));
	printf("Received Team Index : %d of Size %d Bytes\n",*TeamIndexptr,sizeof(TeamIndexptr));
	printf("Send Team Index %d ACK\n",*TeamIndexptr);
	send(new_socket,TeamIndexptr,sizeof(TeamIndexptr),0);
	return TeamIndexptr[0];
}
/*
 * Send Buffer over TCp connection
 * not meant to be threaded
 * with acknowledgment
 * testing only
 */
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
/*
 * receive Buffer over TCp connection
 * not meant to be threaded
 * with acknowledgment
 * testing only
 */
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
