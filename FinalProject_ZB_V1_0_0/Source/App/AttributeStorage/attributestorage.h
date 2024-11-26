/*
 * attributestorage.h
 *
 *  Created on: Aug 25, 2024
 *      Author: Admin
 */

#ifndef SOURCE_APP_ATTRIBUTESTORAGE_ATTRIBUTESTORAGE_H_
#define SOURCE_APP_ATTRIBUTESTORAGE_ATTRIBUTESTORAGE_H_



#endif /* SOURCE_APP_ATTRIBUTESTORAGE_ATTRIBUTESTORAGE_H_ */

#include "app/framework/include/af.h"

#define HOUR_NUM 24
#define ENV_NUM 3
//							 Red   Green  Blue
#define Light_Yellow		{1.00, 1.00, 0.40}
#define Yellow				{1.00, 1.00, 0.00}
#define Light_Golden_Yellow	{0.98, 0.98, 0.82}
#define Yellow_Mansell		{0.94, 0.80, 0.00}
#define Maroon				{0.50, 0.00, 0.00}
#define Light_Sea_Green		{0.12, 0.79, 0.66}
#define Dark_Salmon			{0.91, 0.59, 0.48}
#define Salmon				{0.98, 0.50, 0.45}
#define Light_Salmon		{1.00, 0.63, 0.48}
#define Indian_Red			{0.80, 0.36, 0.36}
#define Peach_Puff			{1.00, 0.85, 0.72}
#define Pale_Turquoise		{0.68, 0.93, 0.93}
#define Turquoise			{0.25, 0.87, 0.82}
#define Cyan				{0.00, 1.00, 1.00}
#define Light_Cyan			{0.88, 1.00, 1.00}
#define Cadet_Blue			{0.36, 0.62, 0.63}
//Thresholds
#define TEMP_HOT	32
#define TEMP_COLD	15
#define HUM_MAX		90
#define HUM_MIN		40
typedef enum{
	Sleep,
	Study,
	Dining,
	Excercise,
	Living
}lightEnvironment_t;

typedef enum{LY,Y,LGY,YM,M,LSG,DS,S,LS,IR,PP,PT,T,C,LC,CB
}baseColor_t;

typedef struct {
	uint8_t r,g,b;
	lightEnvironment_t room;
}RGBColor_t;

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
void setColorControl_Color(uint8_t r, uint8_t g, uint8_t b, uint8_t mode);
void proccessColor(baseColor_t* colors, float* rbgFinal);
void setLightAttribute(uint16_t light);
void setTempAttribute(int16_t temp);
void setHumAttribute(uint16_t hum);
void calculateRGBFromCCT(uint32_t CCT);
void setColorControl_Room(uint8_t room);
