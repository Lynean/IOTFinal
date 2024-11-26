/*
 * matrixop.c
 *
 *  Created on: Aug 31, 2024
 *      Author: Admin
 */

#include "arrayop.h"

void arrayMultiply(float* mat, float multiplier, int len){
	float* ptr = mat;
	for(int i=0;i<len;i++){
		*ptr *= multiplier;
		ptr++;
	}
}

void arrayDivide(float* mat, float divider, int len){
	float* ptr = mat;
	for(int i=0;i<len;i++){
		*ptr /= divider;
		ptr++;
	}
}

void arrayAdd(float* mat1, float* mat2, int len){
	float* ptr1 = mat1;
	float* ptr2 = mat2;
	for(int i=0;i<len;i++){
		*ptr1 += *ptr2;
		ptr1++;
		ptr2++;
	}
}
