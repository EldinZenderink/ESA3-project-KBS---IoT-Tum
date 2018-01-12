#pragma once
/*
    usart.h
    Purpose: Setting up the USART library

    @author(STM32F103): Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @port(ESP8266): Eldin Zenderink
    @version 1.0 12/12/17
*/


#ifndef _USART_H_
#define _USART_H_

/******************************************************************************
  Includes
******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

/******************************************************************************
  Structs
******************************************************************************/
typedef struct SimpleUSART{
	//methods
	void(*USART_init)(struct SimpleUSART *);
	void(*USART_putc)(char, struct SimpleUSART *);
	char(*USART_getc)(struct SimpleUSART *);
	void(*USART_putstr)(char*, struct SimpleUSART *);
	char* (*USART_getstr)(char*, uint32_t iTimeOut, struct SimpleUSART *);
}  SimpleUSART;



/******************************************************************************
  Function prototypes
******************************************************************************/
SimpleUSART * new_SimpleUSART();
#endif // _USART_H_
