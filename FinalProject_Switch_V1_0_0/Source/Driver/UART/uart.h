/*
 * uart.h
 *
 *  Created on: Aug 17, 2024
 *      Author: Admin
 */

#ifndef SOURCE_MID_UART_UART_H_
#define SOURCE_MID_UART_UART_H_



#endif /* SOURCE_MID_UART_UART_H_ */

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_usart.h"
// SPI ports and pins
#define US2MISO_PORT  gpioPortC
#define US2MISO_PIN   1
#define US2MOSI_PORT  gpioPortC
#define US2MOSI_PIN   0
#define US2CLK_PORT   gpioPortC
#define US2CLK_PIN    2
#define US2CS_PORT    gpioPortC
#define US2CS_PIN     3

// Size of the data buffers
#define BUFLEN  10


typedef void (*USARTCallback_t)(uint8_t*);
void USART_initGPIO(void);
void initUSART2();
void USART2_ResetBuffer(void);
