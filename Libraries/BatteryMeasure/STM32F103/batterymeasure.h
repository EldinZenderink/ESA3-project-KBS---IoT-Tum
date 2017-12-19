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
  Structs
******************************************************************************/
typedef struct SimpleBatteryMeasure{
	
	//methods
	void(*ADCInit)(void);
	int(*CalculatePercentage)(uint16_t, float, float);
	int(*getPercentage)(float, float);
}  SimpleBatteryMeasure;

/******************************************************************************
  Function prototypes
******************************************************************************/
SimpleBatteryMeasure * new_SimpleBatteryMeasure(float iDefineFullBattery, float iDefineZeroBattery);
#endif // _BATTERYMEASURE_H_
