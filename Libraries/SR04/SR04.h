
/*
    SR04.h
    Purpose: Setting up the HC-SR04 library

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 
*/

#pragma once
#ifndef _SR04_H_
#define _SR04_H_

#include "stdint.h"

/******************************************************************************
  Defines
******************************************************************************/


/******************************************************************************
  Structs
******************************************************************************/
typedef struct sensSR04{
	//attributes
	
	//methods
	void(*SR04_init)(void);
	void(*SR04_timinit)(void);
	float(*SR04_read)(void);
	uint8_t(*SR04_check)(uint8_t, uint8_t);
	uint8_t(*SR04_reg)(uint8_t, uint8_t);
}  sensSR04;



/******************************************************************************
  Function prototypes
******************************************************************************/
sensSR04 * new_sensSR04(void);
#endif // _SR04_H_
