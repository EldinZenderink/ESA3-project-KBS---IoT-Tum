/*
*@author: Michiel van Dalfsen, Willem Fikkert, Eldin Zenderink
*@purpose: main program of the Sensor module
*/


#include "stm32f10x.h"                  
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
// Included files
// ----------------------------------------------------------------------------											
#include "SR04.h"
#include "delay.h"
#include "usart.h"
#include "ble.h"
#include "batterymeasure.h"


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
static void TIM3_Init(void);


// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
	volatile	int iTimCount =0;
	volatile uint8_t iSendBLE=0;
	
	
// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(void)
{
	//variables
	char cString2[50];//char array /string
	char cString1[50];
	uint8_t iDeviceFound;
	uint16_t iUses=0; //number of uses
  uint16_t iBatteryUsage = 0; //battery percentage
	uint8_t iMode =1; //mode 1 =  toilet register, mode 2 = sanitair register
	uint16_t iDistance = 140; //distance for sensor to register object
	
	//initate structs included libaries
	sensSR04 *sens = new_sensSR04();
	SimpleUSART *com = new_SimpleUSART(100,1);
	SimpleUSART *pcserial = new_SimpleUSART(100,2);
	SimpleBatteryMeasure *simpleBat = new_SimpleBatteryMeasure(4.5,3.7);
	
	//initate the included libarys
	delay_init();
	sens->SR04_init();
	sens->SR04_timinit();
	com->USART_init(com);
	pcserial->USART_init(pcserial);
	TIM3_Init();
	
	//initate bluetooth low energy
	BleCon *ble = new_BleCon('a',11,8,GPIOA,com,pcserial);
	if(ble->Ble_Init(ble)){			
		pcserial->USART_putstr("BLE INITIALIZED! \n", pcserial);
	} else{
		pcserial->USART_putstr("BLE NOT INITIALIZED :( \n", pcserial);
	}
	
	/* when you need to set the name of the bluetooth module
	if(ble->Ble_SetName("SensorX", ble)){			//groot breadbord = 1; klein breadbord = 0
		pcserial->USART_putstr("BLE NAME SUCCESFULLY SET! \n", pcserial);
	} else {
		pcserial->USART_putstr("BLE COULD NOT SET NAME :(  \n", pcserial);
	}
	*/
	//sets the bluetooth mode for the sensormodule
	if(ble->Ble_SetMode('1', ble)){			//groot breadbord = 1; klein breadbord = 0
		pcserial->USART_putstr("BLE MODE SUCCESFULLY SET! \n", pcserial);
	} else {
		pcserial->USART_putstr("BLE COULD NOT SET MODE :(  \n", pcserial);
	}

	delay_ms (1000);
	
	// checks mode of the bluetooth and connects to the master if needed
	if(ble->mode == '1' || ble->mode == '2'){
		while(1){
			iDeviceFound = 0;
			Devices *devices =	ble->Ble_ScanSlaves(ble);
			pcserial->USART_putstr("DEVICES FOUND: \n", pcserial);
			uint8_t counter = 0;
			while(1){
				
				Devices dev = devices[counter];
				if(dev.deviceid != NULL){
					pcserial->USART_putstr("\n DEVICE ID: \n", pcserial);
					pcserial->USART_putstr(dev.deviceid, pcserial);
					
					pcserial->USART_putstr("\n DEVICE MAC: \n", pcserial);
					pcserial->USART_putstr(dev.mac, pcserial);
					
					if(strstr(dev.deviceid, "TEST") != NULL){
						iDeviceFound = 1;
						ble->Ble_ConnectSlave(dev.mac, ble);
						pcserial->USART_putstr("\n  FOUND MASTER AND CONNECT TO IT! \n", pcserial);
						break;
					}
					
				} else {
					break;				
				}
				counter++;
			}	
				ble->Ble_FreeDevices(ble);
				if(iDeviceFound){
					break;
				}
		}
	}
	//disables ble (will be activated when needed)
	ble->Ble_OFF(ble);
	
	// prints current core clock of the device
	int	ret = snprintf(cString1, sizeof cString1, "{ coreclock :%i }\n",SystemCoreClock);
	pcserial->USART_putstr(cString1,pcserial);
	delay_ms(1000);
	
// ----------------------------------------------------------------------------
// MAIN loop
// ----------------------------------------------------------------------------											
  while(1)
  {
		
   //checks if there is something in the range of the sensor
		if( sens->SR04_reg(iDistance ,iMode) ){
			// waits till it dissapears before you add an use
			while(sens->SR04_reg(iDistance ,iMode)){
			 delay_ms(100);
			}		
		iUses = iUses+1;
		}	
		
		//debug tool to see the current status of the module
	  ret = snprintf(cString1, sizeof cString1, "{ uses :%i }\n",iUses);
		pcserial->USART_putstr(cString1,pcserial);
		ret = snprintf(cString1, sizeof cString1, "{ count :%i }\n",iTimCount);
		pcserial->USART_putstr(cString1,pcserial);
		
		// when 10 minutes passed and there is at least one use, send the data to the master
		if(iSendBLE && iUses>0 ){
			ret = snprintf(cString1, sizeof cString1, "{\"id\" : \"XY\", \"er\" : \"0\", \"md\" : \"%i\" \"pw\" : \"%i\", \"us\" : \"%i\"} \n",iMode,iBatteryUsage,iUses);
				ble->Ble_ON(ble);
			  delay_ms(2000);
				pcserial->USART_putstr(cString1,pcserial); // debug to check what the sensor sends
			  ble->Ble_Send(cString1,1,ble);             // transmit char array string1 via ble to master
				delay_ms(2000);
				ble->Ble_OFF(ble);
				iUses = 0;
				iTimCount=0;
				iSendBLE=0;
		}else if(iSendBLE && iUses == 0){ //when uses is 0 reset counter and the sendble bit
			iTimCount =0;
			iSendBLE = 0;
		}
		
		
		//sleep mode untill interupt
		__WFI();
  }
}


//interupt for waking from sleepmode 1x per 10s
void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		
		//waits 10 minutes to set send ble to 1
	if(iTimCount <60){
		iTimCount++;
	}
	else{
		iSendBLE =1;
		
	}
		
		
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}

}
// timer with a frequency of 0.1Hz
static void TIM3_Init(void) {
  uint16_t iPrescalerValue = 0;
  
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);

  /* Compute the prescaler value */
  iPrescalerValue = (uint16_t) (SystemCoreClock / 1000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 10000 - 1;    
  TIM_TimeBaseStructure.TIM_Prescaler = iPrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM3_IRQn);
  TIM_Cmd(TIM3, ENABLE);
}
