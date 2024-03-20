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

static u32 FrameCounter = 1;		//count variable for formating the monitoring output and also used for interpreting data


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


void UpdateFrameCounter(){
	if (FrameCounter == FRAME_LENGTH){
			FrameCounter = 1;
	}else{
			FrameCounter++;
	}
	return;
}
void PrintNewline(FILE *fp){

	if ((FrameCounter % 9 == 0) && (FrameCounter != 0)){
	printf("\n");

	fprintf(fp,"\n");

	}
	return;
}

void PrintFrame(u32 data, FILE *fp){
		printf("%04x:%04x    ",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		fprintf(fp,"%04x:%04x    ",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		if ((FrameCounter == 1) || (FrameCounter == 2)){
			printf("|    ");
			fprintf(fp,"|    ");
		}
	return;
}

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
							decode_Pokemon_data(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex]);
							decode_Pokemon_data(s_Tradehandler->PokemonTeam[s_Tradehandler->TeamIndex+1]);
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




