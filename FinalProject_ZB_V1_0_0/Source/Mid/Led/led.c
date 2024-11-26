/*
 * led.c
 *
 *  Created on: Jun 3, 2024
 *      Author: Admin
 */


#include "led.h"
#include "Source/Driver/driver.h"

led_t ledRGBArray[LED_RGB_ELEMENT] = LED_BOARD_0;
ledBlink_t BlinkState;

EmberEventControl ledToggleEventControl;

uint32_t ledToggleEventHandle();

void ledInit(){
	emberAfCorePrintln("LedInit");
	CMU_ClockEnable(cmuClock_GPIO, true);
	for(int i=0; i<LED_RGB_ELEMENT;i++){
		GPIO_PinModeSet(ledRGBArray[i].port,
						ledRGBArray[i].pin,
						gpioModePushPull, 0);
	}
	ledClear();
}

void ledClear(){
	 for(int i = 0; i < LED_RGB_ELEMENT; i++){
		 GPIO_PinOutSet(ledRGBArray[i].port, ledRGBArray[i].pin);
	 }
		 emberEventControlSetInactive(ledToggleEventControl);
}

void ledColorSet(color_t color){
	for(int i=0; i< LED_RGB_ELEMENT; i++){
		if(((color>>i) & 0x01) == 1){
			GPIO_PinOutClear(ledRGBArray[i].port,ledRGBArray[i].pin);
		}else{
			GPIO_PinOutSet(ledRGBArray[i].port,ledRGBArray[i].pin);
		}
	}
}

void ledColorClear(color_t color){
	for(int i=0; i< LED_RGB_ELEMENT; i++){
		if(((color>>i) & 0x01) == 1){
			GPIO_PinOutSet(ledRGBArray[i].port,ledRGBArray[i].pin);
		}
	}
}


void ledBlink(color_t color, uint8_t blinkNum, uint32_t onTime, uint32_t offTime){
	BlinkState.color = color;
	BlinkState.blinkNum = blinkNum*2 - 1;
	BlinkState.onTime = onTime;
	BlinkState.offTime = offTime;
	ledClear();
	emberEventControlSetActive(ledToggleEventControl);
}

uint32_t ledToggleEventHandle(){
	uint32_t nextDelayMS;
	if(BlinkState.blinkNum % 2 != 0){
		emberAfCorePrintln("ON");
		ledColorSet(BlinkState.color);
		BlinkState.blinkNum--;
		nextDelayMS =  BlinkState.offTime;
	}else{
		emberAfCorePrintln("OFF");
		ledClear();
		if(BlinkState.blinkNum == 0){
			return 0;
		}
		BlinkState.blinkNum--;
		nextDelayMS =  BlinkState.onTime;
	}
	return nextDelayMS;
}

void ledToggleEventHandler(void){
	emberEventControlSetInactive(ledToggleEventControl);
	uint32_t delay;
	delay = ledToggleEventHandle();
	if(delay != 0){
		emberEventControlSetDelayMS(ledToggleEventControl, delay);
	}else{
		emberEventControlSetInactive(ledToggleEventControl);
	}
}
