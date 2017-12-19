
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

#include "stm32f10x.h" 
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
	char cEnergyMode;
	char* receivebuffer;
	char* commandbuffer;
	uint8_t pinpower;
	uint8_t pinprogram;
	uint16_t timeout;
	GPIO_TypeDef *gpio;
	SimpleUSART *com;
	SimpleUSART *passthrough;
	
	//methods
	uint8_t(*Ble_Init)(struct BleCon *);
	uint8_t(*Ble_ON)(struct BleCon *);
	uint8_t(*Ble_OFF)(struct BleCon *);
	uint8_t(*Ble_EnableProgramMode)(struct BleCon *);
	uint8_t(*Ble_DisableProgramMode)(struct BleCon *);
	uint8_t(*Ble_SetPass)(char*,struct BleCon *);
	uint8_t(*Ble_SetName)(char*,struct BleCon *);
	uint8_t(*Ble_SetMode)(char,struct BleCon *);
	Devices*(*Ble_ScanSlaves)(struct BleCon *);
	char*(*Ble_GetSlave)( uint8_t index, struct BleCon *);
	uint8_t(*Ble_ConnectSlave)(char*, struct BleCon *);	
	uint8_t(*Ble_Send)(char*, struct BleCon *);
	char*(*Ble_Receive)(struct BleCon *);
	uint8_t(*Ble_SetTimeout)(uint16_t, struct BleCon *);
	uint8_t(*Ble_Check_Ack)(struct BleCon *);
	uint8_t(*Ble_Send_Ack)(struct BleCon *);
	char*(*Ble_Receive_Ack)(struct BleCon *);
	uint8_t(*Ble_Send_For_Ack)(char*, struct BleCon *);	
}  BleCon;


/******************************************************************************
  Function prototypes
******************************************************************************/
BleCon *new_BleCon(char cEnergyMode, uint8_t pinpower, uint8_t pinprogram, GPIO_TypeDef *pingroup, SimpleUSART *com, SimpleUSART *passthrough);
#endif // _BLE_H_
