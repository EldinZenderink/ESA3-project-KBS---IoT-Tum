/*
    usart.c
    Purpose: USART Logic

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/

#include "stm32f10x.h" 
#include "usart.h"
#include "delay.h"
#include "memman.h"
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------


volatile char cRxBuffer1;
volatile char *cFifoBuffer1;
volatile int iI1, iAmountOfCharInBuffer1  = 0;
volatile int iSizeOfBuffer1 = 100;


volatile char cRxBuffer2;
volatile char *cFifoBuffer2;
volatile int iI2, iAmountOfCharInBuffer2 = 0;
volatile int iSizeOfBuffer2 = 100;


// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------

static void USARTinit(SimpleUSART *susart)
{	
	switch(susart->usartperiphid){
		case 1:
				//allocate memory for fifobuffer usart1
	      //cFifoBuffer1 = (char *)malloc(iSizeOfBuffer1 * sizeof *cFifoBuffer1);
				cFifoBuffer1 = (char*)MemMan_GetFreeBlock(susart->bufsize);
		
			  // GPIOA Periph clock enable		
			  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
				
				//enable PA9 as TX with Output Alternated Function using PUSHPULL
				GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1);
				//enable PA10 as RX as floating input
				GPIOA->CRH |= (GPIO_CRH_CNF9_0);
		
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

				// 115200 Bd @ 72 MHz
				// USARTDIV = 72 MHz / 115200 = 625 = 0x0271
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART1->BRR = 0x0271;

				// USART enable
				// Receiver enable
				// Transmitter enable
				USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

				// Default value
				USART1->CR2 = 0;
				USART1->CR3 = 0; 
				// RXNE interrupt enable
				USART1->CR1 |= USART_CR1_RXNEIE;
				
				// USART1 interrupts enable in NVIC
				NVIC_EnableIRQ(USART1_IRQn);
				NVIC_SetPriority(USART1_IRQn, 0);
				NVIC_ClearPendingIRQ(USART1_IRQn);
				break;
		case 2: 
				//allocate memory for fifo buffer usart2
			  //cFifoBuffer2 = (char *)malloc(iSizeOfBuffer2 * sizeof *cFifoBuffer2);
				cFifoBuffer2 = (char*)MemMan_GetFreeBlock(susart->bufsize);
				
		  // GPIOA Periph clock enable		
			  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
				
				
				//enable PA2 as TX with Output Alternated Function using PUSHPULL
				GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0 | GPIO_CRL_CNF2_1);
				//enable PA3 as RX as floating input
				GPIOA->CRL |= (GPIO_CRL_CNF3_0);
				
				// USART2 clock enable
				RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 

				// 115200 Bd @ 36 MHz
				// USARTDIV = 36 MHz / 115200 = 312 = 0x0138
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART2->BRR = 0x0138;

				// USART enable
				// Receiver enable
				// Transmitter enable
				USART2->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE;
				// Default value
				USART2->CR2 = 0;
				USART2->CR3 = 0; 
				
				// USART1 interrupts enable in NVIC
				NVIC_EnableIRQ(USART2_IRQn);
				NVIC_SetPriority(USART2_IRQn, 0);
				NVIC_ClearPendingIRQ(USART2_IRQn);
				break;
		default:   			
				//allocate memory for fifobuffer usart1
				//cFifoBuffer1 = (char *)malloc(iSizeOfBuffer1 * sizeof *cFifoBuffer1);
				cFifoBuffer1 = (char*)MemMan_GetFreeBlock(susart->bufsize);
			  // GPIOA Periph clock enable		
			  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
				
				//enable PA9 as TX with Output Alternated Function using PUSHPULL
				GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1);
				//enable PA10 as RX as floating input
				GPIOA->CRH |= (GPIO_CRH_CNF9_0);
				
				// USART1 clock enable
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

					// 115200 Bd @ 72 MHz
				// USARTDIV = 72 MHz / 115200 = 625 = 0x0271
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART1->BRR = 0x0271;

				// USART enable
				// Receiver enable
				// Transmitter enable
				USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

				// Default value
				USART1->CR2 = 0;
				USART1->CR3 = 0; 
				// RXNE interrupt enable
				USART1->CR1 |= USART_CR1_RXNEIE;
				 
				// USART1 interrupts enable in NVIC
				NVIC_EnableIRQ(USART1_IRQn);
				NVIC_SetPriority(USART1_IRQn, 0);
				NVIC_ClearPendingIRQ(USART1_IRQn);							
				break;
	}
}


/*
* Get data from USART1 RDR register, append to fifo buffer
*/
void USART1_IRQHandler(void)
{ 

   if(USART1->SR & USART_SR_RXNE)
   {
			if(iAmountOfCharInBuffer1 < iSizeOfBuffer1) {
				cFifoBuffer1[iAmountOfCharInBuffer1] = USART1->DR;
				iAmountOfCharInBuffer1++;
			}	else {
				iAmountOfCharInBuffer1 = 0;
			}
   }	 
}

