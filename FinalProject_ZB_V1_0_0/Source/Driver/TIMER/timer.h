/*
 * timer.h
 *
 *  Created on: Aug 19, 2024
 *      Author: Admin
 */

#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_timer.h"
#include "app/framework/include/af.h"
// Desired PWM frequency and initial duty cycle
#define PWM_FREQ            800000
#define INITIAL_DUTY_CYCLE  32
//60 LEDs * 3 Colors
#define NUM_OF_LED_COLOR	180
//Code dutyCycle for ws2812
//#define CODE_0_DUTYCYCLE 	((float)0.32)
//#define CODE_1_DUTYCYCLE	((float)0.52)
typedef struct{
	uint8_t 			channelCount;

	GPIO_Port_TypeDef 	CC0Port;
	uint8_t				CC0Pin;

	GPIO_Port_TypeDef 	CC1Port;
	uint8_t				CC1Pin;

	GPIO_Port_TypeDef 	CC2Port;
	uint8_t				CC2Pin;
}PWMInitStruct_t;

typedef enum {
	Single,
	Queue,
	Idle
}timerMode_t;

void timerSetDutyCycle(uint8_t CC0, uint8_t CC1, uint8_t CC2);
void initTIMER_PWM(PWMInitStruct_t initStruct);
void initTIMER_WS2812(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin, uint8_t* buffer, uint16_t bufferlen , GPIO_Mode_TypeDef gpioMode);
void initLDMA(uint8_t *buffer, uint16_t size);
void stopPWM();
void startPWM();
void timerQueueDutyCycle();
