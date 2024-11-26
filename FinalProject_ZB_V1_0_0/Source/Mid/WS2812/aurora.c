/*
 * aurora.c
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
int8_t next_dotIntensity[60]; //0->1 ~ 1->10
int8_t current_dotIntensity[60];
void SetColor_AuroraInit(){
	generalColorStruct.red.byte = rand()%255;
	generalColorStruct.green.byte = rand()%255;
	generalColorStruct.blue.byte = rand()%255;
}

void SetColor_Aurora(){
	static uint32_t timer = 0;
	timer++;
	isControllingAll = 0;
	currentAnim = Aurora;

	//RED
	if(timer % 10 == 0){
		if (rCount == 0){
			rCount = rand()%60;
			rDir = 1 - rand()%3; // -1, 0 or 1
		}else{
			if((generalColorStruct.red.byte == 254)&&(rDir > 0)){
				rDir = -1;
			}if((generalColorStruct.red.byte == 1)&&(rDir < 0)){
				rDir = 1;
			}
			generalColorStruct.red.byte += rDir;
			rCount--;
		}
		//GREEN
		if (gCount == 0){
			gCount = rand()%60;
			gDir = 1 - rand()%3; // -1, 0 or 1
		}else{
			if((generalColorStruct.green.byte == 254)&&(gDir > 0)){
				gDir = -1;
			}if((generalColorStruct.green.byte == 1)&&(gDir < 0)){
				gDir = 1;
			}
			generalColorStruct.green.byte += gDir;
			gCount --;
		}
		//BLUE
		if (bCount == 0){
			bCount = rand()%60;
			bDir = 1 - rand()%3; // -1, 0 or 1
		}else{
			if((generalColorStruct.blue.byte == 254)&&(bDir > 0)){
				bDir = -1;
			}if((generalColorStruct.blue.byte == 1)&&(bDir < 0)){
				bDir = 1;
			}
			generalColorStruct.blue.byte += bDir;
			bCount--;
		}
	}

	for(int i=0; i< LED_NUM; i++){
		if(current_dotIntensity[i] == next_dotIntensity[i])
			next_dotIntensity[i] =  rand()%20;
		current_dotIntensity[i] += (next_dotIntensity[i] - current_dotIntensity[i])/abs(next_dotIntensity[i] - current_dotIntensity[i]);
		currentStruct_Indexes[i].red.byte = (uint8_t)((float)generalColorStruct.red.byte * (float)current_dotIntensity[i]/20);
		currentStruct_Indexes[i].blue.byte = (uint8_t)((float)generalColorStruct.blue.byte * (float)current_dotIntensity[i]/20);
		currentStruct_Indexes[i].green.byte = (uint8_t)((float)generalColorStruct.green.byte * (float)current_dotIntensity[i]/20);
		WS2812SetColor_Index(i, currentStruct_Indexes[i]);
	}
}





