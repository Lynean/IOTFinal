/*
 * ldr.h
 *
 *  Created on: 08-05-2024
 *      Author: Windows
 */

#ifndef SOURCE_MID_LDR_LDR_H_
#define SOURCE_MID_LDR_LDR_H_



#endif /* SOURCE_MID_LDR_LDR_H_ */

#include "em_iadc.h"
#include "efr32mg21_iadc.h"
#include "efr32mg21a010f512im32.h"
#include "app/framework/include/af.h"
#include "math.h"

#define CLK_SRC_ADC_FREQ    20000000  // CLK_SRC_ADC
#define CLK_ADC_FREQ        10000000  // CLK_ADC - 10 MHz max in normal mode

#define NUM_SAMPLES         1024

// Set CLK_ADC to 10 MHz
#define CLK_SRC_ADC_FREQ    20000000  // CLK_SRC_ADC
#define CLK_ADC_FREQ        10000000  // CLK_ADC - 10 MHz max in normal mode

double LDR_ReadADC();
void LDR_ADCInit(void);
