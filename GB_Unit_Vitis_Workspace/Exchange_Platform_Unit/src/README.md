# Exchange Platform Unit Instruction
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
The Trading process is similiar to the one descirbed in the Backup_Unit Instruction, accept the exchanges data block are send and received over the Ethernet via TCP and to the database.
At certain points, the Game progress is delayed until the tradepartner catches up to the current state of the tradesequence.
If the Tradepartner makes different choices at certain point in the trading process as you, the trade gets out of sync and should be canceled.
For exmaple if you want enter the trade menu and the partner chooses to leave the room, the communication is out of sync.
But in the normal trade this would result in ending the trade anyway.
Both players should therfore stick to the described trade process.
If both players are in the trade menu and chose their Pokemon they want to trade, they need to press the tradebutton after they set the switches.
The problem is, the trade choice of the master is always send after the trade choice of the slave. Even if the master makes the trade choice before the slave.
Because both GBA are connected as a master to their Zybo Board, this gets a little bit tricky.
So the Player chooses the Pokemon he wants to receive from the partner (spot 0-5) with the Switches 0,1 and 2 on the board. (SW0 = LSB and SW2 = MSB of the number) 
So the switches must be set before the trade button is pressed.
After the button is pressed, both players must confirm their choices again.

## testing
To run all the tests, define
 #define UNIT_TEST
 
 In the funtion RunAllTest(): you need to choose the tests to run and remove the comment.
 Some of the tests require a TCP connection and only work with the appropriate TCP-partner.
The Source file TCP_test.c in /Code can be used and adapted to the specific tests you want to run.
 in the main.c source file and compile the application und execute it again.
 
