/*
 * attributestorage.c
 *
 *  Created on: Aug 25, 2024
 *      Author: Admin
 */


#include "Source/Mid/mid.h"
#include "Source/App/app.h"

EmberEventControl changeColorEventControl;
extern EmberEventControl WS2812SubmitEventControl;
static uint16_t lightAttribute;
static int16_t tempAttribute;
static uint16_t humAttribute;
RGBColor_t color = {0,0,0,Sleep};
ColorMode_t ColorMode;

static void SetLight_Sleep();
static void SetLight_Study();
static void SetLight_Dining();
static void SetLight_Exercise();
static void SetLight_Living();

void setColorControl_Color(uint8_t r, uint8_t g, uint8_t b, uint8_t mode){
	emberEventControlSetInactive(changeColorEventControl);
	color.r = r;
	color.g = g;
	color.b = b;
	ColorMode = mode;
	emberAfCorePrintln("Code: %d",ColorMode);
	emberEventControlSetDelayMS(changeColorEventControl, 100);
}

void setColorControl_Room(uint8_t room){
	emberEventControlSetInactive(changeColorEventControl);
	color.room = room;
	emberEventControlSetDelayMS(changeColorEventControl, 100);
}

void setLightAttribute(uint16_t light){
	lightAttribute = light;
}

void setTempAttribute(int16_t temp){
	tempAttribute = temp;
}

void setHumAttribute(uint16_t hum){
	humAttribute = hum;
}


void setColorAutomatic(RGBColor_t color){
	switch(color.room){
	case Sleep:
		SetLight_Sleep();
		break;
	case Study:
		SetLight_Study();
		break;
	case Dining:
		SetLight_Dining();
		break;
	case Excercise:
		SetLight_Exercise();
		break;
	case Living:
		SetLight_Living();
		break;
	}
}

