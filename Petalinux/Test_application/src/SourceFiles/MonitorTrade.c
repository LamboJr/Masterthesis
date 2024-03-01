#include "../Includes/DataTypes.h"
#include <stdio.h>
#include "../Includes/MonitorTrade.h"

#include "../Includes/Link.h"

#include "../Includes/linkcommand.h"
#include "../Includes/decodeTrainderCard.h"
#include "../Includes/DecodePokemonData.h"

static struct TradeHandler sTradeHandler[2];

static u32 frameCount = 1;		//count variable for formating the monitoring output and also used for interpreting data


void MonitorHandler(u32 data,u32 dump,FILE *fp){

	static s_SystemState s_NextState = TransitionState;	//FSM state vaiable


	//if(checkFrameDuration()==1){return;}

	switch(s_NextState){
	case IdleState:{
		if ( (dump & 0x1) == 1){
			printf("%04x:%04x  \n",GET_MASTERDATA(data),GET_SLAVEDATA(data));
			fprintf(fp,"%04x:%04x  \n",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		}
		if(GET_MASTERDATA(data) == MASTER_HANDSHAKE){
			s_NextState = TradingState;
			frameCount = 0;
			for(u32 i = MASTER;i<= SLAVE;i++){
				sTradeHandler[i].ReadyCloseLink = 0;
				sTradeHandler[i].DataIndex = 0; sTradeHandler[i].TeamIndex = 0;
				sTradeHandler[i].BlockRequestSize = 0;
				sTradeHandler[i].InitBlockSize = 0;
				sTradeHandler[i].InitBlock = 0;
				sTradeHandler[i].BlockRequest = 0;
			}
			printf("Entering Trading State\n");
		}
	}break;
	case TradingState:{

		switch(frameCount){
				case 0:{
					if ( GET_MASTERDATA(data) != GET_SLAVEDATA(data) ){
						printBlankspace("Checksum not matching\n");
					}
				}break;//case0 framecount


				case 1:{
					//----------------------------------------------------------------------
					//Check control frames which are sent. F.E 2222,BBBB,CCCC,5FFF,2FFE,CAFE,
					sTradeHandler[MASTER].LinkCMD = GET_MASTERDATA(data);
					sTradeHandler[SLAVE].LinkCMD = GET_SLAVEDATA(data);

					switch(sTradeHandler[MASTER].LinkCMD){
						case LINKCMD_SEND_LINK_TYPE:{//2222
							printBlankspace("Send Link Type\n");
						}break;
						case LINKCMD_READY_CLOSE_LINK:{//5FFF
							sTradeHandler[MASTER].ReadyCloseLink = 1;
							if((sTradeHandler[MASTER].ReadyCloseLink && sTradeHandler[SLAVE].ReadyCloseLink) == 1){
								s_NextState = TransitionState;
								printBlankspace("Ready close Link -> Transition state\n");
								sTradeHandler[MASTER].ReadyCloseLink = 0;
								sTradeHandler[SLAVE].ReadyCloseLink = 0;
							}
						}break;
						case LINKCMD_SEND_BLOCK_REQ:{//CCCC
							printBlankspace("Send Block Request\n");
							sTradeHandler[MASTER].BlockRequest = 1;
							sTradeHandler[SLAVE].BlockRequest = 1;
						}break;
						case LINKCMD_INIT_BLOCK:{//BBBB
							printBlankspace("Init Block\n");
							sTradeHandler[MASTER].DataIndex = 0;
							sTradeHandler[MASTER].InitBlock = 1;
						}break;
						case LINKCMD_CONT_BLOCK:{//8888
							//implement
							printBlankspace("Cont Block\n");
							sTradeHandler[SLAVE].ContBlock = 1;
						}break;
						case LINKCMD_SEND_HELD_KEYS:{//CAFE

						}break;
						case 0x0000:{
							sTradeHandler[MASTER].DataIndex = 0;
						}break;
						default:break;
					}//end switch
					//---------------------------------------------------------------------------
					switch(sTradeHandler[SLAVE].LinkCMD){
					case LINKCMD_READY_CLOSE_LINK:{//5FFF
						sTradeHandler[SLAVE].ReadyCloseLink = 1;
						if((sTradeHandler[MASTER].ReadyCloseLink && sTradeHandler[SLAVE].ReadyCloseLink) == 1){
							s_NextState = TransitionState;
							printBlankspace("Ready close Link -> Transition state\n");
							sTradeHandler[MASTER].ReadyCloseLink = 0;
							sTradeHandler[SLAVE].ReadyCloseLink = 0;
						}
					}break;
					case LINKCMD_INIT_BLOCK:{//BBBB
						if (sTradeHandler[MASTER].LinkCMD != LINKCMD_INIT_BLOCK){
							printBlankspace("Init Block\n");
						}
						sTradeHandler[SLAVE].DataIndex = 0;
						sTradeHandler[SLAVE].InitBlock = 1;

					}break;
					case LINKCMD_CONT_BLOCK:{//8888
						//implement
						if (sTradeHandler[MASTER].LinkCMD != LINKCMD_CONT_BLOCK){
							printBlankspace("Cont Block \n");
							sTradeHandler[SLAVE].ContBlock = 1;
						}
					}break;
					case 0x0000:{
						sTradeHandler[SLAVE].DataIndex = 0;
					}break;
					default:break;
					}
				}break;//case1 framecount
				//--------------------------------------------------------------------------------------

				//Case2 -------------------------------------------------------------------------------
				case 2:{
					switch(sTradeHandler[MASTER].LinkCMD){
					case LINKCMD_SEND_BLOCK_REQ:{//CCCC
						sTradeHandler[MASTER].BlockRequestSize = GET_MASTERDATA(data);
						sTradeHandler[SLAVE].BlockRequestSize = GET_MASTERDATA(data);
						//printf("RequestBlockSize : %d \n",sTradeHandler[MASTER].BlockRequestSize);
						//printBlankspace("");
					}break;
					case LINKCMD_SEND_LINK_TYPE:{//2222
						sTradeHandler[MASTER].LinkType = GET_MASTERDATA(data);
					}break;
					case LINKCMD_INIT_BLOCK:{//BBBB
						sTradeHandler[MASTER].InitBlockSize = GET_MASTERDATA(data);
						//printf("InitBlockSize : %d \n",sTradeHandler[MASTER].InitBlockSize);
						//printBlankspace("");
					}break;
					case LINKCMD_CONT_BLOCK:{//8888
						updateBuffer(MASTER,data);
					}break;

					default:break;
					}//end switch Master case 2
					//-------------------------------------------------------------------------------------
					switch(sTradeHandler[SLAVE].LinkCMD){
					case LINKCMD_SEND_BLOCK_REQ:{//CCCC
						//Should not occur

					}break;
					case LINKCMD_INIT_BLOCK:{//BBBB
						sTradeHandler[SLAVE].InitBlockSize = GET_SLAVEDATA(data);
					}break;
					case LINKCMD_CONT_BLOCK:{
						//process data

						updateBuffer(SLAVE, data);
					}break;
					default:break;
					}//end switch Slave case 2


				}break;//case2 framecount
				//------------------------------------------------------------------------------------------
				case 3 ... 8:{
					if (sTradeHandler[MASTER].LinkCMD == LINKCMD_CONT_BLOCK){
						updateBuffer(MASTER,data);
					}
					if (sTradeHandler[SLAVE].LinkCMD == LINKCMD_CONT_BLOCK){

						updateBuffer(1,data);
					}
				}break;
				default:break;
				}//end switch case statemachine
				//------------------------------------------------------------------------------------------
				printDataFrame(dump,data,fp);
	}break;
	case TransitionState:{
		if(GET_MASTERDATA(data) == SLAVE_HANDSHAKE){
			s_NextState = IdleState;
			printf("Entering Handshake state\n");
			frameCount = 0;
		}
		//printf("In Transition ");
		printDataFrame(dump,data,fp);
	}break;

	default:break;
	}//end switch NexState Variable
	return;
}

void updateBuffer(u8 MSC,u32 data){
	//printf("InitBlock : %d,BlockRequest : %d, TeamIndex : %d,BlockRequestSize :%04x",sTradeHandler[MSC].InitBlock,sTradeHandler[MSC].BlockRequest,sTradeHandler[MSC].TeamIndex,sTradeHandler[MSC].BlockRequestSize);
	if(sTradeHandler[MSC].InitBlock == 1 ){ //Checks if Block was initializes with BBBB
		if(sTradeHandler[MSC].BlockRequest == 1){ //Checks if Block was Requested from master with CCCC
			if(sTradeHandler[MSC].TeamIndex <6 && sTradeHandler[MSC].BlockRequestSize == 1){ //Checks if Pokemon structure was Requested

				if(sTradeHandler[MSC].DataIndex < (sTradeHandler[MSC].InitBlockSize/2)){
					//Cehcks if Master or Slave Buffer has to be updated
					if(MSC == MASTER){	sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_MASTERDATA(data);}
					else{	sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_SLAVEDATA(data);}
					//printf("Debug:%d ",sTradeHandler[MSC].DataIndex);
					sTradeHandler[MSC].DataIndex++;  //Increase buffer index after succesfully storing data in it
				}else{
					//If Buffer is full
					sTradeHandler[MSC].InitBlock  = 0;
					//printf("\n");
					//Copy temporary buffer in the coresponding Teambuffer in the struct field
					for(size_t i =0;i<POKEMON_BUFFER_LENGTH;i++){
						sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i] = sTradeHandler[MSC].TempBuffer[i];
						//printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
					}
					//PRINTMS(MSC)
					//printf("Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);
/*					if(MSC == MASTER){printf("Master Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}
					else{printf("Slave Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}*/
					//decode_Pokemon_data(sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex]);
					sTradeHandler[MSC].TeamIndex++; //update TeamIndex
					for(size_t i = 0;i<POKEMON_BUFFER_LENGTH;i++){
						sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i] = sTradeHandler[MSC].TempBuffer[i+50];
						//printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
					}
					//PRINTMS(MSC)
					//printf("Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);
					//printf("DEBUG\n");
					//decode_Pokemon_data(sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex]);
					sTradeHandler[MSC].TeamIndex++;

					//There are always send two Pokemon at once
					printBlankspace("");
					if( sTradeHandler[MSC].TeamIndex >= 6){
						//Reset the control signals
						sTradeHandler[MSC].TeamIndex = 0;
						sTradeHandler[MSC].BlockRequest= 0;
					}
				}
			}//Teamindex <6 && BlockRequestsize == 1
			else if (sTradeHandler[MSC].BlockRequestSize == 2){
				if(sTradeHandler[MSC].DataIndex < (sTradeHandler[MSC].InitBlockSize/2)){
							//Cehcks if Master or Slave Buffer has to be updated
							if(MSC == MASTER){
								sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_MASTERDATA(data);
							}
							else{
								sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_SLAVEDATA(data);
							}
							//printf("Debug:%d ",sTradeHandler[MSC].DataIndex);
							sTradeHandler[MSC].DataIndex++;  //Increase buffer index after succesfully storing data in it
				}else{
					sTradeHandler[MSC].InitBlock  = 0;
					//printf("\n");
					//Copy temporary buffer in the coresponding Teambuffer in the struct field
					for(size_t i =0;i<TRAINER_BUFFER_SIZE;i++){
						sTradeHandler[MSC].TrainerCard[i] = sTradeHandler[MSC].TempBuffer[i];
						//printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
					}
					if(MSC == MASTER){//printf("Master TrainerCard\n");

					}
					else{//printf("Slave TrainerCard\n");

					}
					//decodeTrainerCard(sTradeHandler[MSC].TrainerCard);


					//There are always send two Pokemon at once
					printBlankspace("");
						//Reset the control signals
					sTradeHandler[MSC].BlockRequest= 0;

				}// end If data index small enough
			}//Block REquestSize == 2
			else if(sTradeHandler[MSC].BlockRequestSize == 3){
				sTradeHandler[MSC].BlockRequest= 0;
			}//end Block Request Size == 3
			else if (sTradeHandler[MSC].BlockRequestSize == 4){
				sTradeHandler[MSC].BlockRequest= 0;
			}//end  Block Request Size = 4

		}//end BlockRequest == 1
		else{
			//Case for Trading sync data
			if(sTradeHandler[MSC].InitBlockSize == 20){
				if(sTradeHandler[MSC].DataIndex < (sTradeHandler[MSC].InitBlockSize/2)){
					if ( MSC == MASTER){sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_MASTERDATA(data);}
					else{sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_SLAVEDATA(data);}
					sTradeHandler[MSC].DataIndex++;
				}else{
					sTradeHandler[MSC].InitBlock  = 0;
					//printf("\n");
					sTradeHandler[MSC].LinkCMD = sTradeHandler[MSC].TempBuffer[0];
					//PRINTMS(MSC)
					//printf("Command : %04x\n",sTradeHandler[MSC].LinkCMD);

//					switch (sTradeHandler[MSC].LinkCMD){
//						CASEPRINT(LINKCMD_READY_TO_TRADE)
//						CASEPRINT(LINKCMD_READY_FINISH_TRADE)
//						CASEPRINT(LINKCMD_READY_CANCEL_TRADE)
//						CASEPRINT(LINKCMD_START_TRADE)
//						CASEPRINT(LINKCMD_CONFIRM_FINISH_TRADE)
//						CASEPRINT(LINKCMD_SET_MONS_TO_TRADE)
//						CASEPRINT(LINKCMD_PLAYER_CANCEL_TRADE)
//						CASEPRINT(LINKCMD_REQUEST_CANCEL)
//						CASEPRINT(LINKCMD_BOTH_CANCEL_TRADE)
//						CASEPRINT(LINKCMD_PARTNER_CANCEL_TRADE)
//						CASEPRINT(LINKCMD_NONE)
//
//						default:break;
//					}//end switch Link CMD
					printBlankspace("");
				}
			}

		}//end No Block Request
	}//InitBlock ==1
	return;
}





void printBlankspace(char* text){
#ifdef DEBUG_STATES
	char* blankspace = "                    \t\t";
	printf("%s",text);
	for(int i = 0;i<frameCount;i++){
		printf("%s",blankspace);
	}

#endif
}
void printDataFrame(u32 dump,u32 data,FILE *fp){
	if ( (dump & 0x1) == 1){
		printf("%04x:%04x \t",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		fprintf(fp,"%04x:%04x \t",GET_MASTERDATA(data),GET_SLAVEDATA(data));
		if ((frameCount == 1) || (frameCount == 0)){
			printf("|\t");
			fprintf(fp,"|\t");
		}
	}

	if ((frameCount % 8 == 0) && (frameCount != 0)){
		if ( (dump & 0x1) == 1){
			printf("\n");
			fprintf(fp,"\n");
		}
		frameCount = 0;
	}
	else{
		frameCount++;
	}
	return;
}

