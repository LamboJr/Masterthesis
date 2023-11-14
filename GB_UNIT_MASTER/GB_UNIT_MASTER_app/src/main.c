/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
//#include "decode_trainer_data.h"
#include "decode_Pokemon_structure.h"

typedef enum {
	IdleState,  // State for Phase before both gAmeboys are ready to communicate
	handshakeState,	//State for handshake between GBs. Phase if only one GB is Ready till the sny signal 8FFF is sen by master
	TrainerDataExchangeState,	//first state for exchanging information. general and trainer card information
	TradingRoomMovementState,	// State for Moving around in the trading room
	PokemonDataExchangeState,	//State for exchanging Data about the Team Pokemon after taking Place in the chair

} s_SystemState;

typedef enum{
	IdleDataExchangeState,
	TrainerData,
	EmptyState1,
	SLOT1And2,
	EmptyState2,
	SLOT3And4,
	EmptyState3,
	SLOT5And6,
	FinishedState,
} s_DecodeState;

#define PRINT_DATA_FRAMES

u32 trainerdatabuffer[1000];

u32 MasterMask = 0xFFFF0000;
u32 SlaveMask = 0x0000FFFF;

int main()
{
	u16 PokemonMasterSlot1[50];
	u16 PokemonMasterSlot2[50];
	u16 PokemonMasterSlot3[50];
	u16 PokemonMasterSlot4[50];
	u16 PokemonMasterSlot5[50];
	u16 PokemonMasterSlot6[50];

	u16 PokemonSlaveSlot1[50];
	u16 PokemonSlaveSlot2[50];
	u16 PokemonSlaveSlot3[50];
	u16 PokemonSlaveSlot4[50];
	u16 PokemonSlaveSlot5[50];
	u16 PokemonSlaveSlot6[50];

	u16 PokemonMasterBufferIndex = 0;
	u16 PokemonSlaveBufferIndex = 0;

	u32 *address = NULL;  //address pointer to read/write from/to BRAM controller -> ringbuffer

	u32 MasterDataframeCount = 0; //variable for counting the number of relevant  complete 9er frames !!!
	u32 SlaveDataframeCount = 0; //variable for counting the number of relevant  complete 9er frames !!!

	u32 frameCount = 0;		//count variable for formating the monitoring output and also used for interpreting data
	u32 data;			//variable for storing red in data

	//u32 buffer_index = 0;
	u32 MasterBufferIndex = 0;			//Index variable for indexing the Storage buffers for the Master data frames
	u32 SlaveBufferIndex = 0;			//Index variable for indexing the Storage buffers for the Slave data frames



	//u8 record_data = 0;
	u8 RecordMasterData = 0;	// bool variable. Set to 1 if 8888 is received from the Master so the next 7 data frames are relevant game data
	u8 RecordSlaveData = 0;		// Same for Slave data

	u32 MasterTrainerData[100];	// Storage buffer for Trainer pass info send from master
	u32 SlaveTrainerData[100]; // Storage buffer for Trainer pass info send from Slave
	/*
	u32 MasterPokemonData[1000];	// Storage buffer for Pokemon Data structures
	u32 SlavePokemonData[1000];		//Storage buffer for Pokemon Data structures
	*/

	//struct TrainerCard Trainer_Card;


	s_SystemState s_NextState = IdleState;	//FSM state vaiable

	s_DecodeState s_NextMasterDecodeState = IdleDataExchangeState;
	s_DecodeState s_NextSlaveDecodeState = IdleDataExchangeState;



    address = (u32 *) XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;  // address points to BASE ADDR of AXI BRAM controller

    u32 i = 0;

	//u32 count = 0;
    init_platform();

    print("Start Monitoring \n");



    while(1){
    	if (*(address+1) == 0){ // checks the empty flag of the ringbuffer Hardware to indicate if there is new data to read from HW

    		data = *(address);
    		//reads from the ringbuffer if its not empty
			switch(s_NextState){

			case IdleState:{
				if(data == 0xB9A0B9A0){
					s_NextState = handshakeState;
					frameCount = 0;
					xil_printf("\nEntering Handshake State\n");
				}
			}break;

			case handshakeState:{
				if(data == 0x8FFFB9A0){frameCount = 8;
				s_NextState = TrainerDataExchangeState;
				xil_printf("\nEntering TradeInformationExchange State\n");
				}
			}break;

			case TrainerDataExchangeState: {
				if (frameCount == 0){}//checksum
				else if (frameCount == 1){

					if ( (data & MasterMask) == 0x88880000){
						RecordMasterData = 1;
					}
					else{RecordMasterData = 0;}
					if ( (data & SlaveMask) == 0x00008888){
						RecordSlaveData = 1;
					}
					else{RecordSlaveData= 0;}
				}

				else {
					if (RecordMasterData == 1){
						MasterTrainerData[MasterBufferIndex] = data & MasterMask;
						MasterBufferIndex++;
					}
					if (RecordSlaveData == 1){
						SlaveTrainerData[SlaveBufferIndex] = data & SlaveMask;
						SlaveBufferIndex++;
					}
				}

				if(data == 0xCAFECAFE){
					s_NextState = TradingRoomMovementState;
					//print_trainer_data(MasterTrainerData, Trainer_Card)
				}
			}break;


			case TradingRoomMovementState:{
				if(data == 0x8FFFB9A0){frameCount = 8;
					s_NextState = PokemonDataExchangeState;
					s_NextMasterDecodeState = IdleDataExchangeState;
					xil_printf("\nEntering Pokemon Data Exchange phase\n");
					PokemonMasterBufferIndex = 0;
					MasterBufferIndex = 0;
					SlaveBufferIndex = 0;}
			}break;


			case PokemonDataExchangeState:{
				if (frameCount == 0){}//checksum
				else if (frameCount == 1){

					if ( (data & MasterMask) == 0x88880000){
						RecordMasterData = 1;
						MasterDataframeCount++;
					}
					else{RecordMasterData = 0;}
					if ( (data & SlaveMask) == 0x00008888){
						RecordSlaveData = 1;
						SlaveDataframeCount++;
					}
					else{RecordSlaveData= 0;}

					switch(s_NextMasterDecodeState){
						case IdleDataExchangeState: {
							if (RecordMasterData == 1){
								s_NextMasterDecodeState = TrainerData;
								xil_printf("Master Trainer data frame : \n");
							}
						}break;
						case TrainerData: {
							if (RecordMasterData == 0){
								s_NextMasterDecodeState = EmptyState1;
								xil_printf("Master Entering empty1 \n");
							}

						}break;
						case EmptyState1: {
							if (RecordMasterData == 1){
								s_NextMasterDecodeState = SLOT1And2;
								PokemonMasterBufferIndex = 0;
								xil_printf("Master First two Pokemon Data frames :\n");
							}
						}break;
						case SLOT1And2: {
							if (RecordMasterData == 0){
								s_NextMasterDecodeState = EmptyState2;
								xil_printf(" Master Entering empty2 \n");
							}
						}break;
						case EmptyState2: {
							if (RecordMasterData == 1){
								s_NextMasterDecodeState = SLOT3And4;
								PokemonMasterBufferIndex = 0;
								xil_printf("Master 3 & 4 Pokemon data frames:\n");
							}
						}break;
						case SLOT3And4: {
							if (RecordMasterData == 0){
								s_NextMasterDecodeState = EmptyState3;
								xil_printf(" Master Entering empty3 \n");
							}
						}break;
						case EmptyState3: {
							if (RecordMasterData == 1){
								s_NextMasterDecodeState = SLOT5And6;
								PokemonMasterBufferIndex = 0;
								xil_printf("Master 5 & 6 Pokemon data frames:\n");
							}
						}break;
						case SLOT5And6: {
							if (RecordMasterData == 0){
								s_NextMasterDecodeState = FinishedState;
								xil_printf("Master Entering Finish \n");
							}
						}break;
						case FinishedState: {
							// switch to trading state
						}break;
						default:break;
					}//end switch decode state

					//Slave Decoding
					switch(s_NextSlaveDecodeState){
						case IdleDataExchangeState: {
							if (RecordSlaveData == 1){
								s_NextSlaveDecodeState = TrainerData;
								xil_printf("Slave Trainer data frame : \n");
							}
						}break;
						case TrainerData: {
							if (RecordSlaveData == 0){
								s_NextSlaveDecodeState = EmptyState1;
								xil_printf("Slave Entering empty1 \n");
							}

						}break;
						case EmptyState1: {
							if (RecordSlaveData == 1){
								s_NextSlaveDecodeState = SLOT1And2;
								PokemonSlaveBufferIndex = 0;
								xil_printf("Slave First two Pokemon Data frames :\n");
							}
						}break;
						case SLOT1And2: {
							if (RecordSlaveData == 0){
								s_NextSlaveDecodeState = EmptyState2;
								xil_printf("Slave  Entering empty2 \n");
							}
						}break;
						case EmptyState2: {
							if (RecordSlaveData == 1){
								s_NextSlaveDecodeState = SLOT3And4;
								PokemonSlaveBufferIndex = 0;
								xil_printf("Slave 3 & 4 Pokemon data frames:\n");
							}
						}break;
						case SLOT3And4: {
							if (RecordSlaveData == 0){
								s_NextSlaveDecodeState = EmptyState3;
								xil_printf("Slave Entering empty3 \n");
							}
						}break;
						case EmptyState3: {
							if (RecordSlaveData == 1){
								s_NextSlaveDecodeState = SLOT5And6;
								PokemonSlaveBufferIndex = 0;
								xil_printf("Slave 5 & 6 Pokemon data frames:\n");
							}
						}break;
						case SLOT5And6: {
							if (RecordSlaveData == 0){
								s_NextSlaveDecodeState = FinishedState;
								decode_Pokemon_data(PokemonMasterSlot1);
								decode_Pokemon_data(PokemonMasterSlot2);
								decode_Pokemon_data(PokemonSlaveSlot1);
								decode_Pokemon_data(PokemonSlaveSlot2);
								xil_printf("Slave Entering Finish \n");
							}
						}break;
						case FinishedState: {
							// switch to trading state
						}break;
						default:break;
					}//end switch decode state


				}
				else {//frame count > 2 relevant data frames

					switch(s_NextMasterDecodeState){
					case TrainerData:{
						//xil_printf("Trainer data frame : \n");
						MasterBufferIndex = 0;
						MasterBufferIndex = 0;
						SlaveBufferIndex = 0;
					}break;
					case SLOT1And2:{
						if (RecordMasterData == 1){
							if ( PokemonMasterBufferIndex <50){
								PokemonMasterSlot1[PokemonMasterBufferIndex] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else if ((PokemonMasterBufferIndex >= 50) && (PokemonMasterBufferIndex <100) ){
								PokemonMasterSlot2[PokemonMasterBufferIndex-50] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else{}
						}
					}break;
					case SLOT3And4:{
						if (RecordMasterData == 1){
							if ( PokemonMasterBufferIndex <50){
								PokemonMasterSlot3[PokemonMasterBufferIndex] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else if ((PokemonMasterBufferIndex >= 50) && (PokemonMasterBufferIndex <100) ){
								PokemonMasterSlot4[PokemonMasterBufferIndex-50] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else{}
						}
					}break;
					case SLOT5And6:{
						if (RecordMasterData == 1){
							if ( PokemonMasterBufferIndex <50){
								PokemonMasterSlot5[PokemonMasterBufferIndex] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else if ((PokemonMasterBufferIndex >= 50) && (PokemonMasterBufferIndex <100) ){
								PokemonMasterSlot6[PokemonMasterBufferIndex-50] = ((data & MasterMask) >> 16);
								PokemonMasterBufferIndex++;
							}
							else{}
						}
					}break;
					case FinishedState:{

					}break;
					default:break;

					}//end switch decode state

					switch(s_NextSlaveDecodeState){
					case TrainerData:{
						//xil_printf("Trainer data frame : \n");
						SlaveBufferIndex = 0;
					}break;
					case SLOT1And2:{
						if (RecordSlaveData == 1){
							if ( PokemonSlaveBufferIndex <50){
								PokemonSlaveSlot1[PokemonSlaveBufferIndex] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else if ((PokemonSlaveBufferIndex >= 50) && (PokemonSlaveBufferIndex <100) ){
								PokemonSlaveSlot2[PokemonSlaveBufferIndex-50] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else{}
						}
					}break;
					case SLOT3And4:{
						if (RecordSlaveData == 1){
							if ( PokemonSlaveBufferIndex <50){
								PokemonSlaveSlot3[PokemonSlaveBufferIndex] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else if ((PokemonSlaveBufferIndex >= 50) && (PokemonSlaveBufferIndex <100) ){
								PokemonSlaveSlot4[PokemonSlaveBufferIndex-50] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else{}
						}
					}break;
					case SLOT5And6:{
						if (RecordSlaveData == 1){
							if ( PokemonSlaveBufferIndex <50){
								PokemonSlaveSlot5[PokemonSlaveBufferIndex] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else if ((PokemonSlaveBufferIndex >= 50) && (PokemonSlaveBufferIndex <100) ){
								PokemonSlaveSlot6[PokemonSlaveBufferIndex-50] = ((data & SlaveMask));
								PokemonSlaveBufferIndex++;
							}
							else{}
						}
					}break;
					case FinishedState:{

					}break;
					default:break;

					}//end switch decode state

				}

			}break;



			default:break;
			}//end switch system state

#ifdef PRINT_DATA_FRAMES
			xil_printf("0x%08x    ",data);
#endif
			if ((frameCount % 8 == 0) && (frameCount != 0)){xil_printf("\n");frameCount = 0;}
			else{frameCount++;}

    	}
    }
    cleanup_platform();
    return 0;
}

