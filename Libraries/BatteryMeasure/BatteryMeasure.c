/*
    BatteryMeasure.c
    Purpose: battery measure Logic
    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/

#include "stm32f0xx.h"
#include "BatteryMeasure.h"
#include <stdlib.h>

// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void  ADCInit(void);
int CalculatePercentage(uint16_t iADCValue, float iDefineFullBattery, float iDefineZeroBattery);
void DelayBM(const int d);
// ----------------------------------------------------------------------------
// Get percentage
// ----------------------------------------------------------------------------
int getPercentage(float iDefineFullBattery, float iDefineZeroBattery)
{
	uint16_t iValueOfADC;
	
	//initialize ADC.	
  ADCInit();
  
  // Configure channel 11 GPIOC I/O-pin 1
  ADC_ChannelConfig(ADC1, ADC_Channel_11 , ADC_SampleTime_239_5Cycles);
  
  // Start the first conversion
  ADC_StartOfConversion(ADC1);

	// Delay ~0.2 sec.
	DelayBM(SystemCoreClock/8/5);
	
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
 
  //(#) Enable the ADC interface clock using 
  //    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  //(#) ADC pins configuration
  //   (++) Enable the clock for the ADC GPIOs using the following function:
  //        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);   
  //   (++) Configure these ADC pins in analog mode using GPIO_Init();  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //(#) Configure the ADC conversion resolution, data alignment, external
  //    trigger and edge, scan direction and Enable/Disable the continuous mode
  //    using the ADC_Init() function.
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;    
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  // Calibrate ADC before enabling
  ADC_GetCalibrationFactor(ADC1);

  //(#) Activate the ADC peripheral using ADC_Cmd() function.
  ADC_Cmd(ADC1, ENABLE);  
  
  // Wait until ADC enabled
  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET); 
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
// Simple delay function
// ----------------------------------------------------------------------------
void DelayBM(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}

// ----------------------------------------------------------------------------
// BatteryMeasure constructor, with parameters: iDefineFullBattery(max voltage the battery delivers) and iDefineZeroBattery(min voltage the microcontrollers still operates).
// ----------------------------------------------------------------------------
SimpleBatteryMeasure *new_SimpleBatteryMeasure(float iDefineFullBattery, float iDefineZeroBattery){
	
		SimpleBatteryMeasure *batteryMeasurePercentage = (SimpleBatteryMeasure *)malloc(sizeof(SimpleBatteryMeasure));
		batteryMeasurePercentage->ADCInit = ADCInit;
		batteryMeasurePercentage->CalculatePercentage = CalculatePercentage;
		batteryMeasurePercentage->getPercentage = getPercentage;	  
	
		return batteryMeasurePercentage;
}
