/*
    BatteryMeasure.h
    Purpose: Setting up the Battery measure library
    @author Willem Fikkert
    @version 1.0 
*/

#pragma once
#ifndef _BATTERYMEASURE_H_
#define _BATTERYMEASURE_H_

#include "stdint.h"

#include "stm32f10x.h"

/******************************************************************************
  Structs
******************************************************************************/
typedef struct SimpleBatteryMeasure{
	int iPrevValueOfADC;
	//methods
	void(*ADCInit)(void);
	int(*CalculatePercentage)(uint16_t, float, float);
	int(*getPercentage)(float, float, struct SimpleBatteryMeasure *);
}  SimpleBatteryMeasure;

/******************************************************************************
  Function prototypes
******************************************************************************/
SimpleBatteryMeasure * new_SimpleBatteryMeasure(float iDefineFullBattery, float iDefineZeroBattery);
#endif // _BATTERYMEASURE_H_
