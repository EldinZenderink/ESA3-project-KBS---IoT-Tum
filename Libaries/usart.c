/*
    usart.c
    Purpose: USART Logic

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/

#include "stm32f0xx.h"
#include "usart.h"
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
	      cFifoBuffer1 = (char *)malloc(iSizeOfBuffer1 * sizeof *cFifoBuffer1);
		
			  // GPIOA Periph clock enable		
				RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
				
				// PA9 and PA10 Alternate function mode
				GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);
				
				// Set alternate functions AF1 for PA9 and PA10
				GPIOA->AFR[1] |= 0x00000110;
				
				// USART1 clock enable
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

				// 115200 Bd @ 48 MHz
				// USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART1->BRR = 0x01A0;

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
			  cFifoBuffer2 = (char *)malloc(iSizeOfBuffer2 * sizeof *cFifoBuffer2);
		
				 // GPIOA Periph clock enable		
				RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
				
				// PA2 and PA3 Alternate function mode
				GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
				
				// Set alternate functions AF1 for PA2 and PA3     (is this correct??)
				GPIOA->AFR[0] |= 0x00001100;
				
				// USART2 clock enable
				RCC->APB1ENR |= RCC_APB1ENR_USART2EN; 

				// 115200 Bd @ 48 MHz
				// USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART2->BRR = 0x01A0;

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
				cFifoBuffer1 = (char *)malloc(iSizeOfBuffer1 * sizeof *cFifoBuffer1);
			  // GPIOA Periph clock enable
				RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
				
				// PA9 and PA10 Alternate function mode
				GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);
				
				// Set alternate functions AF1 for PA9 and PA10
				GPIOA->AFR[1] |= 0x00000110;
				
				// USART1 clock enable
				RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 

				// 115200 Bd @ 48 MHz
				// USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
				// BRR[15:4] = USARTDIV[15:4]
				// When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
				USART1->BRR = 0x01A0;

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
   if(USART1->ISR & USART_ISR_RXNE)
   {
			if(iAmountOfCharInBuffer1 < iSizeOfBuffer1) {
				cFifoBuffer1[iAmountOfCharInBuffer1] = USART1->RDR;
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
   if(USART2->ISR & USART_ISR_RXNE)
   {
		 
		 
			cRxBuffer2 = USART2->RDR;
		 
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
			while((USART1->ISR & USART_ISR_TXE) == 0) ;
			USART1->TDR = c;
			break;
		case 2:
			while((USART2->ISR & USART_ISR_TXE) == 0) ;
			USART2->TDR = c;
			break;
		default:  
			while((USART1->ISR & USART_ISR_TXE) == 0) ;
			USART1->TDR = c;
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
static char* USARTgetstr(char * buffer, SimpleUSART *susart){	
		int i = 0;
//		int timeout = 0;
		char r;
		int maxiterations = iSizeOfBuffer1;
		switch(susart->usartperiphid){
			case 1:
				maxiterations = iSizeOfBuffer1;
				break;
			case 2:
				maxiterations = iSizeOfBuffer2;
				break;
			default:
				maxiterations = iSizeOfBuffer1;
				break;
		}
		while((r = susart->USART_getc(susart)) != '\0' && i < maxiterations){			
			 buffer[i] = r;
			 i++;
		}
		if(i == 0){
			buffer = "EMPTY";
		} else {			
			buffer[i] = '\0';
		}
		return buffer;
}

/*
* Send clear screen command to USART peripheral depending on which one is selected
*/
static void USARTclearscreen(SimpleUSART *susart)
{
		char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
		char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
		
		susart->USART_putstr(cmd1, susart);
		susart->USART_putstr(cmd2, susart);
}

/*
* USART Constructor, wiht parameters: iBufsize (fifo buffersize for both usart peripherals), iType (which USART peripheral needs to be used)
*/
SimpleUSART *new_SimpleUSART(int iBufsize, int iType){
	
		SimpleUSART *susart = (SimpleUSART *)malloc(sizeof(SimpleUSART));
		iSizeOfBuffer1 = iBufsize;
		iSizeOfBuffer2 = iBufsize;
		susart->usartperiphid = iType;
		susart->USART_clearscreen = USARTclearscreen;
		susart->USART_getc = USARTgetc;
		susart->USART_getstr = USARTgetstr;
		susart->USART_putc = USARTputc;
	  susart->USART_putstr = USARTputstr;
		susart->USART_init = USARTinit;
	
		return susart;
}

