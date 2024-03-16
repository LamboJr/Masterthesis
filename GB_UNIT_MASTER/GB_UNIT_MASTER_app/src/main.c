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
#include <stdint.h>
#include <stdlib.h>

#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xil_types.h"
#include "xparameters.h"

#include "MonitorTrade.h"
#include "TradeHandler.h"

#include "TestDataTrade.h"



#define MASTERMASK 0xFFFF0000
#define SLAVEMASK 0x0000FFFF

#define SWITCH1(buffer) (buffer) & 0x1
#define SWITCH2(buffer) (buffer) & 0x2
#define SWITCH3(buffer) (buffer) & 0x4
#define SWITCH4(buffer) (buffer) & 0x8

//AXI GPIO
#define PL_TO_PS_BUFFER_DEVICE_ID XPAR_AXI_GPIO_PLTOPS_DEVICE_ID
#define PL_TO_PS_BUFFER_CHANNEL 1
#define PL_TO_PS_BUFFER_MASK 0xFFFFFFFF

#define PS_TO_PL_BUFFER_DEVICE_ID XPAR_AXI_GPIO_PSTOPL_DEVICE_ID
#define PS_TO_PL_BUFFER_CHANNEL 1
#define PS_TO_PL_BUFFER_MASK 0xFFFFFFFF

#define DATA_DIRECTION_OUTPUT 0
#define DATA_DIRECTION_INPUT 1

#define ADDR_OFFSET_EMPTY 1


#define MODE_TRADE 0
#define MODE_MONITOR 1

#define TRADE
#define USE_TEST_DATA

int main(){

	//Config of the AXI Gpio Controller
	XGpio_Config *cfg_ptr;
	XGpio PL_TO_PS_BUFFER_Device;
	XGpio PS_TO_PL_BUFFER_Device;

	//Initialize the PL to PS buffer
	cfg_ptr = XGpio_LookupConfig(PL_TO_PS_BUFFER_DEVICE_ID);
	XGpio_CfgInitialize(&PL_TO_PS_BUFFER_Device, cfg_ptr, cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL, DATA_DIRECTION_INPUT);
	//Data Direction = 1 for input

	cfg_ptr = XGpio_LookupConfig(PS_TO_PL_BUFFER_DEVICE_ID);
	XGpio_CfgInitialize(&PS_TO_PL_BUFFER_Device, cfg_ptr, cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&PS_TO_PL_BUFFER_Device, PS_TO_PL_BUFFER_CHANNEL, DATA_DIRECTION_OUTPUT);

	u32 __attribute__ ((unused)) *address = NULL;  //address pointer to read/write from/to BRAM controller -> ringbuffer

    address = (u32 *) XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;  // address points to BASE ADDR of AXI BRAM controller

	u32 data;			//variable for storing red in data

	u32 PL_to_PS_buffer_value;

	//u32 Mode = MODE_MONITOR;
	u32 Mode;

    init_platform();
    u16 result;

    u32 RingbufferValue;

#undef USE_TEST_DATA
#ifdef TRADE
#ifdef USE_TEST_DATA

    xil_printf("Trade Test Data\n");
    u32 sizearray = (sizeof(TestDataTrade1)/(sizeof(u32)));
    xil_printf("Size of Array : %ld\n",sizearray);
    for(size_t i = 0;i< sizearray;i++){
    	PL_to_PS_buffer_value = XGpio_DiscreteRead(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL);
    	data = TestDataTrade1[i] >> 16;
    	result = TradeHandler_func(data,PL_to_PS_buffer_value);
    	xil_printf("%04x : %04x | ",data,result);
    }
    print("\n\nFinished test loop \n\n");


#else

    Mode = MODE_TRADE;
    XGpio_DiscreteWrite(&PS_TO_PL_BUFFER_Device, PS_TO_PL_BUFFER_CHANNEL,Mode);
    xil_printf("Start Trading emulator\n");

    address = (u32 *) XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;

    while(1){
    	PL_to_PS_buffer_value = XGpio_DiscreteRead(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL);

    	if ((*(address+ADDR_OFFSET_EMPTY) & 0x1) == 0){ // checks the empty flag of the ringbuffer Hardware to indicate if there is new data to read from HW

    		//reads data from Ringbuffer to get latest received data package
    		data = *(address);
    		*(address) = data;
    		//Responds to the receives master packet with a own packet
    		//*(address) = TradeHandler_func(data,PL_to_PS_buffer_value);

    	}

    }
#endif;


#else
    Mode = MODE_MONITOR;
    u32 dump;
#ifdef USE_TEST_DATA
        print("Start Monitoring \n");
    dump = 1;
    dump = XGpio_DiscreteRead(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL);
    u32 sizearray = (sizeof(TestDataTrade)/(sizeof(u32)));
    xil_printf("Size of Array : %ld\n",sizearray);
    for(size_t i = 0;i< sizearray;i++){
    	data = TestDataTrade[i];
    	N_updateFSM(data, dump);
    }
    print("\n\nFinished test loop \n\n");

#else
    XGpio_DiscreteWrite(&PS_TO_PL_BUFFER_Device, PS_TO_PL_BUFFER_CHANNEL,Mode);
    while(1){

    	if (*(address+ADDR_OFFSET_EMPTY) == 0){ // checks the empty flag of the ringbuffer Hardware to indicate if there is new data to read from HW
    		data = *(address);
    		//reads from the ringbuffer if its not empty
    		dump = XGpio_DiscreteRead(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL);
    		N_updateFSM(data,dump);

    	}
    }
#endif
#endif
    cleanup_platform();
    return 0;
}

