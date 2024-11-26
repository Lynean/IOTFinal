/*
 * kalman.c
 *
 *  Created on: 12-05-2024
 *      Author: Windows
 */

#include "kalman.h"
#include "Source/Driver/driver.h"


void kalmanInit(kalmanStruct* kalman,float est0, float est_err0, float mea_err0){
	kalman->est_prev = est0;
	kalman->est_err = est_err0;
	kalman->mea_err = mea_err0;
}
static void kalmanUpdGain(kalmanStruct* kalman){
	kalman->gain = (kalman->est_err)/(kalman->est_err + kalman->mea_err);
}
static void kalmanUpdEstimate(kalmanStruct* kalman){
	kalman->est_current = kalman->est_prev + kalman->gain*(kalman->mea - kalman->est_prev);
}

static void kalmanUpdEstError(kalmanStruct* kalman){
	kalman->est_err = (1 - kalman->gain)*kalman->est_prev;
}

float kalmanUpd(kalmanStruct* kalman, float measurement){
	kalman->mea = measurement;
	kalmanUpdGain(kalman);
	kalmanUpdEstimate(kalman);
	kalmanUpdEstError(kalman);
	float ret = kalman->est_prev;
	kalman->est_prev = kalman->est_current;
	return ret;
}
