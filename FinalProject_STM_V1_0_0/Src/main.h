/*
 * main.h
 *
 *  Created on: Nov 25, 2024
 *      Author: Admin
 */

#ifndef MAIN_H_
#define MAIN_H_



#endif /* MAIN_H_ */
#include <stdint.h>
#include "stm32f401re_gpio.h"
#include "stm32f401re_usart.h"
#include "stm32f401re_exti.h"
#include "stm32f401re_syscfg.h"
#include "misc.h"
#include "Ucglib.h"
#include "stm32f401re_rcc.h"
#include "timer.h"
#include "button.h"
#include <string.h>
#include <stdio.h>
#include "stm32f401re_tim.h"
#include "flashmem.h"

#define TIM_PERIOD			(uint32_t)8399
#define Tim_Update			(TIM2->CNT)
#define TimLimit_SendData	1000

#define USARTx_CLK                      RCC_APB2Periph_USART6
#define USARTx_CLK_INIT                 RCC_APB2PeriphClockCmd
#define USARTx_IRQn                     USART6_IRQn
#define USARTx_IRQHandler               USART6_IRQHandler

#define USARTx_GPIO_PORT             	GPIOC
#define USARTx_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define USARTx_AF                    	GPIO_AF_USART6

#define USARTx_TX_PIN                   GPIO_Pin_6
#define USARTx_TX_SOURCE                GPIO_PinSource6

#define USARTx_RX_PIN                   GPIO_Pin_3
#define USARTx_RX_SOURCE                GPIO_PinSource3

//PB5
#define BUTTON1_PORT					GPIOB
#define BUTTON1_PIN						GPIO_Pin_5

//PB3
#define BUTTON2_PORT					GPIOB
#define BUTTON2_PIN						GPIO_Pin_3

//PA4
#define BUTTON3_PORT					GPIOA
#define BUTTON3_PIN						GPIO_Pin_4

//PB0
#define BUTTON4_PORT					GPIOB
#define BUTTON4_PIN						GPIO_Pin_0

//PB4
#define BUTTON5_PORT					GPIOB
#define BUTTON5_PIN						GPIO_Pin_4


#define ALL_BUTTON_NOT_RELEASED			(	!Button_GetLogicInputPin(BUTTON_KIT_ID1) ||\
											!Button_GetLogicInputPin(BUTTON_KIT_ID2) ||\
											!Button_GetLogicInputPin(BUTTON_KIT_ID4) ||\
											!Button_GetLogicInputPin(BUTTON_KIT_ID5))

#define ALL_BUTTON_RELEASED				(	Button_GetLogicInputPin(BUTTON_KIT_ID1) &&\
											Button_GetLogicInputPin(BUTTON_KIT_ID2) &&\
											Button_GetLogicInputPin(BUTTON_KIT_ID4) &&\
											Button_GetLogicInputPin(BUTTON_KIT_ID5))





/*	Startup: Title
 	Init: Time initialization
 	Main: Mode -> RGB or None
 		  Setting -> Room or Time initialization
*/
typedef enum{
	STARTUP,
	INIT,
	MAIN,
	CONFIG
}appState_t;

typedef enum{
	Screen0_None,
	Screen1_Manual,
	Screen1_Auto,
	Screen1_Storm,
	Screen1_Fire,
	Screen1_Aurora,
	Screen1_Party,
	Screen1_Warning,
	Screen1_Setting,
	Screen1_Off,
	Screen2_Red,
	Screen2_Green,
	Screen2_Blue,
	Screen3_Room,
	Screen3_Time,
	Screen4_Living,
	Screen4_Sleep,
	Screen4_Study,
	Screen4_Dining,
	Screen4_Excercise,
	Screen5_Hour,
	Screen5_Minute,
	Screen6_End
}Choice_t;


typedef enum {
	Minute,
	Hour,
	Room,
	Red,
	Green,
	Blue,
	ActiveMode,
	Off
}MessageType_t;

enum {
	Manual = 0,
	Rainbow = 1,
	Storm = 2,
	Fire = 3,
	Aurora = 4,
	Party = 5,
	Warning = 6
};

enum{
	Sleep,
	Study,
	Dining,
	Exercise,
	Living
};

typedef struct{
	uint8_t startByte;
	MessageType_t msgID;
	uint8_t Data;
	uint8_t stopByte;
}messageFrame_t;

typedef enum{
	ButtonReleased,
	ButtonPressed,
	ButtonHolding,
	ButtonSubmitted
}ButtonState_t;


/*
	Draw screen after ClearScreen when button B3
*/
void USER_ButtonSubmitEventHandler();
void USER_ButtonReleaseHandler(uint8_t button);

/*
	Process direction of buttons
*/
void ProcessDirection();
/*
 Initialization
*/
void LoadConfiguration();
void AppInitCommon();
void ButtonInit();
void USART2_Init();
void TimeBasic_Init();
/*
 Config USART Buffer to be sent in USART2_SendMessage()
*/
void ConfigUSARTBuffer(MessageType_t msgID, uint8_t data);
void USART2_SendMessage();
/*
 Main LCD control
*/
void MAIN_RefreshChoices();
void appStateManager();
/*
 Draw string on screen
*/
void screenRefresh();

void UARTBufferPop(messageFrame_t* inBufBase);

void USART2_ResetBuffer(void);

