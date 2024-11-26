/*
 * pwm.c
 *
 *  Created on: Oct 5, 2024
 *      Author: Admin
 */


#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_ldma.h"
#include "em_timer.h"
#include "pwm.h"
//  Desired frequency in Hz
#define PWM_FREQ 800000

/*
 * This table holds the time calculated for each given duty cycle value
 * expressed as a percent.  Note that BUFFER_SIZE must match the number
 * of values in dutyCyclePercentages[BUFFER_SIZE].
 */

unsigned int CurrentDMAChannel;
// Globally declared link descriptor
LDMA_Descriptor_t descLink;
bool USER_DMACallback(unsigned int channel,unsigned int sequenceNo);
/**************************************************************************//**
 * @brief GPIO initialization
 *****************************************************************************/


/**************************************************************************//**
 * @brief
 *    TIMER initialization
 *****************************************************************************/
void initTIMERPWM(GPIO_Port_TypeDef port, uint8_t pin)
{
  GPIO_PinModeSet(port, pin, gpioModeWiredAndAlternate, 0);
  uint32_t timerFreq, topValue;
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef timerCCInit = TIMER_INITCC_DEFAULT;

  // Don't start counter on initialization
  timerInit.enable = false;
  //timerInit.dmaClrAct = 1;
  // PWM mode sets/clears the output on compare/overflow events
  timerCCInit.mode = timerCCModePWM;

  TIMER_Init(TIMER1, &timerInit);

  // Route CC0 output to PC0
  GPIO->TIMERROUTE[1].ROUTEEN  = GPIO_TIMER_ROUTEEN_CC1PEN;
  GPIO->TIMERROUTE[1].CC1ROUTE = (port << _GPIO_TIMER_CC1ROUTE_PORT_SHIFT)
                    | (pin << _GPIO_TIMER_CC1ROUTE_PIN_SHIFT);
  TIMER_InitCC(TIMER1, 1, &timerCCInit);

  // Set top value to overflow at the desired PWM_FREQ frequency
  timerFreq = CMU_ClockFreqGet(cmuClock_TIMER1) / (timerInit.prescale + 1);
  topValue = (timerFreq / PWM_FREQ);
  TIMER_TopSet(TIMER1, topValue);

  // Now start the TIMER
  TIMER_Enable(TIMER1, true);

  // Trigger DMA on compare event to set CCVB to update duty cycle on next period
  TIMER_IntEnable(TIMER1, TIMER_IEN_CC1);
}

/**************************************************************************//**
* @brief
*    Initialize the LDMA module
*
* @details
*    Configure the channel descriptor to use the default peripheral to
*    memory transfer descriptor. Modify it to not generate an interrupt
*    upon transfer completion (we don't need it). Additionally, the transfer
*    configuration selects the TIMER0_CC0 signal as the trigger for the LDMA
*    transaction to occur.
*
* @note
*    The descriptor object needs to at least have static scope persistence so
*    that the reference to the object is valid beyond its first use in
*    initialization
*****************************************************************************/
void initLDMA(uint8_t* buffer, uint16_t size)
{
//	DMADRV_Init();
	DMADRV_AllocateChannel(&CurrentDMAChannel, NULL);
//  LDMA_StopTransfer(0);
//  // Default LDMA initialization
//  LDMA_DeInit();
//  LDMA_Init_t init = LDMA_INIT_DEFAULT;
//  LDMA_Init(&init);

  // Request transfers on CC0 peripheral requests
  LDMA_TransferCfg_t periTransferTx =
		  LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER1_CC1);

  // Transfer from the RAM buffer to CC0 output register in a continuous loop
  LDMA_Descriptor_t xfer = LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(buffer,             // Memory source address
                                     &TIMER1->CC[1].OCB,  // Output compare buffer register
									 size);         // Number of transfers to make
  descLink = xfer;
  // Start transfer, LDMA will trigger after first compare event
  //LDMA_StartTransfer(4, (void*)&periTransferTx, (void*)&descLink);
  uint8_t code;
  code = DMADRV_LdmaStartTransfer(CurrentDMAChannel,
  							&periTransferTx,
  							&descLink,
  							(DMADRV_Callback_t)USER_DMACallback,
  							NULL);
  emberAfCorePrintln("CODE: %d!",code);
}
void StopPWM(){
	TIMER_Enable(TIMER1, false );
	DMADRV_StopTransfer(CurrentDMAChannel);
}

void StartPWM(){
	uint32_t* dest;
	dest = &TIMER1->CC[1].OCB;
//	DMADRV_MemoryPeripheral(4,
//							dmadrvPeripheralSignal_TIMER1_CC1,
//							dest,
//							buffer,
//							true,
//							size,
//							ldmaCtrlSizeWord,
//							(DMADRV_Callback_t)USER_DMACallback,
//							NULL);
	LDMA_TransferCfg_t periTransferTx = LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER1_CC1);
	  uint8_t code;
	  code = DMADRV_LdmaStartTransfer(CurrentDMAChannel,
								&periTransferTx,
								&descLink,
								(DMADRV_Callback_t)USER_DMACallback,
								NULL);
	  //emberAfCorePrintln("CODE: %d!",code);
	TIMER_Enable(TIMER1, true );
}


bool USER_DMACallback(unsigned int channel,unsigned int sequenceNo){
	//StopPWM();
	TIMER1->CC[1].OCB = 0x00000000;
	//emberAfCorePrintln("DMA!");
	return true;
}
