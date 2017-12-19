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
	uint16_t iValueOfADC;
	
	// Delay ~0.2 sec.
	delay_init();
	delay_ms(200);
	
	// Get the conversion result
	iValueOfADC = ADC_GetConversionValue(ADC1);
	
	return CalculatePercentage(iValueOfADC, iDefineFullBattery, iDefineZeroBattery);
}

/**
  * @brief  This function sets PC1 to analog mode, initializes and
  *         calibrates ADC1.
  * @param  None
  * @retval None
  */
void ADCInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  ADC_InitTypeDef  ADC_InitStructure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 
  //(#) Enable the ADC interface clock using 
  //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  ADC_DeInit(ADC1);
	
  //(#) ADC pins configuration
  //   (++) Enable the clock for the ADC GPIOs using the following function:
  //        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);   
  //   (++) Configure these ADC pins in analog mode using GPIO_Init();  
  RCC_AHBPeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  /* ADC1 regular channels configuration */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
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
