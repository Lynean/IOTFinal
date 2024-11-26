/*
 * led.c
 *
 *  Created on: Jun 3, 2024
 *      Author: Admin
 */


#include "led.h"

led_t ledRGBArray[LED_RGB_COUNT][LED_RGB_ELEMENT] = {LED_BOARD_0, LED_BOARD_1};
ledBlink_t BlinkState[LED_RGB_COUNT];

EmberEventControl led0ToggleEventControl;
EmberEventControl led1ToggleEventControl;

uint32_t ledToggleEventHandle(uint8_t led_board);

void ledInit(){
	emberAfCorePrintln("LedInit");
	CMU_ClockEnable(cmuClock_GPIO, true);
	for(int i=0; i<LED_RGB_COUNT;i++){
		for(int j=0; j< LED_RGB_ELEMENT; j++){
			GPIO_PinModeSet(ledRGBArray[i][j].port,
							ledRGBArray[i][j].pin,
							gpioModePushPull, 0);
		}
	}
	ledClear(LED0);
	ledClear(LED1);
}

void ledClear(uint8_t led_board){
	 for(int i = 0; i < LED_RGB_ELEMENT; i++){
		 GPIO_PinOutSet(ledRGBArray[led_board][i].port, ledRGBArray[led_board][i].pin);
	 }
	 if (led_board == LED0){
		 emberEventControlSetInactive(led0ToggleEventControl);
	 }else if(led_board == LED1){
		 emberEventControlSetInactive(led1ToggleEventControl);
	 }

}

void ledColorSet(uint8_t led_board, color_t color){
	for(int i=0; i< LED_RGB_ELEMENT; i++){
		if(((color>>i) & 0x01) == 1){
			GPIO_PinOutClear(ledRGBArray[led_board][i].port,ledRGBArray[led_board][i].pin);
		}else{
			GPIO_PinOutSet(ledRGBArray[led_board][i].port,ledRGBArray[led_board][i].pin);
		}
	}
}

void ledColorClear(uint8_t led_board, color_t color){
	for(int i=0; i< LED_RGB_ELEMENT; i++){
		if(((color>>i) & 0x01) == 1){
			GPIO_PinOutSet(ledRGBArray[led_board][i].port,ledRGBArray[led_board][i].pin);
		}
	}
}


void ledBlink(uint8_t led_board, color_t color, uint8_t blinkNum, uint32_t onTime, uint32_t offTime){
	BlinkState[led_board].color = color;
	BlinkState[led_board].blinkNum = blinkNum*2 - 1;
	BlinkState[led_board].onTime = onTime;
	BlinkState[led_board].offTime = offTime;
	ledClear(led_board);
	if(led_board == 0)
		emberEventControlSetActive(led0ToggleEventControl);
	else if(led_board == 1)
		emberEventControlSetActive(led1ToggleEventControl);
}

uint32_t ledToggleEventHandle(uint8_t led_board){
	uint32_t nextDelayMS;
	if(BlinkState[led_board].blinkNum % 2 != 0){
		emberAfCorePrintln("ON");
		ledColorSet(led_board, BlinkState[led_board].color);
		BlinkState[led_board].blinkNum--;
		nextDelayMS =  BlinkState[led_board].offTime;
	}else{
		emberAfCorePrintln("OFF");
		ledClear(led_board);
		if(BlinkState[led_board].blinkNum == 0){
			return 0;
		}
		BlinkState[led_board].blinkNum--;
		nextDelayMS =  BlinkState[led_board].onTime;
	}
	return nextDelayMS;
}

void led0ToggleEventHandler(void){
	emberEventControlSetInactive(led0ToggleEventControl);
	uint32_t delay;
	delay = ledToggleEventHandle(LED0);
	if(delay != 0){
		emberEventControlSetDelayMS(led0ToggleEventControl, delay);
	}else{
		emberEventControlSetInactive(led0ToggleEventControl);
	}
}

void led1ToggleEventHandler(void){
	emberEventControlSetInactive(led1ToggleEventControl);
	uint32_t delay;
	delay = ledToggleEventHandle(LED1);
	if(delay != 0){
		emberEventControlSetDelayMS(led1ToggleEventControl, delay);
	}else{
		emberEventControlSetInactive(led1ToggleEventControl);
	}
}

