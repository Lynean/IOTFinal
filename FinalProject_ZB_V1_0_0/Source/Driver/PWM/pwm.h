/*
 * pwm.h
 *
 *  Created on: Oct 5, 2024
 *      Author: Admin
 */

#ifndef SOURCE_DRIVER_PWM_PWM_H_
#define SOURCE_DRIVER_PWM_PWM_H_



#endif /* SOURCE_DRIVER_PWM_PWM_H_ */

#include "app/framework/include/af.h"
void initTIMERPWM(GPIO_Port_TypeDef port, uint8_t pin);
void initLDMA(uint8_t* buffer, uint16_t size);
void StartPWM();
void StopPWM();
