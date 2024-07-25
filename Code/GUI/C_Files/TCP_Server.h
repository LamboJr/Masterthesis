#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_T

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_PENDING_CONNECTIONS 5
//#define SERVER_ADDRESS "192.168.178.114"
#define SERVER_ADDRESS "10.0.0.2"

#define START_ID 0
#define HANDSHAKE_ID 1
#define EXCHANGE_POKEMON_DATA_ID 2
#define EXCHANGE_TRAINER_DATA_ID 3
#define EXCHANGE_LETTER_DATA_ID 4
#define EXCHANGE_RIBBON_DATA_ID 5
#define EXCHANGE_EXIT_0_ID 6
#define EXCHANGE_EXIT_20_ID 7
#define EXCHANGE_EXIT_C_ID 8
#define SEND_INITIAL_BLOCK_ID 9
#define SEND_TRADE_COMMAND_ID 10


void SendBufferTCP(int new_socket,uint16_t *BufferArg,size_t BufferArgSize);
void ReceiveBufferTCP(int new_socket,uint16_t* BufferArg,size_t BufferArgSize);
uint8_t ReceiveTeamIndex(int new_socket);
void SendTeamIndex(int new_socket,uint8_t TeamIndex);
void SendID(int new_socket,u8 ID);
u8 ReceiveID(int new_socket);
int TCP_Server_Init();