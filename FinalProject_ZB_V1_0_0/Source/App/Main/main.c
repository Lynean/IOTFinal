/*
 * main.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Admin
 */

#include "Source/App/app.h"
#include "Source/Mid/mid.h"

extern bool networkReady;


EmberEventControl getLightEventControl;
EmberEventControl getTempHumEventControl;
EmberEventControl mainStateEventControl;
SYSTEM_State mainState;

uint8_t r,g,b;
uint8_t month,day,hour,minute, room;

void USER_NetworkEventHandle(NWK_State state);
void USER_pressAndHoldEventHandle(uint8_t button, pressAndHoldEventEnum event);


void emberAfMainInitCallback(void)
{
	//Initialization
	ledInit();
	buttonInit(USER_pressAndHoldEventHandle);
	NETWORK_Init(USER_NetworkEventHandle);
	LDR_ADCInit();
	TempHum_Init();
	timeInit(0,0);
	mainState = POWER_ON_STATE;
	emberEventControlSetActive(mainStateEventControl);
	emberAfCorePrintln("INIT WS2812");
}

void mainStateEventHandler(){
	switch(mainState){
	case POWER_ON_STATE:
		if (emberAfNetworkState() == EMBER_NO_NETWORK){
			ledBlink(red, 3, 300, 300);
			ledBlink(red, 3, 300, 300);
			NETWORK_FindAndJoin();
		}
		mainState = IDLE_STATE;
		break;
	case REPORT_STATE:
		SEND_ReportInfoToHc();
		mainState = IDLE_STATE;
		break;
	case IDLE_STATE:
		break;
	case REBOOT_STATE:
		mainState = IDLE_STATE;
		halReboot();
		break;
	}
}

boolean emberAfPreCommandReceivedCallback(EmberAfClusterCommand* cmd){
	//emberAfCorePrintln("Received");
	uint16_t clusterSpecific = cmd->clusterSpecific;
	uint16_t clusterID = cmd->apsFrame->clusterId;
	uint16_t commandID = cmd->commandId;
	uint8_t DEP = cmd->apsFrame->destinationEndpoint;

	uint8_t startIndex = cmd->payloadStartIndex;
	uint8_t *data = cmd->buffer;


	uint8_t SourceADDR = cmd->source;
	switch(clusterID){
	case ZCL_ON_OFF_CLUSTER_ID:
		RECEIVE_OnOffCommandHandle(clusterSpecific ,commandID, DEP, data, startIndex);
		break;
	case ZCL_COLOR_CONTROL_CLUSTER_ID:
		RECEIVE_ColorControlCommandHandle(clusterSpecific ,commandID, data, startIndex);
		break;
	case ZCL_TIME_CLUSTER_ID:
		RECEIVE_TimeCommandHandle(clusterSpecific ,commandID, data, startIndex);
		break;
	default:
		break;
	}
	return false;
}

/*Handler for each time emberStackStatusCallback is called*/
void USER_NetworkEventHandle(NWK_State state){
	switch(state){
	case NETWORK_JOIN_SUCCESS:
		WS2812_Init(gpioPortC, 0);
		ledBlink(pink, 3, 300, 300);
		mainState = REPORT_STATE;
		emberEventControlSetActive(getLightEventControl);
		emberEventControlSetActive(getTempHumEventControl);
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		SEND_OnOffState(1, 0);
		SEND_ColorState(1, 0x0034, 0);
		SEND_ColorState(1, 0x0038, 0);
		SEND_ColorState(1, 0x003C, 0);
		break;
	case NETWORK_HAS_PARENT:
		WS2812_Init(gpioPortC, 0);
		emberAfCorePrintln("ALREADY JOIN");
		ledBlink(pink, 3, 300, 300);
		networkReady = true;
		mainState = REPORT_STATE;
		emberEventControlSetActive(getLightEventControl);
		emberEventControlSetActive(getTempHumEventControl);
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		SEND_OnOffState(1, 0);
		SEND_ColorState(1, 0x0034, 0);
		SEND_ColorState(1, 0x0038, 0);
		SEND_ColorState(1, 0x003C, 0);
		break;
	case NETWORK_LOST_PARENT:
		emberAfCorePrintln("OUT");
		mainState = IDLE_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		break;
	case NETWORK_OUT_NETWORK:
		if(!networkReady){
			mainState = REBOOT_STATE;
			emberEventControlSetDelayMS(mainStateEventControl, 3000);
		}
		break;
	default:
		break;
	}
}

void USER_pressAndHoldEventHandle(uint8_t button, pressAndHoldEventEnum event){
	if(button == SW1){
		switch(event){
		case press_1:
			break;
		case press_2:
			break;
		case press_3:
			ledBlink( pink, 3, 300, 300);
			emberAfPluginFindAndBindTargetStart(1);
			break;
		case press_4:
			break;
		case press_5:
			break;
		default:
			break;
		}
	}else if(button == SW2){
		switch(event){
		case press_1:
			break;
		case press_2:
			break;
		case press_3:
			ledBlink(pink, 3, 300, 300);
			emberAfPluginFindAndBindTargetStart(1);
			break;
		case press_4:
			break;
		case press_5:
			//Leave Network
			SEND_LeaveInfoToHc();
			NETWORK_Leave();
			break;
		default:
			break;
		}
	}
}

void getTempHumEventHandler(){
	emberEventControlSetInactive(getTempHumEventControl);
	static int16_t lastTemp = 0;
	static int16_t lastHum = 0;
	uint16_t tempCode;
	int16_t tempVal;
	uint16_t humCode;
	uint16_t humVal;

	tempCode = getTemp();
	humCode = getHum();
	tempVal = convertTemp(tempCode);
	humVal = convertHum(humCode);

	if(abs(((int16_t)tempVal - (int16_t)lastTemp)) > 1){
		lastTemp = (int16_t)tempVal;
		SEND_TempMeasurement(3, (int16_t)tempVal);

	}
	if(abs(((uint16_t)humVal - (uint16_t)lastHum)) > 1){
		lastHum = (uint16_t)humVal;
		SEND_HumMeasurement(4, (uint16_t)humVal);
	}

	setHumAttribute((uint16_t)humVal);
	setTempAttribute((int16_t)tempVal);
	emberAfCorePrintln("TEMPERATURE : %d", (uint16_t)humVal);
	emberAfCorePrintln("HUMIDITY : %d", (int16_t)tempVal);
	emberEventControlSetDelayMS(getTempHumEventControl,300000);
}
void getLightEventHandler(){

	emberEventControlSetInactive(getLightEventControl);
	static uint16_t lastResult = 0;
	double result;
	result = LDR_ReadADC();
	emberAfCorePrintln("LightMeasurement: %d", (int)result);

	//If difference > 30 lux
	if(abs(((uint16_t)result - (uint16_t)lastResult)) > 3){
		lastResult = (uint16_t)result;
		SEND_LDRMeasurement(2, (uint16_t)result);
	}
	setLightAttribute((uint16_t)result);
	emberEventControlSetDelayMS(getLightEventControl,30000);
}
