/*
    DELAY
		delay.c
		Purpose: Timed Delay in us or ms
		
		@author: Eldin Zenderink
		@version: 1.0
*/
#include "delay.h"

//global variables.
volatile uint32_t cyclesperus;
volatile uint32_t mstick;


//initialize the delay.
void delay_init(void){
	cyclesperus =  (SystemCoreClock/1000000L);
	
	if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) 
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
}

//wait for an certain amount of microseconds (number of microseconds is the parameter which is given).
void delay_us(uint32_t microseconds){
	int32_t tp = DWT->CYCCNT + microseconds * cyclesperus;
	while (((int32_t)DWT->CYCCNT - tp) < 0);
}

//wait for an certain amount of miliseconds (number of miliseconds is the parameter which is given).
void delay_ms(uint32_t milliseconds){
	uint16_t count = 0;
	while(count < milliseconds){
		int32_t tp = DWT->CYCCNT + (1000 * cyclesperus) ;
		while (((int32_t)DWT->CYCCNT - tp) < 0);			
		count++;
	}
}
