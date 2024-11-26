/*
 * ws2812.h
 *
 *  Created on: Sep 14, 2024
 *      Author: Admin
 */


/*
 * ledPWM.h
 *
 *  Created on: Aug 19, 2024
 *      Author: Admin
 */

#include "app/framework/include/af.h"

#define LED_NUM		60
#define COLOR_NUM 	3
//180*8
#define QUEUE_LEN	1440
//G-R-B
#define CODE_0_DUTYCYCLE 	(16)
#define CODE_1_DUTYCYCLE	(25)

typedef struct{
	uint8_t BIT0 :1;
	uint8_t BIT1 :1;
	uint8_t BIT2 :1;
	uint8_t BIT3 :1;
	uint8_t BIT4 :1;
	uint8_t BIT5 :1;
	uint8_t BIT6 :1;
	uint8_t BIT7 :1;
}BITstruct_t;

typedef union {
	uint8_t byte;
	BITstruct_t byteAdvance;
}AdvanceByte_t;
typedef struct{
	AdvanceByte_t red;
	AdvanceByte_t green;
	AdvanceByte_t blue;
}WSColor_t;

void WS2812SetColor_Index(uint8_t Num, WSColor_t colorStruct);
void WS2812_Init(GPIO_Port_TypeDef gpioPort, uint8_t gpioPortPin);
void WS2812SetColor_All(WSColor_t colorStruct);
void WS2812ClearColor_Index(uint8_t num);
void WS2812ClearColor_All();
void SetColor_StormInit();
void SetColor_Storm();
void SetColor_Fire();
void SetColor_AuroraInit();
void SetColor_Aurora();
void SetColor_Party();
void SetColor_Warning();
