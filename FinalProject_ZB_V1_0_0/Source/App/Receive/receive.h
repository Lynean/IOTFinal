/*
 * receive.h
 *
 *  Created on: 20-05-2024
 *      Author: Windows
 */

#ifndef SOURCE_MID_RECEIVE_RECEIVE_H_
#define SOURCE_MID_RECEIVE_RECEIVE_H_



#endif /* SOURCE_MID_RECEIVE_RECEIVE_H_ */

#include "app/framework/include/af.h"
typedef enum{
	BOARD0,
	BOARD1
}ledNum_t;

typedef struct{
	ledNum_t led;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t mode;
}colorPWM_t;

typedef struct{
	uint16_t attributeID;
	uint8_t dataType;
	uint8_t value;
} __attribute__((packed)) OnOffPayload_t;

typedef struct{
	uint16_t attributeID;
	uint8_t dataType;
	uint8_t value;
} __attribute__((packed)) ColorControlPayload_t;

typedef struct{
	uint16_t attributeID;
	uint8_t dataType;
	uint8_t Minute;
	uint8_t Hour;
	uint8_t Room;
	uint8_t reserved;
} __attribute__((packed)) TimePayload_t;

void RECEIVE_OnOffCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t DEP, uint8_t* buffer, uint8_t payloadStartIndex);
void RECEIVE_ColorControlCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t* buffer, uint8_t payloadStartIndex);
void RECEIVE_TimeCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t* buffer, uint8_t payloadStartIndex);
