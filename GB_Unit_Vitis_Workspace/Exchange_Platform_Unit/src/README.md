# Instruction
## TCP Connection
This app is used to communicate and trade with two GBA over the Ethernet.
To do this, a single GBA is connected to Zybo Z7 Board.
Once you start the application, a TCP connection is tried to establish.
The app can be configured as a TCP server or a TCp Client.
In the main function, either the function
TCP_Server_Init(); or
TCP_Client_Init();
is called, depending on the needed configuration. You can change this and compile the app again.
Because the app ist to be configured as a TCP Client anyway for the planed Cloud platform, this is the current configuration.
## Trade process
Once the connection is established, start the trade in the game as decribed in the thesis.
The Trading process is similiar to the normal one, accept the exchanges data block are send and received over the Ethernet via TCP.
The Game progress is delayed until the tradepartner catches up to the current state of the tradesequence.
If the Tradepartner makes different choices at certain point in the trading process as you, the trade gets out of sync and should be canceled.
For exmaple if you want enter the trade menu and the partner chooses to leave the room, the communication is out of sync.
But in the normal trade this would result in ending the trade anyway.
Both players should therfore stick to the described trade process.

## testing
To run all the tests, define
 #define UNIT_TEST
 
 In the funtion RunAllTest(): you need to choose the tests to run and remove the comment.
 Some of the tests require a TCP connection and only work with the appropriate TCP-partner.
The Source file TCP_test.c in /Code can be used and adapted to the specific tests you want to run.
 in the main.c source file and compile the application und execute it again.
 
