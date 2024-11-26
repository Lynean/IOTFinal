/*
 * si7020.c
 *
 *  Created on: 08-05-2024
 *      Author: Windows
 */

#include "si7020.h"
#include "Source/Driver/driver.h"

void TempHum_Init(){
	// Select LFRCO as source for RTCC clock branch
	CMU_ClockSelectSet(cmuClock_RTCCCLK, cmuSelect_LFRCO);

	// Setup RTCC parameters
	RTCC_Init_TypeDef rtccInit = RTCC_INIT_DEFAULT;
	rtccInit.presc = rtccCntPresc_1;

	// Initialize RTCC
	RTCC_Reset();
	RTCC_Init(&rtccInit);

	CMU_ClockEnable(cmuClock_I2C0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortB, 0, gpioModeWiredAndPullUpFilter  , 1);
	GPIO_PinModeSet(gpioPortB, 1, gpioModeWiredAndPullUpFilter  , 1);

	GPIO->I2CROUTE[0].SDAROUTE = (GPIO->I2CROUTE[0].SDAROUTE & ~_GPIO_I2C_SDAROUTE_MASK)
	                        | (gpioPortB << _GPIO_I2C_SDAROUTE_PORT_SHIFT
	                        | (1 << _GPIO_I2C_SDAROUTE_PIN_SHIFT));
	GPIO->I2CROUTE[0].SCLROUTE = (GPIO->I2CROUTE[0].SCLROUTE & ~_GPIO_I2C_SCLROUTE_MASK)
	                        | (gpioPortB << _GPIO_I2C_SCLROUTE_PORT_SHIFT
	                        | (0 << _GPIO_I2C_SCLROUTE_PIN_SHIFT));
	GPIO->I2CROUTE[0].ROUTEEN = GPIO_I2C_ROUTEEN_SDAPEN | GPIO_I2C_ROUTEEN_SCLPEN;
	I2C_Init_TypeDef init = I2C_INIT_DEFAULT;
	init.freq = 50000;

	I2C_Init(I2C0, &init );
	I2C_Enable(I2C0, true);
	I2C0->CTRL = I2C_CTRL_AUTOSN;
}

uint16_t getTemp()
{
	uint16_t  RxBuffer;
	uint8_t cmd = 0xE3;
	I2C_TransferReturn_TypeDef ret;
	I2C_TransferSeq_TypeDef transferSeq;
	transferSeq.addr = (uint16_t)0x80;
	transferSeq.flags = I2C_FLAG_WRITE_READ;
	transferSeq.buf[0].data = &cmd;
	transferSeq.buf[0].len = 1;
	transferSeq.buf[1].data   = (uint8_t*)&RxBuffer;
	transferSeq.buf[1].len    = 2;
	// Do a polled transfer
	ret = I2C_TransferInit(I2C0, &transferSeq);
	while (ret == i2cTransferInProgress)
	{
	  ret = I2C_Transfer(I2C0);
	}

	uint8_t LSB_MSB[2];
	LSB_MSB[1] = RxBuffer & 0xFF;
	LSB_MSB[0] = RxBuffer >> 8;
	return *((uint16_t*) LSB_MSB);
}

uint16_t getHum()
{
	uint16_t  RxBuffer;
	uint8_t cmd = 0xE5;
	I2C_TransferReturn_TypeDef ret;
	I2C_TransferSeq_TypeDef transferSeq;
	transferSeq.addr = (uint16_t)0x80;
	transferSeq.flags = I2C_FLAG_WRITE_READ;
	transferSeq.buf[0].data = &cmd;
	transferSeq.buf[0].len = 1;
	transferSeq.buf[1].data   = (uint8_t*)&RxBuffer;
	transferSeq.buf[1].len    = 2;
	ret = I2C_TransferInit(I2C0, &transferSeq);
	while (ret == i2cTransferInProgress)
	{
	  ret = I2C_Transfer(I2C0);
	}

	uint8_t LSB_MSB[2];
	LSB_MSB[1] = RxBuffer & 0xFF;
	LSB_MSB[0] = RxBuffer >> 8;
	return *((uint16_t*) LSB_MSB);
}

uint16_t convertHum(uint16_t code){
	float hum;
	hum = ((125*code)/65536) - 6;
	return (uint16_t)hum;
}


int16_t convertTemp(uint16_t code){
	float temp;
	temp = ((175.72*code)/65536) - 46.85;
	return (int16_t)temp;
}