/*
* Get data from USART2 RDR register, append to fifo buffer
*/
void USART2_IRQHandler(void)
{ 
   // Read Data Register not empty interrupt?
   if(USART2->SR & USART_SR_RXNE)
   {
		 
		 
			cRxBuffer2 = USART2->DR;
		 
			//als het aantal characters in de buffer kleiner is dan 100 wordt deze char toegevoegd
			if(iAmountOfCharInBuffer2 < iSizeOfBuffer2) {
				cFifoBuffer2[iAmountOfCharInBuffer2] = cRxBuffer2;
				iAmountOfCharInBuffer2++;
			}	else {
				iAmountOfCharInBuffer2 = 0;
			}
   }	 
}

/*
* Send character to USARTx dependig on which peripheral is selected
*/
static void USARTputc(char c, SimpleUSART *susart)
{	
	switch(susart->usartperiphid){
		case 1:
			while((USART1->SR & USART_SR_TXE) == 0) ;
			USART1->DR = c;
			break;
		case 2:
			while((USART2->SR & USART_SR_TXE) == 0) ;
			USART2->DR = c;
			break;
		default:  
			while((USART1->SR & USART_SR_TXE) == 0) ;
			USART1->DR = c;
			break;
	}
 
}


/*
* Send string to USARTx using USARTputc
*/
static void USARTputstr(char *str, SimpleUSART *susart)
{
  while(*str)
  {
    if(*str == '\n')
    {
      susart->USART_putc('\r', susart);
    }    
    susart->USART_putc(*str++, susart);
  }
}

/*
* Get character from fifobuffer depending on which peripheral is selected
*/
static char USARTgetc(SimpleUSART *susart)
{
  char c = '\0';
		switch(susart->usartperiphid){
			case 1:
					if(iAmountOfCharInBuffer1 > 0 ){
						c = (char) cFifoBuffer1[0];
						for(iI1 = 0; iI1<iAmountOfCharInBuffer1 - 1; iI1++){
								cFifoBuffer1[iI1] = cFifoBuffer1[iI1+1];
						}
						iAmountOfCharInBuffer1--;
					}
				break;
			case 2:
					if(iAmountOfCharInBuffer2 > 0 ){
						c = (char) cFifoBuffer2[0];
						for(iI2 = 0; iI2<iAmountOfCharInBuffer2 - 1; iI2++){
								cFifoBuffer2[iI2] = cFifoBuffer2[iI2+1];
						}
						iAmountOfCharInBuffer2--;
					}
				break;
		}		
	return(c);  
}

/*
* Get string using USARTgetc
*/
static char* USARTgetstr(char * buffer, uint32_t timeout, SimpleUSART *susart){	
		uint32_t i = 0;
		uint32_t b = 0;
		char r;
		int maxiterations = susart->bufsize;	
		while((r = susart->USART_getc(susart)) != '\n' && i < maxiterations){	
			 if(r != NULL){				 
				 buffer[i] = r;
				 i++;
			 } 
			 
			 if(b > timeout){
				 break;
			 }
			 b++;
			 delay_ms(1);
			 
		}	
		buffer[i] = '\0';
		return buffer;
}

/*
* USART Constructor, wiht parameters: iBufsize (fifo buffersize for both usart peripherals), iType (which USART peripheral needs to be used)
*/
SimpleUSART *new_SimpleUSART(int iBufsize, int iType){
		delay_init();
		SimpleUSART *susart = (SimpleUSART *)malloc(sizeof(SimpleUSART));
		iSizeOfBuffer1 = iBufsize;
		iSizeOfBuffer2 = iBufsize;
		susart->usartperiphid = iType;
		susart->USART_getc = USARTgetc;
		susart->USART_getstr = USARTgetstr;
		susart->USART_putc = USARTputc;
	  susart->USART_putstr = USARTputstr;
		susart->USART_init = USARTinit;
		susart->bufsize = iBufsize;
	
		return susart;
}

