
/*
    usart.h
    Purpose: Setting up the USART library

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/


#ifndef _USART_H_
#define _USART_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

/******************************************************************************
  Defines
******************************************************************************/


/******************************************************************************
  Structs
******************************************************************************/
typedef struct SimpleUSART{
	//attributes
	
	//methods
	void(*USART_init)(struct SimpleUSART *);
	void(*USART_putc)(char, struct SimpleUSART *);
	char(*USART_getc)(struct SimpleUSART *);
	void(*USART_putstr)(char*, struct SimpleUSART *);
	char* (*USART_getstr)(char*, uint32_t timeout, struct SimpleUSART *);
}  SimpleUSART;



/******************************************************************************
  Function prototypes
******************************************************************************/
SimpleUSART * new_SimpleUSART();
#endif // _USART_H_
