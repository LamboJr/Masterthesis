//xil_printf("\n Master Trainer buffer \n");
/*for ( i = 0; i <= buffer_index; i++) {
	xil_printf("0x%08x    ",trainerdatabuffer[i]);
	if ((i % 9== 0 ) && (i != 0)){xil_printf("\n");}
}*/
/*for (i = 0; i <= MasterBufferIndex; i++){
	xil_printf("0x%08x    ",MasterTrainerData[i]);
	if (((i+1) % 9== 0 )){xil_printf("\n");}
}
xil_printf("\n Slave Trainer buffer \n");
for (i = 0; i <= SlaveBufferIndex; i++){
	xil_printf("0x%08x    ",SlaveTrainerData[i]);
	if (((i+1) % 9== 0 )){xil_printf("\n");}
}
xil_printf("\nTradindRoomMovement\n");

					if (RecordMasterData == 1){
						//Adds the frame number to the entry in the Pokemon buffer
						//MasterPokemonData[MasterBufferIndex] = (MasterDataframeCount << 16);
						MasterPokemonData[MasterBufferIndex] = ((data & MasterMask) >> 16);
						if (frameCount == 2){
							//decode_Pokemon_data(MasterPokemonData[MasterBufferIndex], MasterDataframeCount);
						}
						MasterBufferIndex++;
					}
					else{

					}
					if (RecordSlaveData == 1){


						SlavePokemonData[SlaveBufferIndex] = data & SlaveMask;
						SlaveBufferIndex++;
					}










*/