/*
    delay.h
    Purpose: Setting up delay with DWT timer (DEBUG WATCHDOG TIMER)?.
		Based of: http://embeddedsystemengineering.blogspot.nl/2016/01/arm-cortex-m3-stm32f103-tutorial-system.html
    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 17/12/17
*/
#include <stdint.h>
#include "stm32f10x.h" 

#ifndef _DELAY_H_
#define _DELAY_H_

void delay_init(void);
void delay_us(uint32_t microseconds);
void delay_ms(uint32_t milliseconds);
#endif // _DELAY_H_
