#pragma once
/*
    memman.h
    Purpose: Managing memory (aka mem: memory, man: management) due to libraries made for stm32f103 needing this library to do memory management without issues, porting there code requires 
    this library  for the ble library though it's functionality is not necessary for the working of the esp.
    @author: Eldin Zenderink, Willem Fikkert
    @port(ESP8266): Eldin Zenderink
    @version 1.0 17/12/17
*/



#ifndef _MEMMAN_H_
/******************************************************************************
  Includes
******************************************************************************/
#include <stdint.h>
#include <stdlib.h>


/******************************************************************************
  Defines
******************************************************************************/
#define MEMSIZE 0x1400

/******************************************************************************
  Function prototypes
******************************************************************************/
uint32_t MemMan_GetFreeMemory(void);
uint32_t MemMan_GetFreeBlock(uint32_t iSize);
void MemMan_FreeBlock(uint32_t iStartAddress, uint32_t iLength);

#define _MEMMAN_H_
#endif //_MEMMAN_H_
