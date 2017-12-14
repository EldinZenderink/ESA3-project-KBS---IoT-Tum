
/*
    SR04.h
    Purpose: Setting up the HC-SR04 library

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 12/12/17
*/


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
	//volatile int iTrigger;
	//volatile int iEcho;
	
	//methods
	void(*SR04_init)(void);
	float(*SR04_read)(void);
}  sensSR04;



/******************************************************************************
  Function prototypes
******************************************************************************/
sensSR04 * new_sensSR04(void);
#endif // _SR04_H_