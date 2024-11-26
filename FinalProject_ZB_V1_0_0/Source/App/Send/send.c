/*
 * send.c
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */

#include "Source/App/app.h"

void SEND_ReportInfoToHc(void){
	if(emberAfNetworkState() != EMBER_JOINED_NETWORK){
		return;
	}

	uint8_t data[MAX_DATA_COMMAND_SIZE];

	uint8_t modelID[19] = {18, 'R', 'G', 'B', '1','_','L','M','1','_','T','M','P','1','_','H','M','P','1'};
	uint8_t manufacturer[5] = {4, 'L', 'U', 'M', 'I'};
	uint8_t version = 1;

	uint16_t attributeId = ZCL_MODEL_IDENTIFIER_ATTRIBUTE_ID;
	data[0] = (uint8_t) (attributeId & 0x00FF);
	data[1] = (uint8_t) ((attributeId & 0xFF00)>>8);
	data[2] = EMBER_SUCCESS ;
	data[3] = (uint8_t) ZCL_CHAR_STRING_ATTRIBUTE_TYPE;

	memcpy(&data[4], (uint8_t*)modelID, 19);

	emberAfFillCommandGlobalClientToServerReadAttributesResponse(ZCL_BASIC_CLUSTER_ID,data, 19+4)
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
	data[2] = ZCL_BOOLEAN_ATTRIBUTE_TYPE;
	data[3] = state;

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_ON_OFF_CLUSTER_ID, data, 4);
	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_ColorState(uint8_t endpoint, uint16_t attributeID, uint8_t value){
	uint8_t data[MAX_DATA_COMMAND_SIZE];

	data[0] = (uint8_t) (attributeID & 0x00FF);
	data[1] = (uint8_t) ((attributeID & 0xFF00)>>8);
	data[2] = ZCL_INT8U_ATTRIBUTE_TYPE;
	data[3] = value;

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_COLOR_CONTROL_CLUSTER_ID, data, 4);

	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_LDRMeasurement(uint8_t endpoint, uint16_t value){
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	uint16_t attributeID = ZCL_ILLUM_MEASURED_VALUE_ATTRIBUTE_ID;

	data[0] = (uint8_t) (attributeID & 0x00FF);
	data[1] = (uint8_t) ((attributeID & 0xFF00)>>8);
	data[2] = ZCL_INT16U_ATTRIBUTE_TYPE;
	memcpy(&data[3], (uint8_t*) &value,2);

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_ILLUM_MEASUREMENT_CLUSTER_ID, data, 5);
	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_TempMeasurement(uint8_t endpoint, int16_t value){
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	uint16_t attributeID = ZCL_TEMP_MEASURED_VALUE_ATTRIBUTE_ID;

	data[0] = (uint8_t) (attributeID & 0x00FF);
	data[1] = (uint8_t) ((attributeID & 0xFF00)>>8);
	data[2] = ZCL_INT16S_ATTRIBUTE_TYPE;
	memcpy(&data[3], (uint8_t*) &value,2);

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_TEMP_MEASUREMENT_CLUSTER_ID, data, 5);
	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_HumMeasurement(uint8_t endpoint, uint16_t value){
	uint8_t data[MAX_DATA_COMMAND_SIZE];
	uint16_t attributeID = ZCL_RELATIVE_HUMIDITY_ATTRIBUTE_ID;

	data[0] = (uint8_t) (attributeID & 0x00FF);
	data[1] = (uint8_t) ((attributeID & 0xFF00)>>8);
	data[2] = ZCL_INT16S_ATTRIBUTE_TYPE;
	memcpy(&data[3], (uint8_t*) &value,2);

	emberAfFillCommandGlobalServerToClientReportAttributes(ZCL_RELATIVE_HUMIDITY_MEASUREMENT_CLUSTER_ID, data, 5);
	SEND_SendCommandUnicast(endpoint, DESTINATION_ENDPOINT, HC_NETWORK_ADDRESS);
}

void SEND_LeaveInfoToHc(){
	uint8_t leaveResponse[9] = {8, 'N', 'W', 'K', 'L', 'E', 'A', 'V', 'E'};
	(void)emberSendZigDevRequest(HC_NETWORK_ADDRESS, LEAVE_RESPONSE, EMBER_APS_OPTION_NONE, leaveResponse, 9);
}

void SEND_SendCommandUnicast(uint8_t source,
									uint8_t destination,
									uint16_t address){
	emberAfSetCommandEndpoints(source, destination);
	(void)emberAfSendCommandUnicast(EMBER_OUTGOING_DIRECT, address);
}

void SEND_OnOffCommandInitiator(uint8_t EP,uint8_t state){
	if(state == 0){
		emberAfFillCommandOnOffClusterOff();
		emberAfSetCommandEndpoints(EP, EP);
		emberAfSendCommandUnicastToBindings();
	}else if(state == 1){
		emberAfFillCommandOnOffClusterOn();
		emberAfSetCommandEndpoints(EP, EP);
		emberAfSendCommandUnicastToBindings();
	}
}
