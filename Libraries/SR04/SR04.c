/*
    HC-SR04
		SR04.c
		Purpose: making an interface for HC-SR04 sonar sensor
		
		@author: Michiel van Dalfsen
		@version: 1.0
		
		@copyright mention for SR04 read: Fabio Angeletti 
*/

// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SR04.h"
#include "delay.h"
// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
#define SR04_TRIG   (1<<6) //PIN PA 6
#define SR04_ECHO   (1<<7) //PIN PA 7
#define SR04_OFFSET -2.0

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Local function prototypes
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
  GPIO_SetBits(GPIOA, GPIO_Pin_6);
  while(TIM_GetCounter(TIM2) < 10);
  GPIO_ResetBits(GPIOA, GPIO_Pin_6);
  TIM_SetCounter(TIM2, 0);
  while(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));// & (TIM_GetCounter(TIM2) < 50000));
  TIM_SetCounter(TIM2, 0);
  while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7));// & (TIM_GetCounter(TIM2) < 50000));
  return ((float)TIM_GetCounter(TIM2) * 0.01715 + SR04_OFFSET);
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
static uint16_t SR04check(uint8_t iDistance, uint8_t iCycles){
	//local variables
	uint8_t i = 0;
	double dSum =0;
	
	//function body
	
	// reads sensor
	for(i=0; i < iCycles; i++){
		dSum= dSum + (double)SR04read();
		delay_ms(50);
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

// registers toilet use
static uint8_t SR04register(uint8_t iDistance){
	//local variables
	uint8_t i =0;
	uint8_t iReturn =0;
	
	//function body
	for(i=0;i<10;i++){
		if(!SR04check(iDistance+20, 10)){
			iReturn++;
			//when sr04 check is false more than 3 times it doesnt register the use
			if(iReturn >3){
				return 0;
			}
		}
	}
	//keeps doing nothing while check is idistance+20
	while(SR04check(iDistance +20, 15)){
	 //do nothing
	}
		
	return 1;
	
}


/**
  * @brief  Main program. will be gone in libary
*/

sensSR04 * new_sensSR04(){
		sensSR04 *setupSensSR04  = (sensSR04 *)malloc(sizeof(sensSR04));
		setupSensSR04->SR04_init = SR04Init;
		setupSensSR04->SR04_read = SR04read;
		setupSensSR04->SR04_check = SR04check;
		setupSensSR04->SR04_reg = SR04register;
		setupSensSR04->SR04_timinit = TIM2_Init;
		return setupSensSR04;
}


