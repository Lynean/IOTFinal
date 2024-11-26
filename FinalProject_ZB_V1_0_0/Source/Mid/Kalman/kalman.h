/*
 * kalman.h
 *
 *  Created on: 12-05-2024
 *      Author: Windows
 */

#ifndef SOURCE_MID_KALMAN_KALMAN_H_
#define SOURCE_MID_KALMAN_KALMAN_H_



#endif /* SOURCE_MID_KALMAN_KALMAN_H_ */

#include "app/framework/include/af.h"

typedef struct{
	float est_current; 	//Current estimation
	float est_prev;		//Previous estimation
	float est_err;		//Estimation error
	float mea;			//Measurement
	float mea_err;		//Measurement error
	float gain;			//Kalman gain
}kalmanStruct;


void kalmanInit(kalmanStruct* kalman,float est0, float est_err0, float mea_err0);
float kalmanUpd(kalmanStruct* kalman, float measurement);
