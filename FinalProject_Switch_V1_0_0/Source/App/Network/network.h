/*
 * network.h
 *
 *  Created on: Jun 6, 2024
 *      Author: Admin
 */

#ifndef SOURCE_APP_NETWORK_NETWORK_H_
#define SOURCE_APP_NETWORK_NETWORK_H_



#endif /* SOURCE_APP_NETWORK_NETWORK_H_ */

#include "app/framework/include/af.h"

typedef void ((*NETWORK_EventHandle)(uint8_t));

typedef enum{
	NETWORK_JOIN_SUCCESS,
	NETWORK_JOIN_FAIL,
	NETWORK_HAS_PARENT,
	NETWORK_LOST_PARENT,
	NETWORK_OUT_NETWORK
}NWK_State;

void NETWORK_Init(NETWORK_EventHandle HandleFunc);
void NETWORK_FindAndJoin(void);
void NETWORK_StopFindAndJoin(void);
void NETWORK_Leave(void);