static void SetLight_Sleep(){
	Time_t time = getTime();
	uint8_t maxIntensity = 255;
	if (time.hours < 6){			//0 -> 6
		float rgbRatio[3] = Maroon;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else if(time.hours < 13){		//6 -> 13
		float rgbRatio[3] = {0,0,0};
		baseColor_t colorArray[3] = {LY, Y, LGY};
		proccessColor(colorArray, rgbRatio);
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else if(time.hours < 16){ 		// 13 -> 16
		float rgbRatio[3] = Yellow_Mansell;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else if(time.hours < 22){		// 16 -> 22
		float rgbRatio[3] = {0,0,0};
		baseColor_t colorArray[3] = {LY, Y, LGY};
		proccessColor(colorArray, rgbRatio);
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else{							// 22 -> 00
		float rgbRatio[3] = Maroon;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}
}

static void SetLight_Study(){
	uint8_t maxIntensity = 255;
	float rgbRatio[3] = {0,0,0};
	baseColor_t colorArray[3] = {LSG, Y, LY};
	proccessColor(colorArray, rgbRatio);
	color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
	color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
	color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
}

static void SetLight_Dining(){
	Time_t time = getTime();
	uint8_t maxIntensity = 255;
	if (time.hours < 6){			//0 -> 7
		float rgbRatio[3] = Dark_Salmon;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else if(time.hours < 12){		//6 -> 12
		float rgbRatio[3] = Indian_Red;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else{							//12 -> 00
		float rgbRatio[3] = Indian_Red;
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}
}

static void SetLight_Exercise(){
	uint8_t maxIntensity = 255;
	float rgbRatio[3] = {0,0,0};
	baseColor_t colorArray[3] = {PP, S, LS};
	proccessColor(colorArray, rgbRatio);
	color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
	color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
	color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
}

static void SetLight_Living(){
	Time_t time = getTime();
	uint8_t maxIntensity = 255;
	float rgbRatio[3] = {0,0,0};
	if (time.hours < 8){			//0 -> 8
		baseColor_t colorArray[3] = {PT, C, CB};
		proccessColor(colorArray, rgbRatio);
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else if(time.hours < 17){		//8 -> 17
		baseColor_t colorArray[3] = {LC, C, T};
		proccessColor(colorArray, rgbRatio);
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}else{							//12 -> 00
		baseColor_t colorArray[3] = {PT, C, CB};
		proccessColor(colorArray, rgbRatio);
		color.r = (uint8_t) (rgbRatio[0]*maxIntensity);
		color.g = (uint8_t) (rgbRatio[1]*maxIntensity);
		color.b = (uint8_t) (rgbRatio[2]*maxIntensity);
	}
}

void proccessColor(baseColor_t* colors, float* rgbFinal){
	baseColor_t* colorPtr = colors;
	float rgb[3][3];
	for (int i = 0; i<3; i++){
		switch(*colorPtr){
			case LY:{
				float temp[3] = Light_Yellow;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case Y:{
				float temp[3] = Yellow;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case LGY:{
				float temp[3] = Light_Golden_Yellow;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case YM:{
				float temp[3] = Yellow_Mansell;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case M:{
				float temp[3] = Maroon;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case LSG:{
				float temp[3] = Light_Sea_Green;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case DS:{
				float temp[3] = Dark_Salmon;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case S:{
				float temp[3] = Salmon;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case LS:{
				float temp[3] = Light_Salmon;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case IR:{
				float temp[3] = Indian_Red;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case PP:{
				float temp[3] = Peach_Puff;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case PT:{
				float temp[3] = Pale_Turquoise;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case T:{
				float temp[3] = Turquoise;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case C:{
				float temp[3] = Cyan;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case LC:{
				float temp[3] = Light_Cyan;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			case CB:{
				float temp[3] = Cadet_Blue;
				// Assign values from the temporary array.
				for (int j = 0; j < 3; j++)
					rgb[i][j] = temp[j];
				break;
			}
			default:
				break;
			}
		colorPtr++;
	}
	//Temp < 15oC
	if (tempAttribute >= TEMP_HOT){
		// = 1st element
		*(rgbFinal + 0) = rgb[0][0];
		*(rgbFinal + 1) = rgb[0][1];
		*(rgbFinal + 2) = rgb[0][2];
	}//Temp > 32oC
	else if(tempAttribute <= TEMP_COLD){
		// = 2nd element
		*(rgbFinal + 0) = rgb[1][0];
		*(rgbFinal + 1) = rgb[1][1];
		*(rgbFinal + 2) = rgb[1][2];

	}//Humid > 95%
	else if(humAttribute >= HUM_MAX){
		*(rgbFinal + 0) = rgb[2][0];
		*(rgbFinal + 1) = rgb[2][1];
		*(rgbFinal + 2) = rgb[2][2];
	}
	else{
		//Temperature Compare
		float ColdCo, HotCo, HumCo;
		HotCo = ((float)tempAttribute - TEMP_COLD)/(TEMP_HOT-TEMP_COLD);
		HumCo = ((float)humAttribute - HUM_MIN)/(HUM_MAX-HUM_MIN);
		ColdCo = 1 - HotCo;
		if((HumCo >= HotCo) && (HumCo >= ColdCo)){ //MONSOON
			*(rgbFinal + 0) = rgb[2][0];
			*(rgbFinal + 1) = rgb[2][1];
			*(rgbFinal + 2) = rgb[2][2];
		}
		else if((HotCo >= HumCo) && (HotCo >= ColdCo)){ //TEMP HOT
			*(rgbFinal + 0) = rgb[0][0];
			*(rgbFinal + 1) = rgb[0][1];
			*(rgbFinal + 2) = rgb[0][2];
		}else{
			*(rgbFinal + 0) = rgb[1][0];
			*(rgbFinal + 1) = rgb[1][1];
			*(rgbFinal + 2) = rgb[1][2];
		}
	}
}

void changeColorEventHandler(){
	emberEventControlSetInactive(changeColorEventControl);
	WSColor_t* WScolor;
	switch(ColorMode){
	case Rainbow:
		setColorAutomatic(color);
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		WScolor = (WSColor_t*) &color;
		WS2812SetColor_All(*WScolor);
		emberEventControlSetDelayMS(changeColorEventControl, 5000);
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Storm:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		SetColor_StormInit();
		SetColor_Storm();
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Fire:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		SetColor_Fire();
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Aurora:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		SetColor_AuroraInit();
		SetColor_Aurora();
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Party:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		SetColor_Party();
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Warning:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0008, ColorMode);
		//ledPWMControl(color.r, color.g, color.b, 1);
		SetColor_Warning();
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	case Manual:
		SEND_OnOffState(1, 1);
		SEND_ColorState(1, 0x0034, color.r);
		SEND_ColorState(1, 0x0038, color.g);
		SEND_ColorState(1, 0x003C, color.b);
		//ledPWMControl(color.r, color.g, color.b, 1);
		WScolor = (WSColor_t*) &color;
		WS2812SetColor_All(*WScolor);
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	default: //Off
		SEND_OnOffState(1, 0);
		SEND_ColorState(1, 0x0034, color.r);
		SEND_ColorState(1, 0x0038, color.g);
		SEND_ColorState(1, 0x003C, color.b);
		//ledPWMControl(color.r, color.g, color.b, 0);
		WScolor = (WSColor_t*) &color;
		WS2812ClearColor_All(*WScolor);
		emberEventControlSetActive(WS2812SubmitEventControl);
		break;
	}
}

void calculateRGBFromCCT(uint32_t CCT){
	//Red
	if (CCT <= 66){
		color.r = 255;
	}else{
		color.r = CCT - 60;
		color.r = 329.698727446 * (pow((float)color.r, (-0.1332047592)));
		if (color.r>255){
			color.r = 255;
		}else if(color.r<0){
			color.r = 0;
		}
	}
	//Green
	if (CCT <= 66){
		color.r = CCT;
		color.r = 99.4708025861 * log((float)color.r) - 161.1195681661;
		if (color.r>255){
			color.r = 255;
		}else if(color.r<0){
			color.r = 0;
		}
	}else{
		color.r = CCT - 60;
		color.r = 288.1221695283 * (pow((float)color.r, (-0.0755148492)));
		if (color.r>255){
			color.r = 255;
		}else if(color.r<0){
			color.r = 0;
		}
	}
	//Blue
	if (CCT >= 66){
		color.b = 255;
	}else if(CCT <= 19){
		color.b = 0;
	}else{
		color.b = CCT - 10;
		color.b = 138.5177312231 * log((float)color.b) - 305.0447927307;
		if (color.b>255){
			color.b = 255;
		}else if(color.b<0){
			color.b = 0;
		}
	}
}
