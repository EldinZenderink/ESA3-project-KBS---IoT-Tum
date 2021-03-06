
/*
ble.h
Purpose: Setting up interface with the JDY-08 Bluetooth (LE) module.

@author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
@version 1.0 
*/

#pragma once
#ifndef _BLE_H_
#define _BLE_H_

#include "stdint.h"
#include "usart.h"  

#include "stm32f10x.h" 
/******************************************************************************
Defines
******************************************************************************/


/******************************************************************************
Structs
******************************************************************************/
typedef struct Devices {
	char* mac;
	char* deviceid;
	uint8_t index;
} Devices;

typedef struct BleCon {
	//attributes
	char cEnergyMode;
	char* receivebuffer;
	char* commandbuffer;
	char mode;
	uint8_t pinpower;
	uint8_t pinprogram;
	uint16_t timeout;
	GPIO_TypeDef *gpio;
	SimpleUSART *com;
	SimpleUSART *passthrough;
	uint8_t AckReceived;
	uint32_t firstAddress;
	uint8_t numberOfDevices;
	Devices *devices;
	uint8_t iAckSendSwitch;
	uint8_t iAckReceiveSwitch;



	//methods
	uint8_t(*Ble_Init)(struct BleCon *);
	uint8_t(*Ble_ON)(struct BleCon *);
	uint8_t(*Ble_OFF)(struct BleCon *);
	uint8_t(*Ble_EnableProgramMode)(struct BleCon *);
	uint8_t(*Ble_DisableProgramMode)(struct BleCon *);
	uint8_t(*Ble_SetPass)(char*, struct BleCon *);
	uint8_t(*Ble_SetName)(char*, struct BleCon *);
	uint8_t(*Ble_SetMode)(char, struct BleCon *);
	Devices*(*Ble_ScanSlaves)(struct BleCon *);
	char*(*Ble_GetSlave)(uint8_t index, struct BleCon *);
	uint8_t(*Ble_ConnectSlave)(char*, struct BleCon *);
	uint8_t(*Ble_Send)(char*, uint8_t, struct BleCon *);
	char*(*Ble_Receive)(uint8_t, struct BleCon *);
	uint8_t(*Ble_SetTimeout)(uint16_t, struct BleCon *);
	void(*Ble_FreeDevices)(struct BleCon *);
}  BleCon;


/******************************************************************************
Function prototypes
******************************************************************************/
BleCon *new_BleCon(char cEnergyMode, uint8_t pinpower, uint8_t pinprogram, GPIO_TypeDef *pingroup, SimpleUSART *com, SimpleUSART *passthrough);
#endif // _BLE_H_
