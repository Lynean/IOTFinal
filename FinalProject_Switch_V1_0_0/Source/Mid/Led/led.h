/*
 * led.h
 *
 *  Created on: Jun 3, 2024
 *      Author: Admin
 */

#include "app/framework/include/af.h"

#define LED_RGB_COUNT 2
#define LED_RGB_ELEMENT 3

#define LED_BOARD_0 {{BSP_LED0_PIN, BSP_LED0_PORT},{BSP_LED1_PIN, BSP_LED1_PORT}, {BSP_LED2_PIN, BSP_LED2_PORT}}
#define LED_BOARD_1 {{BSP_LED3_PIN, BSP_LED3_PORT},{BSP_LED4_PIN, BSP_LED4_PORT}, {BSP_LED5_PIN, BSP_LED5_PORT}}
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
void ledColorSet(uint8_t led_board, color_t color);
void ledColorClear(uint8_t led_board, color_t color);
void ledClear(uint8_t led_board);
void ledBlink(uint8_t led_board, color_t color, uint8_t blinkNum, uint32_t onTime, uint32_t offTime);

