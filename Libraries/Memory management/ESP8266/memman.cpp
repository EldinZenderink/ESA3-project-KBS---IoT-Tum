/*
    memman.h
    Purpose: Managing memory (aka mem: memory, man: management) due to libraries made for stm32f103 needing this library to do memory management without issues, porting there code requires 
    this library  for the ble library though it's functionality is not necessary for the working of the esp.
    @author(stm32f103): Eldin Zenderink, Willem Fikkert
    @port(ESP8266): Eldin Zenderink
    @version 1.0 17/12/17
*/

/******************************************************************************
  Includes
******************************************************************************/
#include "memman.h"


/******************************************************************************
  Global Variables
******************************************************************************/
static char cBigAssBuffer[MEMSIZE];


/******************************************************************************
  Local Function Prototypes
******************************************************************************/
/*
 * Function : retreives the available memory in cBigAssBuffer, similar to GetFreeHeap on ESP
 * Returns  : uint32_t with amount free memory as value
 */
uint32_t MemMan_GetFreeMemory(){
		uint32_t iStart;
		uint32_t iNullCount = 0;
		for(iStart = 0; iStart < MEMSIZE; iStart++){
			 char cChar = cBigAssBuffer[iStart];
			 if(cChar != NULL){
				 iNullCount++;
			 }
		}
		return iNullCount;
}


/*
 * Function : gets the first available location where a block of memory is not in use in cBigAssBuffer and fills the allocated block with an identifier character (ETX)
 * Parameter: uint32_t, uint32_t iSize = iSize of memory block
 * Returns  : uint32_t with address to first index of memory within cBigAssBuffer where its certain that that memory is free until the end of the block
 */
uint32_t MemMan_GetFreeBlock(uint32_t iSize){
		uint32_t iStart= 0;
		uint32_t iNullCount = 0;
	
		//run through memory
		for(iStart = 0; iStart < MEMSIZE; iStart++){
			 char cChar = cBigAssBuffer[iStart];
			 if(cChar == 0x00){
				 if(iNullCount >= iSize){
					 break;
				 }
				 iNullCount++;
			 } else {
				 iNullCount = 0;		 
			 }
		}
		
		uint32_t iFillStart;
		for(iFillStart = (iStart - iSize + 1); iFillStart < iStart + 1; iFillStart++){
			cBigAssBuffer[iFillStart] = 3;			 
		}
		return (uint32_t) &cBigAssBuffer[iStart - iSize + 1];
}

/*
 * Function : frees the memory for iStarting at iStartAddress until iLength+iStartAddress
 * Parameter: uint32_t,uint32_t, uint32_t iStartAddress = iStartAddress if memoryblock, uint32_t iLength = iSize of memory block
 */
void MemMan_FreeBlock(uint32_t iStartAddress, uint32_t iLength){
	uint32_t iCount;
	for(iCount = iStartAddress; iCount < (iStartAddress + iLength +1); iCount++){
			cBigAssBuffer[iCount] = NULL;		
	}	
}

