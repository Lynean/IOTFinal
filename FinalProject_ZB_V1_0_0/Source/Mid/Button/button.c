/*
 * button.c
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */


#include "button.h"
#include "Source/Driver/driver.h"

ButtonEventHandle buttonEventHandle;
uint8_t pressCount = 0;
uint8_t lastButton;

EmberEventControl buttonTimeoutEventControl;

void buttonInit(ButtonEventHandle USER_buttonEventHandle){
	if (USER_buttonEventHandle != NULL){
		buttonEventHandle = USER_buttonEventHandle;
	}
	pressCount = 0;
}

void buttonTimeoutEventHandler(){
	emberEventControlSetInactive(buttonTimeoutEventControl);
	emberAfCorePrintln("Press Count: %d", pressCount);

	switch(pressCount){
	case 1:
		pressCount = 0;
		(*buttonEventHandle)(lastButton, press_1);
		break;
	case 2:
		pressCount = 0;
		(*buttonEventHandle)(lastButton, press_2);
		break;
	case 3:
		pressCount = 0;
		(*buttonEventHandle)(lastButton, press_3);
		break;
	case 4:
		pressCount = 0;
		(*buttonEventHandle)(lastButton, press_4);
		break;
	case 5:
		pressCount = 0;
		(*buttonEventHandle)(lastButton, press_5);
		break;
	default:
		pressCount = 0;
		break;
	}

}

void emberAfHalButtonIsrCallback(int8u button,int8u state){
	static uint32_t lastEventTick = 0;
	uint32_t current;
	current = halCommonGetInt32uMillisecondTick();
	if(state == 1){
		if(current - lastEventTick < 40){
			lastEventTick = current;
			return;
		}else{
			lastEventTick = current;
		}
		emberAfCorePrintln("Pressing");
		emberEventControlSetInactive(buttonTimeoutEventControl);
		if(button == lastButton){
			pressCount++;
		}else{

			lastButton = button;
			pressCount = 1;
		}
	}else{
		emberAfCorePrintln("Released, Count: %d", pressCount);
		emberEventControlSetDelayMS(buttonTimeoutEventControl, 200);
	}
}














