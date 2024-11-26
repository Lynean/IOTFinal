/*
 * network.c
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */
#include "Source/App/app.h"
#include "Source/Mid/mid.h"

uint8_t timeFindAndJoin;
bool  networkReady;
NETWORK_EventHandle NWK_EventHandle;

EmberEventControl joinNetworkEventControl;

void NETWORK_Init(NETWORK_EventHandle HandleFunc){
	timeFindAndJoin = 0;
	if (HandleFunc != NULL){
		NWK_EventHandle = HandleFunc;
	}
}

void NETWORK_FindAndJoin(void){
	if(emberAfNetworkState() == EMBER_NO_NETWORK){
		emberAfPluginNetworkSteeringStart();
		emberEventControlSetDelayMS(joinNetworkEventControl, 2000);
		emberAfCorePrintln("Steering the network");
	}
}

void NETWORK_StopFindAndJoin(void){
	emberEventControlSetInactive(joinNetworkEventControl);
}

void joinNetworkEventHandler(){
	emberEventControlSetInactive(joinNetworkEventControl);
	if(emberAfNetworkState() == EMBER_NO_NETWORK){
		NETWORK_FindAndJoin();
		timeFindAndJoin++;
		emberEventControlSetDelayMS(joinNetworkEventControl,10000);
	}
}

bool emberAfStackStatusCallback(EmberStatus status){
	emberAfCorePrintln("emberAfStackStatusCallback\n");
	if(status== EMBER_NETWORK_UP){
		networkReady = true;
		if(timeFindAndJoin>0){
			NETWORK_StopFindAndJoin();
			if(NWK_EventHandle != NULL){
				(*NWK_EventHandle)(NETWORK_JOIN_SUCCESS);
			}
		}else{
			if(NWK_EventHandle != NULL){
				(*NWK_EventHandle)(NETWORK_HAS_PARENT);
			}
		}
	}else{
		EmberNetworkStatus nwkStatusCurrent = emberAfNetworkState();
		if(nwkStatusCurrent == EMBER_NO_NETWORK){
			networkReady = false;
			if(NWK_EventHandle != NULL){
				(*NWK_EventHandle)(NETWORK_OUT_NETWORK);
			}
		}
		else if(nwkStatusCurrent == EMBER_JOINED_NETWORK_NO_PARENT){
			(*NWK_EventHandle)(NETWORK_LOST_PARENT);
		}
	};
	return false;
}

void NETWORK_Leave(){
	emberClearBindingTable();
	emberLeaveNetwork();
}
