/*
 * send.h
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */

#ifndef SOURCE_APP_SEND_SEND_H_
#define SOURCE_APP_SEND_SEND_H_



#endif /* SOURCE_APP_SEND_SEND_H_ */

#include "app/framework/include/af.h"

#define MAX_DATA_COMMAND_SIZE 20
#define HC_NETWORK_ADDRESS 0x0000
#define SOURCE_ENDPOINT_PRIMARY 0x01
#define DESTINATION_ENDPOINT 0x01
enum{
	Sleep,
	Study,
	Dining,
	Excercise,
	Living
};

enum{
	Manual,
	Rainbow,
	Storm,
	Fire,
	Aurora,
	Party,
	Warning
};

void SEND_ReportInfoToHc(void);
void SEND_OnOffState(uint8_t endpoint, uint8_t state);
void SEND_LDRMeasurement(uint8_t endpoint,uint16_t value);
void SEND_ColorCommandInitiator(uint8_t EP,uint8_t msgType, uint8_t data);
void SEND_LeaveInfoToHc();
void SEND_SendCommandUnicast(uint8_t source, uint8_t destination, uint16_t address);
