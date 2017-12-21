/*
    memman.h
    Purpose: Managing memory (aka mem: memory, man: management) to hopefully prevent memory overflows.
    @author Eldin Zenderink
    @version 1.0 17/12/17
*/
#include <stdint.h>
#include <stdlib.h>
#include "stm32f10x.h" 

#define MEMSTART 0x20000000
#define MEMSIZE 0x2800


#ifndef _MEMMAN_H_
extern char bigassbuffer[MEMSIZE];

uint32_t MemMan_GetFreeMemory(void);
uint32_t MemMan_GetFreeBlock(uint32_t size);
void MemMan_FreeBlock(uint32_t startaddress, uint32_t length);

#define _MEMMAN_H_
#endif //_MEMMAN_H_