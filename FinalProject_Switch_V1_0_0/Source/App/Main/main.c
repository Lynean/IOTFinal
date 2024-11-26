/*
 * main.c
 *
 *  Created on: Jun 9, 2024
 *      Author: Admin
 */

#include "main.h"
#include "Source/Mid/mid.h"
#include "Source/App/app.h"
extern bool networkReady;

EmberEventControl getLightEventControl;
EmberEventControl mainStateEventControl;

SYSTEM_State mainState;

void USER_NetworkEventHandle(NWK_State state);
void USER_pressAndHoldEventHandle(uint8_t button, pressAndHoldEventEnum event);

void emberAfMainInitCallback(void)
{
	//Initialization
	ledInit();
	buttonInit(USER_pressAndHoldEventHandle);
	NETWORK_Init(USER_NetworkEventHandle);
	STMCom_Init((STMComEventHandle)SEND_ColorCommandInitiator);
	mainState = POWER_ON_STATE;
	emberEventControlSetActive(mainStateEventControl);
}


void mainStateEventHandler(){
	switch(mainState){
	case POWER_ON_STATE:
		if (emberAfNetworkState() == EMBER_NO_NETWORK){
			ledBlink(LED1, red, 3, 300, 300);
			ledBlink(LED1, red, 3, 300, 300);
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
	uint16_t clusterID = cmd->apsFrame->clusterId;
	uint16_t commandID = cmd->commandId;
	uint8_t DEP = cmd->apsFrame->destinationEndpoint;
	uint8_t SourceADDR = cmd->source;
	switch(clusterID){
	case ZCL_ON_OFF_CLUSTER_ID:
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
		ledBlink(LED1, pink, 3, 300, 300);
		mainState = REPORT_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
		break;
	case NETWORK_HAS_PARENT:
		emberAfCorePrintln("ALREADY JOIN");
		ledBlink(LED1, pink, 3, 300, 300);
		networkReady = true;
		mainState = REPORT_STATE;
		emberEventControlSetDelayMS(mainStateEventControl, 1000);
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
			ledColorSet(LED0, blue);
			SEND_OnOffState(1, 1);
			break;
		case press_2:
			ledClear(LED0);
			SEND_OnOffState(1, 0);
			break;
		case press_3:
			ledBlink(LED0, pink, 3, 300, 300);
			emberAfPluginFindAndBindInitiatorStart(1);
			break;
		case press_4:
			ledBlink(LED0, pink, 4, 100, 100);
			break;
		case press_5:
			SEND_LeaveInfoToHc();
			NETWORK_Leave();
			break;
		default:
			break;
		}
	}else if(button == SW2){
		switch(event){
		case press_1:
			ledColorSet(LED1, blue);
			SEND_OnOffState(2, 1);
			break;
		case press_2:
			ledClear(LED1);
			SEND_OnOffState(2, 0);
			break;
		case press_3:
			ledBlink(LED1, pink, 3, 300, 300);
			emberAfPluginFindAndBindInitiatorStart(1);
			break;
		case press_4:
			ledBlink(LED1, pink, 4, 100, 100);
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
