/*
 * warning.c
 *
 *  Created on: Nov 12, 2024
 *      Author: Admin
 */

#include "ws2812.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
typedef enum{
	Manual,
	Rainbow,
	Storm,
	Fire,
	Aurora,
	Party,
	Warning,
	Off
}ColorMode_t;
//extern WSColor_t currentStruct_Indexes[60];
extern uint8_t currentAnim;
extern uint8_t isControllingAll;
WSColor_t generalColorStruct;
int16_t rCount, gCount, bCount;
int8_t rDir, gDir, bDir;

void SetColor_Warning(){
	currentAnim = Warning;
	WSColor_t red;
	static int8_t intensity = 0;
	if(intensity == 25)
		intensity = -25;
	red.red.byte = (uint8_t)(10*(float)abs(intensity));
	red.green.byte = (uint8_t)(5*(float)abs(intensity));
	intensity += 1;
	for(int i=0; i< LED_NUM; i++){
		WS2812SetColor_Index(i, red);
	}
	isControllingAll = 0;
}



