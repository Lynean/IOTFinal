/*
 * receive.c
 *
 *  Created on: 20-05-2024
 *      Author: Windows
 */

#include "Source/App/app.h"
#include "Source/Mid/mid.h"

colorPWM_t RGBState;

void RECEIVE_OnOffCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t DEP, uint8_t* buffer, uint8_t payloadStartIndex){
	if (clusterSpecific == 0){
		if(cmd == ZCL_WRITE_ATTRIBUTES_COMMAND_ID){
			buffer += payloadStartIndex;
			OnOffPayload_t* pointer;
			pointer = (OnOffPayload_t*)buffer;
//			for(int i = 0; i< 5; i++)
//				emberAfCorePrintln("0x%X ", (uint8_t)(*(buffer + i)));
			emberAfCorePrintln("AttrID: 0x%X, DatType:0x%X, Val: 0x%X ", pointer->attributeID, pointer->dataType,(pointer->value));
			switch(pointer->attributeID){
			case 0x0000:
				if(((pointer->value)) == 0x01){
					emberAfCorePrintln("ON");
					RGBState.mode = Manual;
				}else if((pointer->value) == 0x00){
					emberAfCorePrintln("OFF");
					RGBState.mode = Off;
					setColorControl_Color(RGBState.red, RGBState.green, RGBState.blue, RGBState.mode);
				}
				break;
			default:
				break;
			}
		}
	}
}


void RECEIVE_ColorControlCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t* buffer, uint8_t payloadStartIndex){
	buffer += payloadStartIndex;
	ColorControlPayload_t* pointer;
	pointer = (ColorControlPayload_t*)buffer;
	if (!clusterSpecific){
		if(cmd == ZCL_WRITE_ATTRIBUTES_COMMAND_ID){
//			for(int i = 0; i< 5; i++)
//				emberAfCorePrintln("0x%X ", (uint8_t)(*(buffer + i)));
			emberAfCorePrintln("AttrID: 0x%X, DatType:0x%X, Val: 0x%X ", pointer->attributeID, pointer->dataType,(pointer->value));
			switch(pointer->attributeID){
			case 0x0034:
				RGBState.red = pointer->value;
				setColorControl_Color(RGBState.red, RGBState.green, RGBState.blue, RGBState.mode);
				break;
			case 0x0038:
				RGBState.green = pointer->value;
				setColorControl_Color(RGBState.red, RGBState.green, RGBState.blue, RGBState.mode);
				break;
			case 0x003C:
				RGBState.blue = pointer->value;
				setColorControl_Color(RGBState.red, RGBState.green, RGBState.blue, RGBState.mode);
				break;
			case 0x0008:
				//Set ColorMode to value
				RGBState.mode = pointer->value;
				setColorControl_Color(RGBState.red, RGBState.green, RGBState.blue, RGBState.mode);
				break;
			default:
				break;
			}
		}
	}
}

void RECEIVE_TimeCommandHandle(uint8_t clusterSpecific, uint8_t cmd, uint8_t* buffer, uint8_t payloadStartIndex){
	buffer += payloadStartIndex;
	TimePayload_t* pointer;
	pointer = (TimePayload_t*)buffer;
	uint8_t hour,minute,room;
	if (!clusterSpecific){
		if(cmd == ZCL_WRITE_ATTRIBUTES_COMMAND_ID){
//			for(int i = 0; i< 5; i++)
//				emberAfCorePrintln("0x%X ", (uint8_t)(*(buffer + i)));
			emberAfCorePrintln("AttrID: 0x%X, DatType:0x%X", pointer->attributeID, pointer->dataType);
			switch(pointer->attributeID){
			case 0x0000:
				if((pointer->Minute != 0xFF)){
					minute = pointer->Minute;
					timeInit(minute, 0xFF);
				}if((pointer->Hour != 0xFF)){
					hour = pointer->Hour;
					timeInit(0xFF, hour);
				}
				if(pointer->Room != 0xFF){
					emberAfCorePrintln("ROOM: %d", pointer->Room);
					room = pointer->Room;
					setColorControl_Room(room);
				}
				break;
			default:
				break;
			}
		}
	}
}
