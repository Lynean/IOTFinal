/*
 * send.c
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */

#include "Source/Mid/mid.h"
#include "Source/App/app.h"

void SEND_ReportInfoToHc(){
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
		return;
	}

	uint8_t data[MAX_DATA_COMMAND_SIZE];

	uint8_t modelID[4] = {3, 'S', 'W', '1'};
	uint8_t manufacturer[5] = {4, 'L', 'U', 'M', 'I'};
	uint8_t version = 1;

	uint16_t attributeId = ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID;
	data[0] = (uint8_t) (attributeId & 0x00FF);
	data[1] = (uint8_t) ((attributeId & 0xFF00)>>8);
	data[2] = EMBER_SUCCESS ;
	data[3] = (uint8_t) ZCL_CHAR_STRING_ATTRIBUTE_TYPE;

	memcpy(&data[4], (uint8_t*)modelID, 13);

	emberAfFillCommandGlobalClientToServerReadAttributesResponse(ZCL_BASIC_CLUSTER_ID,data, 13+4)
	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);

	attributeId = ZCL_MANUFACTURER_NAME_ATTRIBUTE_ID;
	data[0] = (uint8_t) (attributeId & 0x00FF);
	data[1] = (uint8_t) ((attributeId & 0xFF00)>>8);
	data[2] = EMBER_SUCCESS ;
	data[3] = (uint8_t) ZCL_CHAR_STRING_ATTRIBUTE_TYPE;
	memcpy(&data[4], (uint8_t*)manufacturer, 5);

	emberAfFillCommandGlobalClientToServerReadAttributesResponse(ZCL_BASIC_CLUSTER_ID,data, 5+4)
	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);

	attributeId = ZCL_VERSION_ATTRIBUTE_ID;
	data[0] = (uint8_t) (attributeId & 0x00FF);
	data[1] = (uint8_t) ((attributeId & 0xFF00)>>8);
	data[2] = EMBER_SUCCESS ;
	data[3] = (uint8_t) ZCL_INT8U_ATTRIBUTE_TYPE;
	data[4] = version;

	emberAfFillCommandGlobalClientToServerReadAttributesResponse(ZCL_BASIC_CLUSTER_ID,data, 1+4)
	SEND_SendCommandUnicast(SOURCE_ENDPOINT_PRIMARY, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_OnOffState(uint8_t endpoint, uint8_t state){
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	uint16_t attributeID = ZCL_ON_OFF_ATTRIBUTE_ID;

	data[0] = (uint8_t) (attributeID & 0x00FF);
	data[1] = (uint8_t) ((attributeID & 0xFF00)>>8);
	data[2] = ZCL_INT8U_ATTRIBUTE_TYPE;
	data[3] = state;

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_ON_OFF_CLUSTER_ID, data, 4);
	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_LeaveInfoToHc(){
	uint8_t leaveResponse[9] = {8, 'N', 'W', 'K', 'L', 'E', 'A', 'V', 'E'};
	(void)emberSendZigDevRequest(HC_NETWORK_ADDRESS, LEAVE_RESPONSE, EMBER_APS_OPTION_NONE, leaveResponse, 9);
}

void SEND_ColorCommandInitiator(uint8_t EP,uint8_t msgType, uint8_t data){
	uint8_t writeAttributeRecords[MAX_DATA_COMMAND_SIZE];
	//Set the array to full 0s
	memset(writeAttributeRecords,0, MAX_DATA_COMMAND_SIZE);
	uint16_t attrID;
	switch(msgType){
		case Minute: //Time
			attrID = ZCL_TIME_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_UTC_TIME_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			writeAttributeRecords[4] = 0xFF; // 0x000000[00] = minute; 0x0000[00]00 = hour ; 0x00[00]0000 = room
			writeAttributeRecords[5] = 0xFF;
			writeAttributeRecords[6] = 0xFF;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_TIME_CLUSTER_ID, writeAttributeRecords, 7)
			break;
		case Hour:
			attrID = ZCL_TIME_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_UTC_TIME_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = 0xFF;
			writeAttributeRecords[4] = data; // 0x000000[00] = minute; 0x0000[00]00 = hour ; 0x00[00]0000 = room
			writeAttributeRecords[5] = 0xFF;
			writeAttributeRecords[6] = 0xFF;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_TIME_CLUSTER_ID, writeAttributeRecords, 7)
			break;
		case Room: //Room
			attrID = ZCL_TIME_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_UTC_TIME_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = 0xFF;
			writeAttributeRecords[4] = 0xFF;
			writeAttributeRecords[5] = data;
			writeAttributeRecords[6] = 0xFF;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_TIME_CLUSTER_ID, writeAttributeRecords, 7)
			break;
		case Red:
			attrID = ZCL_COLOR_CONTROL_COLOR_POINT_R_INTENSITY_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_INT8U_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_COLOR_CONTROL_CLUSTER_ID, writeAttributeRecords, 4)
			break;
		case Green:
			attrID = ZCL_COLOR_CONTROL_COLOR_POINT_G_INTENSITY_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_INT8U_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_COLOR_CONTROL_CLUSTER_ID, writeAttributeRecords, 4)
			break;
		case Blue:
			attrID = ZCL_COLOR_CONTROL_COLOR_POINT_B_INTENSITY_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_INT8U_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_COLOR_CONTROL_CLUSTER_ID, writeAttributeRecords, 4)
			break;
		case ActiveMode:
			attrID = ZCL_COLOR_CONTROL_COLOR_MODE_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_ENUM8_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_COLOR_CONTROL_CLUSTER_ID, writeAttributeRecords, 4)
			break;
		case Off:
			attrID = ZCL_ON_OFF_ATTRIBUTE_ID;
			writeAttributeRecords[0] = (uint8_t) (attrID & 0x00FF);
			writeAttributeRecords[1] = (uint8_t) ((attrID & 0xFF00)>>8);
			writeAttributeRecords[2] = ZCL_BOOLEAN_ATTRIBUTE_TYPE;
			writeAttributeRecords[3] = data;
			emberAfFillCommandGlobalClientToServerWriteAttributes(ZCL_ON_OFF_CLUSTER_ID, writeAttributeRecords, 4)
			break;
		default:
			break;
	}
	emberAfCorePrintln("Data: %d %d", msgType, data);
	emberAfSetCommandEndpoints(EP, EP);
	emberAfSendCommandUnicastToBindings();
}


void SEND_SendCommandUnicast(uint8_t source, uint8_t destination, uint16_t address){
	emberAfSetCommandEndpoints(source, destination);
	(void)emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, address);
}

