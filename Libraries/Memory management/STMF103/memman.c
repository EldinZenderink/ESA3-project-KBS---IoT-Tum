#include "memman.h"
static char bigassbuffer[MEMSIZE];
static uint32_t lastmemwrite = 0;
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
		//lastmemwrite =  start - size + 1;
		return (uint32_t) &bigassbuffer[start - size + 1];
}

void MemMan_FreeBlock(uint32_t startaddress, uint32_t length){
	uint32_t count;
	for(count = startaddress; count < (startaddress + length +1); count++){
			bigassbuffer[count] = NULL;		
	}	
}

