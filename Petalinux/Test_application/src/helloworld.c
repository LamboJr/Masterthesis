/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define u32 uint32_t

#define XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR 0x40000000U

#define PORT 8080
#define MAX_PENDING_CONNECTIONS 5
#define BUFFER_SIZE 1024

/*
#define PL_TO_PS_BUFFER_DEVICE_ID XPAR_AXI_GPIO_PLTOPS_DEVICE_ID
#define PL_TO_PS_BUFFER_CHANNEL 1
#define PL_TO_PS_BUFFER_MASK 0xFFFFFFFF

#define PS_TO_PL_BUFFER_DEVICE_ID XPAR_AXI_GPIO_PSTOPL_DEVICE_ID
#define PS_TO_PL_BUFFER_CHANNEL 1
#define PS_TO_PL_BUFFER_MASK 0xFFFFFFFF

#define DATA_DIRECTION_OUTPUT 0
#define DATA_DIRECTION_INPUT 1
*/

#define ADDR_OFFSET_EMPTY 1

int main_hello() {

	/*XGpio_Config *cfg_ptr;
	XGpio PL_TO_PS_BUFFER_Device;
	XGpio PS_TO_PL_BUFFER_Device;*/

	/*//Initialize the PL to PS buffer
	cfg_ptr = XGpio_LookupConfig(PL_TO_PS_BUFFER_DEVICE_ID);
	XGpio_CfgInitialize(&PL_TO_PS_BUFFER_Device, cfg_ptr, cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&PL_TO_PS_BUFFER_Device, PL_TO_PS_BUFFER_CHANNEL, DATA_DIRECTION_INPUT);
	//Data Direction = 1 for input

	cfg_ptr = XGpio_LookupConfig(PS_TO_PL_BUFFER_DEVICE_ID);
	XGpio_CfgInitialize(&PS_TO_PL_BUFFER_Device, cfg_ptr, cfg_ptr->BaseAddress);
	XGpio_SetDataDirection(&PS_TO_PL_BUFFER_Device, PS_TO_PL_BUFFER_CHANNEL, DATA_DIRECTION_OUTPUT);
*/

	u32 __attribute__ ((unused)) *BRAM_CNTL_address = NULL;  //address pointer to read/write from/to BRAM controller -> ringbuffer

	BRAM_CNTL_address = (u32 *) XPAR_AXI_BRAM_CTRL_0_S_AXI_BASEADDR;  // address points to BASE ADDR of AXI BRAM controller
	u32 data;			//variable for storing red in data

	printf("Start Program 6\n");
/*	while(1){
		if ((*(BRAM_CNTL_address+ADDR_OFFSET_EMPTY) & 0x1) == 0){ // checks the empty flag of the ringbuffer Hardware to indicate if there is new data to read from HW

		   //reads data from Ringbuffer to get latest received data package
		   data = *(BRAM_CNTL_address);
		   printf("Data : %d \n",data);
		   //Responds to the receives master packet with a own packet
		   *(BRAM_CNTL_address) = data;
		}
	}*/


    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    const char *hello = "Hello from server";

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket address settings
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to localhost and the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, MAX_PENDING_CONNECTIONS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    // Read data from the client
    read(new_socket, buffer, BUFFER_SIZE);
    printf("Client: %s\n", buffer);

    // Send data to the client
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    return 0;
}
