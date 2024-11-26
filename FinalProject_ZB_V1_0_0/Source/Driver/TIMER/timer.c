/*
 * timer.c
 *
 *  Created on: Aug 19, 2024
 *      Author: Admin
 */

#include "timer.h"
#define BUFFER_SIZE 1440
// Duty cycle global variable for IRQ handler use
uint32_t dutyCycle_CC0 = 0, dutyCycle_CC1 = 0, dutyCycle_CC2 = 0;
uint32_t NEWdutyCycle_CC0 = 0, NEWdutyCycle_CC1 = 0, NEWdutyCycle_CC2 = 0;
static PWMInitStruct_t AttributeStruct;
EmberEventControl changePWMEventControl;
uint8_t* baseQueueAddress;
uint16_t baseQueueLen;
uint8_t* currentQueuePtr;
uint16_t remainQueueLen;
uint8_t currentReadBit=0;
timerMode_t TimerMode = Queue;
uint32_t debugCount=0;
uint16_t count= 0;
// Globally declared link descriptor
LDMA_Descriptor_t descLink;

//1.25us = 100 NOP
#define SHORT_DELAY __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
        			__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
        			__NOP(); __NOP(); __NOP(); __NOP(); __NOP();__NOP(); __NOP(); __NOP();; __NOP(); __NOP();  \
        			__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();; __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();; __NOP(); __NOP();  \
					__NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();; __NOP(); __NOP();
void Timer_QueueModeHandle();
void Timer_SingleModeHandle();
void initTIMER_WS2812(GPIO_Port_TypeDef gpioPort, uint8_t gpioPin, uint8_t* buffer, uint16_t bufferlen , GPIO_Mode_TypeDef gpioMode)
{
	baseQueueAddress = buffer;
	baseQueueLen = bufferlen;

	AttributeStruct.channelCount =1;
	AttributeStruct.CC0Port = gpioPort;
	AttributeStruct.CC0Pin = gpioPin;
	GPIO_PinModeSet(gpioPort, gpioPin, gpioMode, 0);

	uint32_t timerFreq, topValue, dutyCount;
	TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
	TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

	// Don't start counter on initialization
	timerInit.enable = false;

	// PWM mode sets/clears the output on compare/overflow events
	timerCCInit.mode = timerCCModePWM;
	timerCCInit.outInvert = false;
	timerCCInit.filter = true;
	timerCCInit.cmoa = timerOutputActionClear;
	TIMER_Init(TIMER1, &timerInit);

	// Enable routes
	GPIO->TIMERROUTE[1].ROUTEEN  |= GPIO_TIMER_ROUTEEN_CC0PEN;
	GPIO->TIMERROUTE[1].CC0ROUTE = (gpioPort << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT) | (gpioPin << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);

	TIMER_InitCC(TIMER1, 0, &timerCCInit);
	// Set top value to overflow at the desired PWM_FREQ frequency
	timerFreq = CMU_ClockFreqGet(cmuClock_TIMER1) / (timerInit.prescale + 1);
	emberAfCorePrintln("FREQ: %d\n" ,CMU_ClockFreqGet(cmuClock_TIMER1));
	topValue = (timerFreq / PWM_FREQ);
	TIMER_TopSet(TIMER1, topValue);
	TIMER_CompareSet(TIMER1, 0, 32);
	// Now start the TIMER
	//TIMER_Enable(TIMER1, true);

	// Enable TIMER0 compare event interrupts to update the duty cycle
	TIMER_IntEnable(TIMER1, TIMER_IEN_CC0);
	NVIC_EnableIRQ(TIMER1_IRQn);

}




/**************************************************************************//**
 * @brief
 *    Interrupt handler for TIMER0
 *
 * @note
 *    In this example, the duty cycle of the output waveform does not
 *    change unless the value of the dutyCycle global variable is
 *    modified outside the scope of this function.
 *
 *    Alternatively, other code could be inserted here to modify the
 *    duty cycle based on some other input, e.g. the voltage measured
 *    by the IADC on a given input channel.
 *****************************************************************************/
//void TIMER1_IRQHandler(void)
//{
//	//If 0 then 32 if 1 then 54
//	//uint32_t newDutyCount;
//	// Acknowledge the interrupt
//	uint32_t flags = TIMER_IntGet(TIMER1);
//	TIMER_IntClear(TIMER1, flags);
//	if ((flags & TIMER_IF_CC0) != 0){
//		Timer_QueueModeHandle();
//		stopPWM();
//	}else if(((flags & TIMER_IF_OF) != 0)){
//		startPWM();
//	}else{
//		stopPWM();
//	}
//
//
//
//	//emberAfCorePrintln("Countdown:%d", remainQueueLen);
//}

void timerSetDutyCycle(uint8_t CC0, uint8_t CC1, uint8_t CC2){
	emberEventControlSetInactive(changePWMEventControl);
	NEWdutyCycle_CC0 = CC0;
	NEWdutyCycle_CC1 = CC1;
	NEWdutyCycle_CC2 = CC2;
	TimerMode = Single;
	emberEventControlSetDelayMS(changePWMEventControl, 100);
}
void changePWMEventHandler(){
	emberEventControlSetInactive(changePWMEventControl);
	switch (TimerMode){
	case Single:
		break;
	case Queue:
		Timer_QueueModeHandle();
		startPWM();
		break;
	case Idle:
		break;
	}

}
void Timer_QueueModeHandle(){
	uint8_t val;
	val = *currentQueuePtr;
	static uint8_t lastCode = 0;
	if(remainQueueLen >0){
		if(val != lastCode){
			TIMER_CompareBufSet(TIMER1, 0, val);
			lastCode = val;
		}
		currentQueuePtr++;
		remainQueueLen--;
	}else{
		stopPWM();
		lastCode = 0;
		//emberAfCorePrintln("Count%d",count);
	}
}
void timerQueueDutyCycle(){
	emberEventControlSetInactive(changePWMEventControl);
	TimerMode = Queue;
	currentQueuePtr = baseQueueAddress;
	remainQueueLen = baseQueueLen;
	emberEventControlSetDelayMS(changePWMEventControl,10);
}

void stopPWM(){
	//LDMA_StopTransfer(0);
	TIMER_Enable(TIMER1, false);
	GPIO_PinOutClear(AttributeStruct.CC0Port, AttributeStruct.CC0Pin);
	GPIO_PinOutClear(AttributeStruct.CC1Port, AttributeStruct.CC1Pin);
	GPIO_PinOutClear(AttributeStruct.CC2Port, AttributeStruct.CC2Pin);
}

void startPWM(){
	TIMER_Enable(TIMER1, true );
	//LDMA_TransferCfg_t periTransferTx =
			            //LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER0_CC0);
	//LDMA_StartTransfer(0, (void*)&periTransferTx, (void*)&descLink);
}



