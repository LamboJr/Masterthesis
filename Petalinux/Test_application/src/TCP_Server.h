int TCP_Server_Init();

void SendBufferTCP(u16 *BufferArg,size_t BufferArgSize);

void ReceiveBufferTCP(u16* BufferArg,size_t BufferArgSize);

void *NewConnectionServer(void *ptr);




void SendTeamIndex(u8 TeamIndex);
u8 ReceiveTeamIndex();
