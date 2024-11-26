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



#define MAX_DATA_COMMAND_SIZE 40
#define HC_NETWORK_ADDRESS 0x0000
#define SOURCE_ENDPOINT_PRIMARY 0x01
#define DESTINATION_ENDPOINT 0x01

void SEND_ReportInfoToHc(void);
void SEND_OnOffState(uint8_t endpoint, uint8_t state);
void SEND_ColorState(uint8_t endpoint, uint16_t attributeID, uint8_t value);
void SEND_LDRMeasurement(uint8_t endpoint,uint16_t value);
void SEND_TempMeasurement(uint8_t endpoint, int16_t value);
void SEND_HumMeasurement(uint8_t endpoint, uint16_t value);
void SEND_LeaveInfoToHc();
void SEND_SendCommandUnicast(uint8_t source,
									uint8_t destination,
									uint16_t address);
