int TCP_Server_Init();

void SendBufferTCP(u16 *BufferArg,size_t BufferArgSize);

void ReceiveBufferTCP(u16* BufferArg,size_t BufferArgSize);

void *ExchangePokemonBufferTCP(void *ptr);

void *ExchangeBufferTCP(void *arg);
void *ExchangeIDTCP(void *ptr);
void SendID(u8 ID);
u8 ReceiveID();

void SendTeamIndex(u8 TeamIndex);
u8 ReceiveTeamIndex();
