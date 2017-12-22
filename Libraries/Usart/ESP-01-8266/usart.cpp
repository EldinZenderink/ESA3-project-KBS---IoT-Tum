
/*
    usart.c
    Purpose: USART Logic

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/


#include <Arduino.h>
#include "usart.h"
#include <stdint.h>
#include <stdlib.h>
// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------

void USARTinit(SimpleUSART *susart)
{	
  Serial.begin(115200);
  Serial.setTimeout(1000);
  Serial.println("STARTING USART ON ESP!\n");
}

/*
* Send character to USARTx dependig on which peripheral is selected
*/
void USARTputc(char c, SimpleUSART *susart)
{	
	  Serial.print(c);
}


/*
* Send string to USARTx using USARTputc
*/
void USARTputstr(char *str, SimpleUSART *susart)
{
  Serial.print(str);
}

/*
* Get character from fifobuffer depending on which peripheral is selected
*/
char USARTgetc(SimpleUSART *susart)
{
  if (Serial.available()){
    return Serial.read();
  } else {
    return '\0';
  }
}

/*
* Get string using USARTgetc
*/
char* USARTgetstr(char * buffer, uint32_t timeout, SimpleUSART *susart){  

    uint32_t i = 0;
    while(!Serial.available()){
      
        if( i > timeout){
            buffer[0] = '\0';
            return buffer;
        }
        delay(1);
        i++;
    }
    if (Serial.available()) {
      String str = Serial.readString();
      str.toCharArray(buffer, str.length());
      return buffer;
    } else {
      buffer[0] = '\0';
      return buffer;
    }
}

/*
* USART Constructor, wiht parameters: iBufsize (fifo buffersize for both usart peripherals), iType (which USART peripheral needs to be used)
*/
SimpleUSART *new_SimpleUSART(){
	
		SimpleUSART *susart = (SimpleUSART *)malloc(sizeof(SimpleUSART));
		susart->USART_getc = USARTgetc;
		susart->USART_getstr = USARTgetstr;
		susart->USART_putc = USARTputc;
	  susart->USART_putstr = USARTputstr;
		susart->USART_init = USARTinit;
	
		return susart;
}

