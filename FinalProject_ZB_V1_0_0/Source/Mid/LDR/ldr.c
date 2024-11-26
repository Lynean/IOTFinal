/*
 * ldr.c
 *
 *  Created on: 08-05-2024
 *      Author: Windows
 */

#include "ldr.h"
#include "Source/Driver/driver.h"

void LDR_ADCInit(){
	CMU_ClockEnable(cmuClock_IADC0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortB, 0, gpioModeWiredAndPullUpFilter  , 1);

	CMU_ClockSelectSet(cmuClock_IADCCLK, cmuSelect_FSRCO);

	IADC_Init_t init = IADC_INIT_DEFAULT;
	IADC_AllConfigs_t initAllConfigs = IADC_ALLCONFIGS_DEFAULT;
	IADC_InitSingle_t initSingle = IADC_INITSINGLE_DEFAULT;
	IADC_SingleInput_t singleInput = IADC_SINGLEINPUT_DEFAULT;

	singleInput.posInput = iadcPosInputPortCPin5;

	// Shutdown between conversions to reduce current
	init.warmup = iadcWarmupNormal;
	// Set the HFSCLK prescale value here
	init.srcClkPrescale = IADC_calcSrcClkPrescale(IADC0, CLK_SRC_ADC_FREQ, 0);

	initAllConfigs.configs[0].reference = iadcCfgReferenceInt1V2;
	initAllConfigs.configs[0].vRef = 1210;
	initAllConfigs.configs[0].analogGain = iadcCfgAnalogGain0P5x;

	// Set conversions to run continuously
	initSingle.triggerSelect = _IADC_TRIGGER_SCANTRIGSEL_IMMEDIATE;
	initSingle.triggerAction = iadcTriggerActionOnce;

	// Set alignment to right justified with 12 bits for data field
	initSingle.alignment = iadcAlignRight12;
	initSingle.start = true;
	initAllConfigs.configs[0].adcClkPrescale = IADC_calcAdcClkPrescale(IADC0,CLK_ADC_FREQ,0,iadcCfgModeNormal,init.srcClkPrescale);
	GPIO->CDBUSALLOC |= GPIO_CDBUSALLOC_CDODD0_ADC0;
	// Initialize IADC
	IADC_reset(IADC0);
	IADC_initSingle(IADC0, &initSingle, &singleInput);
	IADC_init(IADC0, &init, &initAllConfigs);
	IADC_command(IADC0, iadcCmdStartSingle);


}

double LDR_ReadADC(){
	IADC_command(IADC0, iadcCmdStartSingle);
	double result;
	while((IADC0->STATUS &( _IADC_STATUS_CONVERTING_MASK |_IADC_STATUS_SINGLEFIFODV_MASK)) != IADC_STATUS_SINGLEFIFODV);

	result = IADC_readSingleResult(IADC0).data;

	emberAfCorePrintln("Illum Code: %d", (uint16_t)result);

	double Vout = result * (1.2 / (4096 - 1));// Conversion analog to voltage
	double RLDR = ((1.2 - Vout)*10000)/ Vout; // Conversion voltage to resistance
	emberAfCorePrintln("Resistance: %d", (uint16_t)RLDR);
	result = (pow(10,5.63))/(pow(RLDR, 0.86));
	return result;
}


