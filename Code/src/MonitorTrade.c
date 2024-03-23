#include "Includes/DataTypes.h"
#include <stdio.h>


#include "Includes/Link.h"
#include "Includes/structs.h"

#include "Includes/linkcommand.h"
#include "Includes/decodeTrainderCard.h"
#include "Includes/DecodePokemonData.h"
#include "Includes/MonitorTrade.h"
#include "Includes/constant.h"

static struct TradeHandler sTradeHandler[2];

static u32 FrameCounter = 1;
//count variable for formating the monitoring output and also used for interpreting data
//Counts the number of data pakets in the current 9er frame. Values are in range 1-9


/*---------------------------------------------------------------------------------
MonitorHandler is the most important function to the Monitoring Unit
MonitorHandler implements a State machine called s_NextState.
This State machine is used to detect the state of the communication.
It has 4 states : HandshakeState, DataState, RequestBlockState and RequestExitstate
---------------
|HAndshakeState:
---------------
The state machine compares the value of the master packet with the Value 0x8FFF
If this Value is detected, the communication changes from the handshake state to the
data state
---------------
|DataState:
---------------
In this State the data send from both master and slave are analyzed.
The packets send from the master and slave are covert separately
Both the master and the slave tradehandler have a state variable with two states:
Data identification and BlockSendState
	-----------------------------------------------------------------------------------------
	Data identification is the state where the control-field of the data frame gets analyzed
	Depending on the control field the state machine is updated with the corresponding state
	BlockSendState : is the state when one of the two (Master or Slave) sends a  Data block on its own
	There is no CCCC Control-field for this block
	--------------------------------------------------------------------------------------------
-------------------
|BlockRequestState:
-------------------
If the MAster detects a Block Request at the control field ( 0xCCCC), the state of the nextState
machine is set to this state. A Blockrequest indicated, that both master and Slave send some kind of data Block with a
specific size. If both Master and Slave have send the requested Block, which is also stored in a buffer by the program,
The State machine returns to the data state.
-------------------
|RequestExitState:
-------------------
If both the MASter and the Slave send a 0x5FFF control field, the state machine switches to this state.
A 0x5FFF Control field indicated the transition to the Handshake State.
In this state, the program waits for the master to send the first packet of handshake phase and then switches to the
handshake phase.

----------------------------------------------------------------------------------*/
void MonitorHandler(u32 data,u32 dump,FILE *fp){

	//static s_SystemState s_NextState = TransitionState;	//FSM state vaiable
	static SyncState s_NextState = handshakeState;
	//if(checkFrameDuration()==1){return;}

	PrintFrame(data, fp);
	switch(s_NextState){
	case handshakeState:{
		FrameCounter = FRAME_LENGTH ;
		if(GET_MASTERDATA(data) == MASTER_HANDSHAKE){
			printf("\n");
			s_NextState = DataState;

			TradehandlerReset(&sTradeHandler[MASTER]);
			TradehandlerReset(&sTradeHandler[SLAVE]);
			//printf("Entering Trading State\n");
		}
	}break;
	case DataState:{

		if(sTradeHandler[MASTER].s_NextdataState == IdendificationState){
			switch(FrameCounter){
			case 1:{
				if ( GET_MASTERDATA(data) != GET_SLAVEDATA(data) ){
					printBlankspace("Checksum not matching\n");
				}
			}break;//case1 FrameCounter
			case 2:{
			//----------------------------------------------------------------------
			//Check control frames which are sent. F.E 2222,BBBB,CCCC,5FFF,2FFE,CAFE,
			sTradeHandler[MASTER].LinkCMD = GET_MASTERDATA(data);
			switch(sTradeHandler[MASTER].LinkCMD){
				case LINKCMD_SEND_LINK_TYPE:{//2222
					printBlankspace("Send Link Type\n");
					sTradeHandler[MASTER].BlockRequest = Active;
					sTradeHandler[SLAVE].BlockRequest = Active;
					sTradeHandler[MASTER].DataFieldActive = NoRequest;
					sTradeHandler[MASTER].BufferIndex = 0;
					sTradeHandler[SLAVE].DataFieldActive  = NoRequest;
					sTradeHandler[SLAVE].BufferIndex = 0;
				}break;
				case LINKCMD_READY_CLOSE_LINK:{//5FFF
					sTradeHandler[MASTER].ReadyCloseLink = 1;
					if((sTradeHandler[MASTER].ReadyCloseLink && sTradeHandler[SLAVE].ReadyCloseLink) == 1){
						s_NextState = RequestExitState;
						printBlankspace("Ready close Link -> Transition state\n");
						sTradeHandler[MASTER].ReadyCloseLink = 0;
						sTradeHandler[SLAVE].ReadyCloseLink = 0;
					}
				}break;
				case LINKCMD_SEND_BLOCK_REQ:{//CCCC
					printBlankspace("Send Block Request\n");
					//printf("Send Block Request\n");
					sTradeHandler[MASTER].BlockRequest = Active;
					sTradeHandler[SLAVE].BlockRequest = Active;
					sTradeHandler[MASTER].DataFieldActive = NoRequest;
					sTradeHandler[MASTER].BufferIndex = 0;
					sTradeHandler[SLAVE].DataFieldActive  = NoRequest;
					sTradeHandler[SLAVE].BufferIndex = 0;
				}break;
				case LINKCMD_INIT_BLOCK:{//BBBB
					printBlankspace("Init Block\n");
					sTradeHandler[MASTER].BufferIndex = 0;
					sTradeHandler[MASTER].DataFieldActive  = NoRequest;
				}break;
				default:{}break;
				}//end switch

			}break;//case2 Controllfield FrameCounter
			//Case3 -------------------------------------------------------------------------------
			case 3:{
				switch(sTradeHandler[MASTER].LinkCMD){
				case LINKCMD_SEND_BLOCK_REQ:{//CCCC
					sTradeHandler[MASTER].BlockRequestSize = GET_MASTERDATA(data);
					sTradeHandler[SLAVE].BlockRequestSize = GET_MASTERDATA(data);
					s_NextState = BlockRequestState;
					//printf("RequestBlockSize : %d \n",sTradeHandler[MASTER].BlockRequestSize);
					//printBlankspace("");
				}break;
				case LINKCMD_SEND_LINK_TYPE:{//2222
					sTradeHandler[MASTER].LinkType = GET_MASTERDATA(data);
					sTradeHandler[MASTER].BlockRequestSize = BLOCK_SIZE_60;
					sTradeHandler[SLAVE].BlockRequestSize = BLOCK_SIZE_60;
					s_NextState = BlockRequestState;
				}break;
				case LINKCMD_INIT_BLOCK:{//BBBB
					sTradeHandler[MASTER].s_NextdataState = BlockSendState;
					sTradeHandler[MASTER].BlockRequestSize = BLOCK_SIZE_20;
					//printf("InitBlockSize : %d \n",sTradeHandler[MASTER].InitBlockSize);
				}break;
				default:{}break;
				}//end switch Master case 2

			}break;//case2 FrameCounter
			default:{}break;
			}//end switch case statemachine
			//------------------------------------------------------------------------------------------


		//if DataStateState Master == BlockSendState
		}else{
			if (MonitorBlockRequestAnalyser(FrameCounter, GET_MASTERDATA(data), &sTradeHandler[MASTER]) == Granted){
				sTradeHandler[MASTER].s_NextdataState = IdendificationState;
			}
		}
//-----------------------------------------------------------------------------------------------------------------------
//-------------------------------------SLAVE-----------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
		if(sTradeHandler[SLAVE].s_NextdataState == IdendificationState){
			switch(FrameCounter){
				case 1:{
					if ( GET_MASTERDATA(data) != GET_SLAVEDATA(data) ){
						printBlankspace("Checksum not matching\n");
					}
				}break;//case1 FrameCounter
				case 2:{
					sTradeHandler[SLAVE].LinkCMD = GET_SLAVEDATA(data);
					switch(sTradeHandler[SLAVE].LinkCMD){
					case LINKCMD_READY_CLOSE_LINK:{//5FFF
						sTradeHandler[SLAVE].ReadyCloseLink = 1;
						if((sTradeHandler[MASTER].ReadyCloseLink && sTradeHandler[SLAVE].ReadyCloseLink) == 1){
							s_NextState = RequestExitState;
							printBlankspace("Ready close Link -> Transition state\n");
							sTradeHandler[MASTER].ReadyCloseLink = 0;
							sTradeHandler[SLAVE].ReadyCloseLink = 0;
						}
					}break;
					case LINKCMD_INIT_BLOCK:{//BBBB
						sTradeHandler[SLAVE].BufferIndex = 0;
						sTradeHandler[SLAVE].DataFieldActive  = NoRequest;
					}break;
					default:{}break;
					}
				}break;//case2 Controllfield FrameCounter
				case 3:{
					switch(sTradeHandler[SLAVE].LinkCMD){
					case LINKCMD_INIT_BLOCK:{//BBBB
						sTradeHandler[SLAVE].BlockRequestSize = BLOCK_SIZE_20;
						sTradeHandler[SLAVE].s_NextdataState = BlockSendState;
					}break;
					default:{}break;
					}//end switch Slave case 2

				}break;//case2 FrameCounter
				default:break;
			}//end switch case statemachine

		}
		else{
			if (MonitorBlockRequestAnalyser(FrameCounter, GET_SLAVEDATA(data), &sTradeHandler[SLAVE]) == Granted ){
				sTradeHandler[SLAVE].s_NextdataState = IdendificationState;
			}
		}


	}break;

	case BlockRequestState : {
		if ( sTradeHandler[MASTER].BlockRequest == Active){
			if (MonitorBlockRequestAnalyser(FrameCounter, GET_MASTERDATA(data), &sTradeHandler[MASTER]) == Granted){
				sTradeHandler[MASTER].BlockRequest = Granted;
			}
		}
		if (sTradeHandler[SLAVE].BlockRequest == Active){
			if (MonitorBlockRequestAnalyser(FrameCounter, GET_SLAVEDATA(data), &sTradeHandler[SLAVE]) == Granted ){
				sTradeHandler[SLAVE].BlockRequest = Granted;
			}
		}
		if((sTradeHandler[MASTER].BlockRequest == Granted) && (sTradeHandler[SLAVE].BlockRequest == Granted)){
			s_NextState = DataState;
			//printf("Exit BlockRequest Handler\n");
		}

	}break;

	case RequestExitState:{
		if(GET_MASTERDATA(data) == SLAVE_HANDSHAKE){
			s_NextState = handshakeState;
			//printf("Entering Handshake state\n");
			FrameCounter = FRAME_LENGTH;
		}
		//printf("In Transition ");

	}break;

	default:{}break;
	}//end switch NexState Variable
	PrintNewline(fp);
	UpdateFrameCounter();
	return;
}

