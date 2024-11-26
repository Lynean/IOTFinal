/*
 * stmcom.h
 *
 *  Created on: Sep 7, 2024
 *      Author: Admin
 */

#ifndef SOURCE_MID_STMCOM_STMCOM_H_
#define SOURCE_MID_STMCOM_STMCOM_H_



#endif /* SOURCE_MID_STMCOM_STMCOM_H_ */

#include "app/framework/include/af.h"

typedef enum {
	Minute,
	Hour,
	Room,
	Red,
	Green,
	Blue,
	ActiveMode,
	Off
}MessageType_t;


typedef struct {
	uint8_t startByte;
	MessageType_t cmdID;
	uint8_t data;
	uint8_t stopByte;
}__attribute__((packed)) receiveCommon_t;

typedef void (*STMComEventHandle)(uint8_t ,uint8_t, uint8_t);
void STMCom_Init(STMComEventHandle USER_Callback);
void UARTBufferPop(receiveCommon_t* inBufBase);
