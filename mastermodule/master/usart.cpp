
/*
    usart.c
    Purpose: USART Logic
    @author(STM32F103): Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @port(ESP8266): Eldin Zenderink
    @version 1.0 12/20/17
*/

/******************************************************************************
  Includes
******************************************************************************/
#include "usart.h"

/******************************************************************************
  Local Function Prototypes
******************************************************************************/
static void USARTinit(SimpleUSART *susart)
{	
  Serial.begin(115200);
  Serial.setTimeout(1000);
}

/*
 * Function : Send character to serial device
 * Parameter: char, SimpleUSART pointer, char c = character to send, SimpleUSART *susart = usart data
 */
static void USARTputc(char cChar, SimpleUSART *susart)
{	
	  Serial.print(cChar);
}


/*
 * Function : Send character to serial device
 * Parameter: char, SimpleUSART pointer, char c = character to send, SimpleUSART *susart = usart data
 */
static void USARTputstr(char *cString, SimpleUSART *susart)
{
  Serial.print(cString);
}

/*
 * Function : Get character from serial device
 * Parameter: SimpleUSART pointer, char c = character to send, SimpleUSART *susart = usart data
 * Returns  : character as char
 */
static char USARTgetc(SimpleUSART *susart)
{
  if (Serial.available()){
    return Serial.read();
  } else {
    return '\0';
  }
}

/*
 * Function : Get characters from serial device
 * Parameter: char pointer, uint32_t, SimpleUSART pointer, char* cBuffer = cBuffer to append characters to,uint32_t iTimeOut = iTimeOut before returning cBuffer, SimpleUSART *susart = usart data
 * Returns  : sReceiveding as char*
 */
static char* USARTgetstr(char * cBuffer, uint32_t iTimeOut, SimpleUSART *susart){  

    uint32_t iCount = 0;
    while(!Serial.available()){
      
        if( iCount > iTimeOut){
            cBuffer[0] = '\0';
            return cBuffer;
        }
        delay(1);
        iCount++;
    }
    if (Serial.available()) {
      String sReceived = Serial.readString();
      sReceived.toCharArray(cBuffer, sReceived.length() + 1);
      cBuffer[sReceived.length() + 1] = '\0';
      return cBuffer;
    } else {
      cBuffer[0] = '\0';
      return cBuffer;
    }
}

/*
 * Function : ConsReceiveduct SimpleUSART
 * Returns  : SimpleUSART as SimpleUSART*
 */
SimpleUSART *new_SimpleUSART(){
	
		SimpleUSART *susart = (SimpleUSART *)malloc(sizeof(SimpleUSART));
    if(susart == NULL){
      return NULL;
    }
		susart->USART_getc = USARTgetc;
		susart->USART_getstr = USARTgetstr;
		susart->USART_putc = USARTputc;
	  susart->USART_putstr = USARTputstr;
		susart->USART_init = USARTinit;
	
		return susart;
}

