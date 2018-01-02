
/*
    ble.h
    Purpose: Setting up interface with the JDY-08 Bluetooth (LE) module.

    @author Willem Fikkert, Michiel van Dalfsen, Eldin Zenderink
    @version 1.0 16/12/17
*/


#ifndef _BLE_H_
#define _BLE_H_

#include "stdint.h"
#include "usart.h" 
#include "memman.h" 
/******************************************************************************
  Defines
******************************************************************************/


/******************************************************************************
  Structs
******************************************************************************/
typedef struct Devices{
	char* mac;
	char* deviceid;
	uint8_t index;	
} Devices;

typedef struct BleCon{
	//attributes
	char* receivebuffer;
	char* commandbuffer;  
  char mode;
	uint16_t timeout;
  uint8_t AckReceived;
  uint32_t firstAddress;
  uint8_t numberOfDevices;
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
	uint8_t(*Ble_SetTimeout)(uint16_t, struct BleCon *);
  void (*Ble_FreeDevices)(struct BleCon *);
}  BleCon;


/******************************************************************************
  Function prototypes
******************************************************************************/
BleCon *new_BleCon(SimpleUSART *com);
#endif // _BLE_H_
