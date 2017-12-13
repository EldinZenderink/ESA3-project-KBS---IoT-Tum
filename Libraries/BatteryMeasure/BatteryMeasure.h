/*
    BatteryMeasure.h
    Purpose: Setting up the Battery measure library
    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 13/12/17
*/


#ifndef _BATTERYMEASURE_H_
#define _BATTERYMEASURE_H_

#include "stdint.h"

/******************************************************************************
  Defines
******************************************************************************/


/******************************************************************************
  Structs
******************************************************************************/
typedef struct BatteryMeasure{
	//attributes
	volatile int usartperiphid;
	
	//methods
	void(*ADCInit)(void);
	int(*CalculatePercentage)(uint16_t, float, float);
	int(*getPercentage)(float, float, struct BatteryMeasure *);
	void(*Delay)(const int);
}  BatteryMeasure;



/******************************************************************************
  Function prototypes
******************************************************************************/
BatteryMeasure * new_BatteryMeasure(float iDefineFullBattery, float iDefineZeroBattery);
#endif // _BATTERYMEASURE_