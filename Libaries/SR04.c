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
#include "stm32f0xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"
#include "SR04.h"
// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
#define SR04_TRIG   (1<<6) //PIN PC 6
#define SR04_ECHO   (1<<7) //PIN PC 7
#define SR04_OFFSET 0.2

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
void TIM2_Init(void);

/*
  Function that initializes timer 2
*/
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
  GPIO_SetBits(GPIOC, SR04_TRIG);
  while(TIM_GetCounter(TIM2) < 8);
  GPIO_ResetBits(GPIOC, SR04_TRIG);
  //TIM_SetCounter(TIM2, 0);
  while(!GPIO_ReadInputDataBit(GPIOC, SR04_ECHO));// & (TIM_GetCounter(TIM2) < 50000));
  TIM_SetCounter(TIM2, 0);
  while(GPIO_ReadInputDataBit(GPIOC, SR04_ECHO));// & (TIM_GetCounter(TIM2) < 50000));
  return ((float)TIM_GetCounter(TIM2) * 0.01715 + SR04_OFFSET);
}

/*
  Configure SR04 GPIO & TIM2
 */
static void SR04Init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // configuring clock sources for GPIOC
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  /* Configure SR04 trigger, in this case uses PC6 - TRIGGER */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Configure SR04 echo, in this case uses PC7 - ECHO */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // hold TRIG pin low
  GPIO_ResetBits(GPIOC, SR04_TRIG);
	
	//also initates the timer needed for the SR04
	TIM2_Init();
}

/**
  * @brief  Main program. will be gone in libary
*/

sensSR04 * new_sensSR04(){
		sensSR04 *setupSensSR04  = (sensSR04 *)malloc(sizeof(sensSR04));
		setupSensSR04->SR04_init = SR04Init;
		setupSensSR04->SR04_read = SR04read;
	
		return setupSensSR04;
}