void printBlankspace(char* text){
#ifdef DEBUG_STATES
	char* blankspace = "                    \t\t";
	printf("%s",text);
	for(int i = 0;i<FrameCounter;i++){
		printf("%s",blankspace);
	}

#endif
}

/*----------------------------------------------------------------
 This function simply increments the framecounter by 1. If the framecounter is 9 it is set back to 1
 This refers to the 9 datapakets send during one frame
 */
void UpdateFrameCounter(){
	if (FrameCounter == FRAME_LENGTH){
			FrameCounter = 1;
	}else{
			FrameCounter++;
	}
	return;
}
/*
 * Prints a Newline Character if the Framecounter is 9, which means a Frame is finished
 */
void PrintNewline(FILE *fp){

	if ((FrameCounter % 9 == 0) && (FrameCounter != 0)){
	printf("\n");

	fprintf(fp,"\n");

	}
	return;
}

/*
 * Formated print of the Master and Slave datapakets in the format : Masterpaket:Slavepaket
 * The Value is printed as an hexadecimal number
 * The controlfield of the frame (framecounter = 2) is marked with two bars | Controlfield |
 */

void PrintFrame(u32 data, FILE *fp){
		printf("%04x:%04x    ",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		fprintf(fp,"%04x:%04x    ",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		if ((FrameCounter == 1) || (FrameCounter == 2)){
			printf("|    ");
			fprintf(fp,"|    ");
		}
	return;
}

/*
 * restes all values and attributes of the Tradehandler struct.
 */

void TradehandlerReset(TradeHandler* sTradeHandler){
	sTradeHandler->ReadyCloseLink = 0;
	sTradeHandler->TeamIndex = 0;
	sTradeHandler->BlockRequestSize = 0;
	sTradeHandler->InitBlockSize = 0;
	sTradeHandler->BlockRequest = 0;
	sTradeHandler->DataFieldActive = NoRequest;
	sTradeHandler->BufferIndex = 0;
	sTradeHandler->s_NextdataState = IdendificationState;
	return;
}

/*
 * This Function handles the Tracking and Analysis of the Datablock
 * It waits for the actual data block, marked with a 0x8888 controlfield to start
 * As long as the data block continues, depending on the BlockRequestSize detected earlier
 * the received data is stored in a corresponding buffer.
 * After all the valid data packets of the  data block request is detected and stored in a buffer,
 * The functions sets the returnvalue to granted, to indicate that the Send of the data block is completely handled.
 * If the Blockrequest had the BlockRequestSize 200 ( indicating two Pokemon data structures)
 * the two stored Pokemon data buffers are decoded with the function PrintDecodedPokemonData()
 *
 */
RequestStatus MonitorBlockRequestAnalyser(u32 Framecounter,u16 data, TradeHandler* s_Tradehandler){
		//Function which safes the received Pokemondata(Master) in a corresponding data structure

		if (Framecounter == 1){//ignore
		}
		else if( Framecounter == 2){
			if( data == 0x8888){
				s_Tradehandler->DataFieldActive = Active;
			}
			else{
				s_Tradehandler->DataFieldActive  = NoRequest;
			}
		}
		else{
			//printf("DF : %d ", s_Tradehandler.DataFieldActive);
			if ( s_Tradehandler->DataFieldActive == Active){

				switch(s_Tradehandler->BlockRequestSize){

				case BLOCK_REQ_SIZE_200 :{
					if ( s_Tradehandler->BufferIndex < 50 ){
						s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex][s_Tradehandler->BufferIndex++] = data;
					}else if ((s_Tradehandler->BufferIndex >= 50) && (s_Tradehandler->BufferIndex < 100)){
						s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex+1][(s_Tradehandler->BufferIndex++)-50] = data;
					}else{
						if( Framecounter == 9){
							s_Tradehandler->DataFieldActive  = NoRequest;
							s_Tradehandler->BufferIndex = 0;
							//PrintDecodedPokemonData(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex]);
							//PrintDecodedPokemonData(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex+1]);
							PrintDecodedPokemonDataToFile(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex]);
							PrintDecodedPokemonDataToFile(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex+1]);
							s_Tradehandler->TeamIndex = (s_Tradehandler->TeamIndex + 2);
							if(s_Tradehandler->TeamIndex > 5){
								s_Tradehandler->TeamIndex = 0;
							}
							//printf("Block Analyser finished\n");
							return Granted;
						}
					}
				}break;
				case BLOCK_REQ_SIZE_100 : {
					if ( s_Tradehandler->BufferIndex < 50 ){
						s_Tradehandler->TrainerCard[s_Tradehandler->BufferIndex++] = data;
					}else{
						if( Framecounter == 9){
							s_Tradehandler->DataFieldActive  = NoRequest;
							s_Tradehandler->BufferIndex = 0;
							//printf("Block Analyser finished\n");
							return Granted;
						}
					}
				}break;

				case BLOCK_REQ_SIZE_220 : {
					if(s_Tradehandler->BufferIndex < 110){
						s_Tradehandler->LetterBuffer[s_Tradehandler->BufferIndex++] = data;
					}else{
						if( Framecounter == 9){
							s_Tradehandler->DataFieldActive  = NoRequest;
							s_Tradehandler->BufferIndex= 0;
							//printf("Block Analyser finished\n");
							return Granted;
						}
					}
				}break;
				case BLOCK_REQ_SIZE_40 : {
					if(s_Tradehandler->BufferIndex < 20){
						s_Tradehandler->RibbonsBuffer[s_Tradehandler->BufferIndex++] = data;
					}
					else{
						if( Framecounter == 9){
							s_Tradehandler->DataFieldActive  = NoRequest;
							s_Tradehandler->BufferIndex = 0;
							//printf("Block Analyser finished\n");
							return Granted;
						}
					}
				}break;
				case BLOCK_SIZE_60 : {
					if(s_Tradehandler->BufferIndex < 30){
						s_Tradehandler->InitialBuffer[s_Tradehandler->BufferIndex++] = data;
					}
					else{
						if(Framecounter == 9){
							s_Tradehandler->DataFieldActive  = NoRequest;
							s_Tradehandler->BufferIndex = 0;
							//printf("Block Analyser finished\n");
							return Granted;
						}
					}
				}break;
				case BLOCK_SIZE_20 : {
						if ( s_Tradehandler->BufferIndex < 10 ){
								s_Tradehandler->TradeCommandBuffer[s_Tradehandler->BufferIndex++] = data;
						}else{
							if( Framecounter == 9){
								s_Tradehandler->DataFieldActive  = NoRequest;
								s_Tradehandler->BufferIndex = 0;
								//printf("Init Block Analyser Finished\n");
								return Granted;
							}
						}
				}break;
				default : {

				}break;
				}//end switch;
			}
		}
		return Pending;
	}




