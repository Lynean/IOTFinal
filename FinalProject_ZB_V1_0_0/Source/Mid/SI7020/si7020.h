/*
 * si7020.h
 *
 *  Created on: 08-05-2024
 *      Author: Windows
 */

#ifndef SOURCE_MID_SI7020_SI7020_H_
#define SOURCE_MID_SI7020_SI7020_H_



#endif /* SOURCE_MID_SI7020_SI7020_H_ */

#include "em_i2c.h"
#include "app/framework/include/af.h"
#include "efr32mg21a010f512im32.h"
#include "em_rtcc.h"

void TempHum_Init();
uint16_t getTemp();
uint16_t getHum();

uint16_t convertHum(uint16_t code);
int16_t convertTemp(uint16_t code);
