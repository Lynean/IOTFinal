/*
 * time.c
 *
 *  Created on: Aug 28, 2024
 *      Author: Admin
 */

#include "../../Driver/driver.h"
#include "time.h"

Time_t currentTime;
EmberEventControl minuteTimerEventControl;

void timeInit(uint8_t minutes, uint8_t hours){
	emberEventControlSetInactive(minuteTimerEventControl);
	if(minutes != 0xFF){
		currentTime.minutes = minutes;
	}
	if(hours != 0xFF){
		currentTime.hours = hours;
	}
	emberEventControlSetDelayMS(minuteTimerEventControl, 100);
}

void minuteTimerEventHandler(){
	emberEventControlSetInactive(minuteTimerEventControl);
	if (currentTime.minutes < 59)
		currentTime.minutes++;
	else{
		currentTime.hours++;
		currentTime.minutes = 0;
		if(currentTime.hours >= 24){
			currentTime.hours = 0;
		}
	}
	emberAfCorePrintln("Current %d : %d\n",currentTime.hours, currentTime.minutes);
	emberEventControlSetDelayMS(minuteTimerEventControl, 60000);
}

Time_t getTime(){
	return currentTime;
}
