/*
    BatteryMeasure.c
    Purpose: battery measure Logic
    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/

#include "stm32f10x.h"  
#include "batterymeasure.h"
#include <stdlib.h>
#include "stm32f10x_adc.h"
#include "delay.h"


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void  ADCInit(void);
int CalculatePercentage(uint16_t iADCValue, float iDefineFullBattery, float iDefineZeroBattery);

// ----------------------------------------------------------------------------
// Get percentage
// ----------------------------------------------------------------------------
int getPercentage(float iDefineFullBattery, float iDefineZeroBattery)
{
    GPIOA->ODR ^= 0x0002;
		GPIOA->CRL ^= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
		GPIOA->CRL ^= GPIO_CRL_CNF0_0;
    uint16_t iValueOfADC;
    uint8_t iPercentageOfBattery;
	
		//testing delay
		delay_ms(2000);
	
		
    // Get the conversion result
    iValueOfADC = ADC1->DR;

    iPercentageOfBattery = CalculatePercentage(iValueOfADC, iDefineFullBattery, iDefineZeroBattery);
		GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
    GPIOA->ODR |= 0x0002;
    return iPercentageOfBattery;
}

/**
  * @brief  This function sets PA0 to analog mode, initializes and
  *         calibrates ADC1.
  * @param  None
  * @retval None
  */
void ADCInit(void)
{
  
  //(#) Enable the ADC interface clock using 
  //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  //ADC_DeInit(ADC1);
	
	

	//enable GPIOA clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	//pin A0 on analog input'
	//first pin A0 has to be open drain output
	
	GPIOA->CRL ^= GPIO_CRL_CNF0_0;
	
	//pin A1 on Open drain output
	GPIOA->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);

	
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	ADC1->CR2 |= ADC_CR2_ADON;  // enable analog digital converter 
	ADC1->CR1 &= ~ADC_CR1_DUALMOD;	//independent mode
	ADC1->CR2 &= ~ADC_CR2_EXTSEL;		//external trig conv_none
	ADC1->CR2 &= ~ADC_CR2_EXTTRIG;	//external trig disabled
	ADC1->CR1 |= ADC_CR1_SCAN;			//scan conv mode
	ADC1->CR2 |= ADC_CR2_CONT ; // enable Continuous mode 
	ADC1->CR2 &= ~ADC_CR2_ALIGN; // allign 'right' 
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;   // select channel 1 for adc 1 
	ADC1->SQR1 |= ADC_SQR1_L;
	ADC1->SMPR2 |= ADC_SMPR2_SMP1_0 | ~ADC_SMPR2_SMP1_1 | ADC_SMPR2_SMP1_2;	//set sample time on 55.5 cycles 
	
	ADC1->CR2 |= ADC_CR2_RSTCAL; //reset calibration
	delay_ms(20);
	ADC1->CR2 |= ADC_CR2_CAL;		//SET calibration
	delay_ms(20);
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	//set pin A0 on open drain output
	GPIOA->CRL |= (GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);
	
}

// ----------------------------------------------------------------------------
// Calculate percentage of the battery
// ----------------------------------------------------------------------------
int CalculatePercentage(uint16_t iADCValue, float iDefineFullBattery, float iDefineZeroBattery){
	
	uint8_t iPercentageOfBattery = 100;
	int16_t iDifferenceFullZero;
	int16_t iDifferenceADCZero;
	
	//voltage division, because the ADC of stm can't handle more than 3V.
	iDefineFullBattery = iDefineFullBattery/2;
	iDefineZeroBattery = iDefineZeroBattery/2;
	
	//Calculate an ADC value of the voltage value.
	iDefineFullBattery = (iDefineFullBattery * 4096)/3;
	iDefineZeroBattery = (iDefineZeroBattery * 4096)/3;
	
	//iDifferenceFullZero = 100%, iDifferenceADCZero = current battery level
	iDifferenceFullZero = (uint16_t)(iDefineFullBattery - iDefineZeroBattery);
	iDifferenceADCZero = (uint16_t)(iADCValue - iDefineZeroBattery);
	
	if(iDifferenceADCZero <= 0){
		iDifferenceADCZero = 0;
	}
	
	//calculate percentage
	iPercentageOfBattery = (iDifferenceADCZero * (100/(float)iDifferenceFullZero));
	
	//Sometimes a battery delivers a bit more voltage as promised, so it could say it is 101%, this if statement sets it back to 100%
	if(iPercentageOfBattery >= 100){
		iPercentageOfBattery = 100;
	}
	
	return iPercentageOfBattery;
}



// ----------------------------------------------------------------------------
// BatteryMeasure constructor, with parameters: iDefineFullBattery(max voltage the battery delivers) and iDefineZeroBattery(min voltage the microcontrollers still operates).
// ----------------------------------------------------------------------------
SimpleBatteryMeasure *new_SimpleBatteryMeasure(float iDefineFullBattery, float iDefineZeroBattery){

		SimpleBatteryMeasure *batteryMeasurePercentage = (SimpleBatteryMeasure *)malloc(sizeof(SimpleBatteryMeasure));
	
		batteryMeasurePercentage->ADCInit = ADCInit;
		batteryMeasurePercentage->CalculatePercentage = CalculatePercentage;
		batteryMeasurePercentage->getPercentage = getPercentage;	  
		ADCInit();
		return batteryMeasurePercentage;
}
