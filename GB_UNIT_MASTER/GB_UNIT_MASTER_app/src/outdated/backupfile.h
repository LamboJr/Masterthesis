void updateBuffer(u8 MSC,u32 data){
	//xil_printf("InitBlock : %d,BlockRequest : %d, TeamIndex : %d,BlockRequestSize :%04x",sTradeHandler[MSC].InitBlock,sTradeHandler[MSC].BlockRequest,sTradeHandler[MSC].TeamIndex,sTradeHandler[MSC].BlockRequestSize);

	//Check the necessary flags for updating the buffer
	if (sTradeHandler[MSC].InitBlock == 1 && sTradeHandler[MSC].BlockRequest == 1 && sTradeHandler[MSC].TeamIndex <6 && sTradeHandler[MSC].BlockRequestSize == 1){
		//Checks if buffer is full or not
		if(sTradeHandler[MSC].DataIndex < (sTradeHandler[MSC].InitBlockSize/2)){
			//Cehcks if Master or Slave Buffer has to be updated
			if(MSC == MASTER){
				sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_MASTERDATA(data);
			}
			else{
				sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_SLAVEDATA(data);
			}
			//xil_printf("Debug:%d ",sTradeHandler[MSC].DataIndex);
			sTradeHandler[MSC].DataIndex++;  //Increase buffer index after succesfully storing data in it
		}else{
			//If Buffer is full
			sTradeHandler[MSC].InitBlock  = 0;
			xil_printf("\n");
			//Copy temporary buffer in the coresponding Teambuffer in the struct field
			for(size_t i =0;i<POKEMON_BUFFER_LENGTH;i++){
				sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i] = sTradeHandler[MSC].TempBuffer[i];
				//xil_printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
			}
			if(MSC == MASTER){xil_printf("Master Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}
			else{xil_printf("Slave Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}
			decode_Pokemon_data(sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex]);
			sTradeHandler[MSC].TeamIndex++; //update TeamIndex
			for(size_t i = 0;i<POKEMON_BUFFER_LENGTH;i++){
				sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i] = sTradeHandler[MSC].TempBuffer[i+50];
				//xil_printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
			}
			if(MSC == MASTER){xil_printf("Master Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}
			else{xil_printf("Slave Pokemon Spot %d\n",sTradeHandler[MSC].TeamIndex+1);}
			decode_Pokemon_data(sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex]);
			sTradeHandler[MSC].TeamIndex++;

			//There are always send two Pokemon at once
			printBlankspace("");
			if( sTradeHandler[MSC].TeamIndex >= 6){
				//Reset the control signals
				sTradeHandler[MSC].TeamIndex = 0;
				sTradeHandler[MSC].BlockRequest= 0;
			}
		}
	}
	else if(sTradeHandler[MSC].InitBlock == 1 && sTradeHandler[MSC].BlockRequest == 1 && sTradeHandler[MSC].BlockRequestSize == 2){
		if(sTradeHandler[MSC].DataIndex < (sTradeHandler[MSC].InitBlockSize/2)){
					//Cehcks if Master or Slave Buffer has to be updated
					if(MSC == MASTER){
						sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_MASTERDATA(data);
					}
					else{
						sTradeHandler[MSC].TempBuffer[sTradeHandler[MSC].DataIndex] = GET_SLAVEDATA(data);
					}
					//xil_printf("Debug:%d ",sTradeHandler[MSC].DataIndex);
					sTradeHandler[MSC].DataIndex++;  //Increase buffer index after succesfully storing data in it
		}else{
			sTradeHandler[MSC].InitBlock  = 0;
			xil_printf("\n");
			//Copy temporary buffer in the coresponding Teambuffer in the struct field
			for(size_t i =0;i<TRAINER_BUFFER_SIZE;i++){
				sTradeHandler[MSC].TrainerCard[i] = sTradeHandler[MSC].TempBuffer[i];
				//xil_printf("Team[%d][%ld] = %04x\n ",sTradeHandler[MSC].TeamIndex,i,sTradeHandler[MSC].PokemonTeam[sTradeHandler[MSC].TeamIndex][i]);
			}
			if(MSC == MASTER){xil_printf("Master TrainerCard\n");}
			else{xil_printf("Slave TrainerCard\n");}
			decodeTrainerCard(sTradeHandler[MSC].TrainerCard);


			//There are always send two Pokemon at once
			printBlankspace("");
				//Reset the control signals
			sTradeHandler[MSC].BlockRequest= 0;

		}
	}
return;
}
