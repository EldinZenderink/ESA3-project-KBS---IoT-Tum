/*
    memory allocating
		memman.c
		Purpose: libary for memory allocation
		
		@author: Eldin Zenderink, Willem Fikkert
		@version: 1.0
*/

//includes
#include "memman.h"

//global variables
static char bigassbuffer[MEMSIZE];

//get amount of free memory.
uint32_t MemMan_GetFreeMemory(){
		uint32_t start;
		uint32_t nullcount = 0;
		for(start = 0; start < MEMSIZE; start++){
			 char d = bigassbuffer[start];
			 if(d != NULL){
				 nullcount++;
			 }
		}
		return nullcount;
}

//get free block, you give the size of the data you want to store, and then it searches through the memory for a block which has at least so much free space that you can store your data in it.
uint32_t MemMan_GetFreeBlock(uint32_t size){
		uint32_t start= 0;
		uint32_t nullcount = 0;
		uint8_t state = 0;
	
		//run through memory
		for(start = 0; start < MEMSIZE; start++){
			 char d = bigassbuffer[start];
			 if(d == 0x00){
				 if(nullcount >= size){
					 break;
				 }
				 nullcount++;
			 } else {
				 nullcount = 0;		 
			 }
		}
		uint32_t fillstart;
		for(fillstart = (start - size + 1); fillstart < start + 1; fillstart++){
			bigassbuffer[fillstart] = 3;			 
		}
		return (uint32_t) &bigassbuffer[start - size + 1];
}

//free the block, give startaddress and length, then the data in the memory will be freed
void MemMan_FreeBlock(uint32_t startaddress, uint32_t length){
	uint32_t count;
	for(count = startaddress; count < (startaddress + length +1); count++){
			bigassbuffer[count] = NULL;		
	}	
}


