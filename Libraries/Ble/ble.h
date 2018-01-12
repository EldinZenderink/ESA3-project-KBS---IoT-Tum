#pragma once
/*
    ble.h
    Purpose: Setting up interface with the JDY-08 Bluetooth (LE) module.

    @author(STM32F103): Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @port(ESP8266): Eldin Zenderink
    @version 1.0 16/12/17
*/


#ifndef _BLE_H_
#define _BLE_H_

/******************************************************************************
  Includes
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "usart.h" 
#include "memman.h" 

/******************************************************************************
  Structs
******************************************************************************/
typedef struct Devices{
	char* cMac;
	char* cDeviceId;
	uint8_t iIndex;	
} Devices;

typedef struct BleCon{
	//attributes
	char* cReceiveBuffer;
	char* cCommandBuffer;  
  char cMode;
	uint16_t iTimeOut;
  uint8_t iAckReceived;
  uint32_t iFirstAddress;
  uint8_t iNumberOfDevices;
  Devices *devices;
	SimpleUSART *com; 
	uint8_t iAckSendSwitch;
  uint8_t iAckReceiveSwitch;

 
	
	//methods
	uint8_t(*Ble_EnableProgramMode)(struct BleCon *);
	uint8_t(*Ble_DisableProgramMode)(struct BleCon *);
	uint8_t(*Ble_SetPass)(char*,struct BleCon *);
	uint8_t(*Ble_SetName)(char*,struct BleCon *);
	uint8_t(*Ble_SetMode)(char,struct BleCon *);
	Devices*(*Ble_ScanSlaves)(struct BleCon *);
	char*(*Ble_GetSlave)( uint8_t index, struct BleCon *);
	uint8_t(*Ble_ConnectSlave)(char*, struct BleCon *);	
  uint8_t(*Ble_Send)(char*, uint8_t, struct BleCon *);
  char*(*Ble_Receive)(uint8_t, struct BleCon *);
	void(*Ble_SetTimeout)(uint16_t, struct BleCon *);
  void (*Ble_FreeDevices)(struct BleCon *);
}  BleCon;


/******************************************************************************
  Function prototypes
******************************************************************************/
BleCon *new_BleCon(SimpleUSART *com);
#endif // _BLE_H_
