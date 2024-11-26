/*
 * ws2812.c
 *
 *  Created on: Sep 14, 2024
 *      Author: Admin
 */
#include "ws2812.h"
#include "Source/Driver/driver.h"
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


uint8_t outputBuffer[QUEUE_LEN+10];
uint8_t *outputBufferStartPtr = (uint8_t *)outputBuffer+10;
GPIO_Port_TypeDef ws2812_Port;
uint8_t ws2812_Pin;
EmberEventControl WS2812SubmitEventControl;
EmberEventControl ledAnimEventControl;
uint8_t testNum;
WSColor_t currentStruct_All, nextStruct_All;
WSColor_t currentStruct_Indexes[60];
uint8_t isControllingAll = 1;
uint8_t currentAnim = 0;



static void splitByteBit(uint8_t* buffer,BITstruct_t byte);
void WS2812Display_Struct();
void WS2812SubmitEventHandler();

void WS2812_Init(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin){
	ws2812_Port = gpioPort;
	ws2812_Pin = gpioPin;
	memset(outputBufferStartPtr, 0, 10);
	//initTIMER_WS2812(ws2812_Port, ws2812_Pin,(uint8_t*)outputBuffer, QUEUE_LEN, gpioModeWiredAndAlternateFilter);
	currentStruct_All.green.byte= 0;
	currentStruct_All.red.byte= 0;
	currentStruct_All.blue.byte= 0;
	WS2812ClearColor_All();
	initTIMERPWM(ws2812_Port, ws2812_Pin);
	initLDMA(outputBuffer, (uint16_t)QUEUE_LEN+10);
	WS2812SubmitEventHandler();
	//WS2812Display_Struct();
	//initLDMA();
	//emberAfCorePrintln("%d - %d = %d", *outputBuffer, *outputBufferStartPtr,outputBuffer- outputBufferStartPtr);
}

static void splitByteBit(uint8_t* buffer,BITstruct_t byte){
	*(buffer+0) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT7;
	*(buffer+1) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT6;
	*(buffer+2) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT5;
	*(buffer+3) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT4;
	*(buffer+4) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT3;
	*(buffer+5) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT2;
	*(buffer+6) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT1;
	*(buffer+7) = CODE_0_DUTYCYCLE + (CODE_1_DUTYCYCLE - CODE_0_DUTYCYCLE)*byte.BIT0;
}

void WS2812SetColor_Index(uint8_t Num, WSColor_t colorStruct){
	isControllingAll = 0;
	uint16_t base = ((uint16_t)Num)*24;
	splitByteBit((outputBufferStartPtr+base), (BITstruct_t)(colorStruct.green.byteAdvance));
	splitByteBit((outputBufferStartPtr+base+8), (BITstruct_t)(colorStruct.red.byteAdvance));
	splitByteBit((outputBufferStartPtr+base+16), (BITstruct_t)(colorStruct.blue.byteAdvance));
}

void WS2812SubmitEventHandler(){
	emberEventControlSetInactive(WS2812SubmitEventControl);
	emberEventControlSetInactive(ledAnimEventControl);
	if (isControllingAll){//If controlling all led -> Increment/Decrement slowly
		currentAnim = Manual;
		for(int i=0; i< LED_NUM; i++){
			uint16_t base = ((uint16_t)i)*24;
			splitByteBit((outputBufferStartPtr+base), (BITstruct_t)(currentStruct_All.green.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+8), (BITstruct_t)(currentStruct_All.red.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+16), (BITstruct_t)(currentStruct_All.blue.byteAdvance));
		}
		StartPWM();
		emberEventControlSetDelayMS(WS2812SubmitEventControl, 1000);
		emberEventControlSetDelayMS(ledAnimEventControl, 100);
	}else{
		WS2812ClearColor_All();
		currentStruct_All = nextStruct_All;
		for(int i=0; i< LED_NUM; i++){
			uint16_t base = ((uint16_t)i)*24;
			splitByteBit((outputBufferStartPtr+base), (BITstruct_t)(currentStruct_All.green.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+8), (BITstruct_t)(currentStruct_All.red.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+16), (BITstruct_t)(currentStruct_All.blue.byteAdvance));
		}
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 100);
	}
}

void WS2812SetColor_All(WSColor_t colorStruct){
	isControllingAll = 1;
	nextStruct_All = colorStruct;
}


void WS2812ClearColor_All(){
	isControllingAll = 1;
	nextStruct_All.green.byte= 0;
	nextStruct_All.red.byte= 0;
	nextStruct_All.blue.byte= 0;
	memset(currentStruct_Indexes,0,180);
}

void WS2812ClearColor_Index(uint8_t num){
	isControllingAll = 0;
	WSColor_t colorStruct;
	colorStruct.green.byte= 0;
	colorStruct.red.byte= 0;
	colorStruct.blue.byte= 0;
	WS2812SetColor_Index(num, colorStruct);
}

void WS2812Display_Struct(){
	for(int i=100; i>=0; i--){
		emberAfCorePrintln("%d . Data:%d", i, *(outputBufferStartPtr + i));
	}
}

void ledAnimEventHandler(){
	emberEventControlSetInactive(ledAnimEventControl);
	switch (currentAnim){
	case Storm:
		SetColor_Storm();
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	case Fire:
		SetColor_Fire();
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	case Aurora:
		SetColor_Aurora();
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	case Party:
		SetColor_Party();
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	case Warning:
		SetColor_Warning();
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	case Manual:
	case Rainbow:
		if((currentStruct_All.red.byte == nextStruct_All.red.byte)&&(currentStruct_All.green.byte == nextStruct_All.green.byte)&&(currentStruct_All.blue.byte == nextStruct_All.blue.byte)){
			break;
		}
		int16_t current, next;
		current = (int16_t)currentStruct_All.red.byte;
		next = (int16_t)nextStruct_All.red.byte;
		if (abs(current - next) > 3){
			currentStruct_All.red.byte += 2*(next - current)/abs(next - current);
		}else{
			currentStruct_All.red.byte = (uint8_t)next;
		}
		current = (int16_t)currentStruct_All.green.byte;
		next = (int16_t)nextStruct_All.green.byte;
		if (abs(current - next) > 3){
			currentStruct_All.green.byte += 2*(next - current)/abs(next - current);
		}else{
			currentStruct_All.green.byte = (uint8_t)next;
		}
		current = (int16_t)currentStruct_All.blue.byte;
		next = (int16_t)nextStruct_All.blue.byte;
		if (abs(current - next) > 3){
			currentStruct_All.blue.byte += 2*(next - current)/abs(next - current);
		}else{
			currentStruct_All.blue.byte = (uint8_t)next;
		}


		for(int i=0; i< LED_NUM; i++){
			uint16_t base = ((uint16_t)i)*24;
			splitByteBit((outputBufferStartPtr+base), (BITstruct_t)(currentStruct_All.green.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+8), (BITstruct_t)(currentStruct_All.red.byteAdvance));
			splitByteBit((outputBufferStartPtr+base+16), (BITstruct_t)(currentStruct_All.blue.byteAdvance));
		}
		StartPWM();
		emberEventControlSetDelayMS(ledAnimEventControl, 42);
		break;
	default:
		break;
	}
}




