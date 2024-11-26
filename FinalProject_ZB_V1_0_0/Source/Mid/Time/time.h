/*
 * time.h
 *
 *  Created on: Aug 28, 2024
 *      Author: Admin
 */

#ifndef SOURCE_MID_TIME_TIME_H_
#define SOURCE_MID_TIME_TIME_H_



#endif /* SOURCE_MID_TIME_TIME_H_ */

#include "app/framework/include/af.h"
typedef struct{
	uint8_t minutes;
	uint8_t hours;
}Time_t;

void timeInit(uint8_t minutes, uint8_t hours);
Time_t getTime();
