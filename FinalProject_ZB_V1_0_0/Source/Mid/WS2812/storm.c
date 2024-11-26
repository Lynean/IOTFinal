/*
 * storm.c
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
uint8_t intensity;
WSColor_t colorStruct;

uint8_t rRat,gRat,bRat;
uint8_t raindrops[LED_NUM];
void SetColor_StormInit(){
	rRat = 7 + rand()%3;
	gRat = 7 + rand()%3;
	bRat = 7 + rand()%3;
	memset(raindrops, 0, LED_NUM);
}


void SetColor_Storm(){
	currentAnim = Storm;

	static uint32_t timer = 0;
	timer++;
	for(int i = 0; i< LED_NUM; i++){
		uint8_t done = 0;
		if(raindrops[i] != 0){
			if(currentStruct_Indexes[i].blue.byte > bRat){
				currentStruct_Indexes[i].blue.byte -= bRat;
			}else{
				currentStruct_Indexes[i].blue.byte = 0;
				done++;
			}
			if(currentStruct_Indexes[i].green.byte > gRat){
				currentStruct_Indexes[i].green.byte -= gRat ;

			}
			else{
				currentStruct_Indexes[i].green.byte = 0;
				done++;
			}
			if(currentStruct_Indexes[i].red.byte > rRat){
				currentStruct_Indexes[i].red.byte -=rRat;
			}
			else{
				currentStruct_Indexes[i].red.byte = 0;
				done++;
			}
			if(done <3){
				WS2812SetColor_Index(i, currentStruct_Indexes[i]);
			}
			else{
				emberAfCorePrintln("Raindrop deleted");
				raindrops[i]--;
				if(raindrops[i]){
					currentStruct_Indexes[i].red.byte = (uint8_t)((float)intensity/rRat);
					currentStruct_Indexes[i].green.byte = (uint8_t)((float)intensity/gRat);
					currentStruct_Indexes[i].blue.byte = (uint8_t)((float)intensity/bRat);
				}
				WS2812SetColor_Index(i, currentStruct_Indexes[i]);
			}
		}
	}
	if(timer%2== 0){
		uint8_t raindropPos = 0, intensity;
		raindropPos = rand()%LED_NUM;
		intensity = (20 + rand()%5);
		raindrops[raindropPos] = 3;
		colorStruct.red.byte = (uint8_t)((float)intensity*rRat);
		colorStruct.green.byte = (uint8_t)((float)intensity*gRat);
		colorStruct.blue.byte = (uint8_t)((float)intensity*bRat);
		currentStruct_Indexes[raindropPos] = colorStruct;
		WS2812SetColor_Index(raindropPos, colorStruct);
	}
	isControllingAll = 0;
}
