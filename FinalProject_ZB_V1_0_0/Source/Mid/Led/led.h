/*
 * led.h
 *
 *  Created on: Jun 3, 2024
 *      Author: Admin
 */

#include "app/framework/include/af.h"
#include "em_timer.h"


#define LED_RGB_ELEMENT 3

#define LED_BOARD_0 {{4, gpioPortA},{3, gpioPortA},{0, gpioPortA}}
#define LED_BOARD_1 {{0, gpioPortD},{1, gpioPortD},{2, gpioPortD}}

#define LED0 0
#define LED1 1

typedef enum{
	off = 0x000,
	red = BIT(0),
	green = BIT(1),
	blue = BIT(2),
	pink = BIT(0) | BIT(2),
	yellow = BIT (1) | BIT(2),
	white = BIT(0) | BIT(1) | BIT(2)
}color_t;

typedef struct{
	uint8_t pin;
	GPIO_Port_TypeDef port;
}led_t;

typedef struct {
	color_t color;
	uint8_t blinkNum;
	uint32_t onTime;
	uint32_t offTime;
}ledBlink_t;

void ledInit();
void ledColorSet(color_t color);
void ledColorClear(color_t color);
void ledClear();
void ledBlink(color_t color, uint8_t blinkNum, uint32_t onTime, uint32_t offTime);

