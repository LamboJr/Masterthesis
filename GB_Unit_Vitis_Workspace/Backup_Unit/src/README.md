###Backup Unit
This Application project functions as a database unit for the gameboy advance with a gen 3 Pokemon Game.
To start this App run  ./Backup_Uni.elf
This app initializes the AXI components and creates respective pointer to acces the AXI components.
## database Access
At the beginning of the application, you have to choose the Pokemon which you want to load from the database.
The program list all stored Pokemon in the database.
The Pokemon have a unique name depending on their personality value
After you successfully chose a Pokemon to be loaded, the Application continues.

## Trade Handling
in order to trade the loaded Pokemon into your Game, you need to start the Trading process.
CHoose the TRdaecenter option and start a trade.
If the board is connected correctly, the GBA should act as if there was another physical GBA connected to it,
which wants to trade which the Player.
You follow the instuctions provided in the Thesis and navigate to the chair in the room, once you entered the room after successful handshake
Once you navigated xour character to the chair, the trademenu will appear.
The player now have the option to select the Pokemon he wants to store in the database.
Choose the Pokemon and then choose the trade option.
## Choose Pokemon
You will see, that the GBA will promt you to wait.
Select the Pokemon you want to  load into your game.
You can choose the Pokemon you loaded from the database in the beginning or a dummy Pokemon.
You can select the Pokemon using the Switches on the Zybo Board ( Switch 0 chooses either the first or the second Pokemon, the other switch must have the value 0)
Once the switches are in the coorect state, press the Tradebutton ( BNTN 1 on the Zybo Z7 Board)
The Game then progresses an you have to confirm the Choices made before.
Once you confirm the Choice, the trade starts.
The Pokemon on your Game is traded with the board an stores in teh data base and you receive the Pokemon loaded from teh Database befor.
## Leave or Repeat
You then either leave the Trade or trade another Pokemon, maybe for a dummy Pokemon. You can only load on Pokemon per Run from the database.
If yoou want to load another one, you have to restart the application and the trading process on the GBA.


# Testitng
To run the test funcitons,
 #define UNIT_TEST
 in the main.c source file and compile the app again.
 After you run it you can seee the test results.
