/*
 * party.c
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
extern WSColor_t currentStruct_Indexes[60];
extern uint8_t currentAnim;
extern uint8_t isControllingAll;
WSColor_t generalColorStruct;
int16_t rCount, gCount, bCount;
int8_t rDir, gDir, bDir;

void SetColor_Party(){
	currentAnim = Party;
	WSColor_t new, temp;
	new.blue.byte =  rand()%2*255;
	new.red.byte =  rand()%2*255;
	new.green.byte =  rand()%2*255;
	temp = new;
	for (int i = 0; i< LED_NUM; i++){
		if(i != 59){
			new = currentStruct_Indexes[i];
		}
		currentStruct_Indexes[i] = temp;
		temp = new;
		WS2812SetColor_Index(i,temp);
	}
	isControllingAll = 0;
}





