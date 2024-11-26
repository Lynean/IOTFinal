/*
 * button.h
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */

#ifndef SOURCE_MID_BUTTON_BUTTON_H_
#define SOURCE_MID_BUTTON_BUTTON_H_



#endif /* SOURCE_MID_BUTTON_BUTTON_H_ */

#include "app/framework/include/af.h"

#define BUTTON_PORT gpioPortD
#define SW1 BUTTON0
#define SW2 BUTTON1


typedef enum{
	released,
	press_1,
	press_2,
	press_3,
	press_4,
	press_5
}pressAndHoldEventEnum;

typedef void (*ButtonEventHandle)(uint8_t, pressAndHoldEventEnum);

void buttonInit(ButtonEventHandle USER_buttonEventHandle);
void emberAfHalButtonIsrCallback(int8u button,int8u state);
