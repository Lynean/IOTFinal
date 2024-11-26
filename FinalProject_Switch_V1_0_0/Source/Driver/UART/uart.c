/***************************************************************************//**
 * @file main_s2_xg21.c
 *
 * @brief This project demonstrates interrupt-driven use of the USART in
 * synchronous (SPI) secondary mode.  The main loop transmits the specified number
 * of bytes and receives the byte that is shifted in by the main with each
 * outgoing one.
 *
 * The pins used in this example are defined below and are described in the
 * accompanying readme.txt file.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *******************************************************************************
 * # Evaluation Quality
 * This code has been minimally tested to ensure that it builds and is suitable
 * as a demonstration for evaluation purposes only. This code will be maintained
 * at the sole discretion of Silicon Labs.
 ******************************************************************************/

#include "uart.h"

// Incoming data
uint8_t inbuf[BUFLEN];

// Position in the buffer
uint32_t bufpos;

USARTCallback_t USER_USARTCallback;
/**************************************************************************//**
 * @brief
 *    GPIO initialization
 *****************************************************************************/
void USART_initGPIO(void)
{
  // Configure MOSI pin as an input
  GPIO_PinModeSet(US2MOSI_PORT, US2MOSI_PIN, gpioModeInput, 0);

  // Configure MISO pin as an output
  GPIO_PinModeSet(US2MISO_PORT, US2MISO_PIN, gpioModePushPull, 0);

  // Configure CLK pin as an input
  GPIO_PinModeSet(US2CLK_PORT, US2CLK_PIN, gpioModeInput, 0);

  // Configure CS pin as an input pulled high
  GPIO_PinModeSet(US2CS_PORT, US2CS_PIN, gpioModeInput, 1);

}

/**************************************************************************//**
 * @brief
 *    USART0 initialization
 *****************************************************************************/
void initUSART2(USARTCallback_t callback)
{
	USER_USARTCallback =  callback;
	// Default asynchronous initializer (main mode, 9600, 8-bit data)
	USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
	init.baudrate = 9600;
	init.enable = usartDisable;  // Do not enable yet
	// Configure but do not enable USART2
	USART_InitAsync(USART2, &init);

	// Route USART0 RX, TX, CLK, and CS to the specified pins.
	GPIO->USARTROUTE[2].RXROUTE = (US2MOSI_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
	  | (US2MOSI_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);
	GPIO->USARTROUTE[2].TXROUTE = (US2MISO_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
	  | (US2MISO_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

	// Enable USART interface pins
	GPIO->USARTROUTE[2].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN  |    // MOSI
								GPIO_USART_ROUTEEN_TXPEN ;    // MISO
	// Enable NVIC USART sources
	NVIC_ClearPendingIRQ(USART2_RX_IRQn);
	NVIC_EnableIRQ(USART2_RX_IRQn);
	USART_IntEnable(USART2, USART_IEN_RXDATAV);
	USART_Enable(USART2, USART_CMD_RXEN);
}
/**************************************************************************//**
 * @brief
 *    USART0 receive interrupt handler
 *****************************************************************************/
void USART2_RX_IRQHandler(void){
	/*
	* Save the byte received concurrent with the transmission of the
	* last bit of the previous outgoing byte, and increment the buffer
	* position to the next byte.  Note that this read clears the
	* USART_IF_RXDATAV interrupt flag.
	*/
	inbuf[bufpos++] = USART2->RXDATA;
	//If corresponding start / stop byte
	//GPIO_IntClear(1 << US2CS_PIN);
	USER_USARTCallback(inbuf);
}

void USART2_ResetBuffer(void){
	bufpos = 0;
	memset(inbuf,0,BUFLEN);
}

