/*
 * stmcom.c
 *
 *  Created on: Sep 7, 2024
 *      Author: Admin
 */
#include "Source\Mid\mid.h"
#include "Source\Driver\UART\uart.h"


uint8_t hour,minute, room;
uint8_t r, g, b;

STMComEventHandle STMCom_ReceivedCallback;
void STMCom_Init(STMComEventHandle USER_Callback){
	USART_initGPIO();
	initUSART2(UARTBufferPop);
	STMCom_ReceivedCallback = USER_Callback;
}
void UARTBufferPop(receiveCommon_t* inBufBase){
	receiveCommon_t* common;
	common = inBufBase;
	uint8_t receivedData;
	receivedData = common->data;
	if (((common->startByte == 0xB1)) && (common->stopByte == 0xFF)){
		STMCom_ReceivedCallback(1,common->cmdID, receivedData);
		USART2_ResetBuffer();
	}
}



