/*
    HC-SR04
		SR04.c
		Purpose: making an interface for HC-SR04 sonar sensor
		
		@author: Michiel van Dalfsen
		@version: 1.0
*/

// ----------------------------------------------------------------------------
// Incudes
// ----------------------------------------------------------------------------
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SR04.h"
#include "delay.h"

// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define SR04_OFFSET  0.0


// ----------------------------------------------------------------------------
// Timer 2 init, frequency: ~15Hz.
// ----------------------------------------------------------------------------
static void TIM2_Init(void) {
  uint16_t iPrescalerValue = 0;
  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Compute the prescaler value */
  iPrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 65535 - 1;      //in uSecs
  TIM_TimeBaseStructure.TIM_Prescaler = iPrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_Cmd(TIM2, ENABLE);
}

/*
    returns SR04 measurement in cm
		SR04_TRIG is connected to trigger pin
		SRO04_ECHO is connected echo pin 
 */
static float SR04read(void) {
  TIM_SetCounter(TIM2, 0);
	GPIOA->ODR |= 0x00000040; // sets trigger pin to 1
  while(TIM_GetCounter(TIM2) < 10);
	GPIOA->ODR &= ~0x00000040;// sets trigger pin to 0
  TIM_SetCounter(TIM2, 0);
  while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)); // waits till echo pin is false 
  TIM_SetCounter(TIM2, 0);													// reset timer
  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));  // waits till pin is true 
  return ((float)TIM_GetCounter(TIM2) * 0.01715 + SR04_OFFSET); // takes time it took to get an echo and multplies it by 0.01715 which is (1/(speed of sound *2)) for an accurate measurement
}

/*
  Configure SR04 GPIO 
 */
static void SR04Init(void) {
	// ENABLE CLOCK GPOIA
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	GPIOA->CRL=0;
	//ENABLE PIN 6 TRIGGER
	GPIOA->CRL|=(GPIO_CRL_MODE6_1|GPIO_CRL_MODE6_0);
	//ENABLE PIN 7 ECHO
	GPIOA->CRL|=GPIO_CRL_CNF7_0;

  // hold TRIG pin low
  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	
}

// reads sensor iCycle times takes average and checks if this is below input (1=True 0=False)
static uint8_t SR04check(uint8_t iDistance, uint8_t iCycles){
	//local variables
	uint8_t i = 0;
	double dSum =0;
	
	//function body
	// reads sensor
	for(i=0; i < iCycles; i++){
		dSum= dSum + (double)SR04read();
		delay_us(500);
	}
	//calculates average
	dSum= dSum/iCycles;

	if((int)dSum <= iDistance){
		return 1;
	}
	else{
		return 0;
	}
	
}

// registers toilet/sanitair use
static uint8_t SR04register(uint8_t iDistance, uint8_t iMode){
	//local variables
	uint8_t i =0;
	uint8_t iReturn =0;
	
	//function body
	if(iMode==1){ // mode 1 == register toilet use
		for(i=0;i<10;i++){
			if(!SR04check(iDistance+20, 10)){
				iReturn++;
				//when sr04 check is false more than 3 times it doesnt register the use
				if(iReturn >3){
					return 0;
				}
			}
		}
		
		while(SR04check(iDistance +20, 15)){//keeps doing nothing while sensor read is below idistance+20
		 //do nothing
		}	
		return 1;
	}
	else if(iMode==2){ // mode 2 == register sanitair use
		if(SR04check(iDistance ,4) ){
			while(SR04check(iDistance ,5)){ // while sensor read is < distance  wait till its not
			 delay_us(500);
			}
			return 1;
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}


//configure .h file with .c file
sensSR04 * new_sensSR04(){
		sensSR04 *setupSensSR04  = (sensSR04 *)malloc(sizeof(sensSR04));
		setupSensSR04->SR04_init = SR04Init;
		setupSensSR04->SR04_read = SR04read;
		setupSensSR04->SR04_check = SR04check;
		setupSensSR04->SR04_reg = SR04register;
		setupSensSR04->SR04_timinit = TIM2_Init;
		return setupSensSR04;
}


