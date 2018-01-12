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
static char cBigassbuffer[MEMSIZE];

//get amount of free memory.
uint32_t MemMan_GetFreeMemory(){
		uint32_t iStart;
		uint32_t iNullcount = 0;
		for(iStart = 0; iStart < MEMSIZE; iStart++){
			 char d = cBigassbuffer[iStart];
			 if(d != NULL){
				 iNullcount++;
			 }
		}
		return iNullcount;
}

//get free block, you give the size of the data you want to store, and then it searches through the memory for a block which has at least so much free space that you can store your data in it.
uint32_t MemMan_GetFreeBlock(uint32_t size){
		uint32_t iStart= 0;
		uint32_t iNullcount = 0;

	
		//run through memory
		for(iStart = 0; iStart < MEMSIZE; iStart++){
			 char d = cBigassbuffer[iStart];
			 if(d == 0x00){
				 if(iNullcount >= size){
					 break;
				 }
				 iNullcount++;
			 } else {
				 iNullcount = 0;		 
			 }
		}
		uint32_t iFillstart;
		for(iFillstart = (iStart - size + 1); iFillstart < iStart + 1; iFillstart++){
			cBigassbuffer[iFillstart] = 3;			 
		}
		return (uint32_t) &cBigassbuffer[iStart - size + 1];
}

//free the block, give startaddress and length, then the data in the memory will be freed
void MemMan_FreeBlock(uint32_t startaddress, uint32_t length){
	uint32_t count;
	for(count = startaddress; count < (startaddress + length +1); count++){
			cBigassbuffer[count] = NULL;		
	}	
}


