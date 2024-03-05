### Entering Seqeunce
Requirement : Both Systems are turned on and are in Game.
Results:
Master searches for connection sending 0xB9A0 data packet

If Slave not ready yet( not searching for connection ):
Slave answers with 0xFFF8 data Packets

if Slave is ready ( Seraching for connection) :
Slave answers with 0xB9A0 data packet

Master recognizes this and shows this on the screen.
Player needs to confirm the connection by pressing A a few times.
If this connection is conformid by the player, Master send one 0x8FFF data packet

Slave just sends 0xB9A0 packet
System leaving the handhake State
System entering the Data exchange state
Higher thorughput of data packets at the sam time.
9 data packets instead of 1 in one "Frame" 


### Moving Sequence



### Make Pokemon Trade Choice 

Master dont send anything until Slave responds. Master responds then after Slave send its Tradecommand
Slave send Tradecommand 0xAABB with TradecommandData (0-5) (Pokemon Spotnumber in Slave team)
Master responds with  Tradecommand 0xDDDD with TradecommandData ( 0-5) (Pokemon Spotnumber in Master team)



### Canceling Trade Choice
bbcc bbcc
ddee
 MAster/ Slave send their TradeDommand 0xBBCC. The order doesn't matter. Packets are send when button is pressed 

 If both Packets are send and received.

 Master sends Tradecommand 0xDDEE. Slave doesn't send this TRadecommand back.

 ### Confirm Trade

 Master / Slave send theis TradeCommanfd 0xBBBB8 (not the Controlcommand !!). The order doesn't matter. Packets are send when corresponding Button is pressed

 if both packets are send and received

 Master send TradeCoammnd 0xCCDD. Slave doesn't respond to this TradeCommand back.


