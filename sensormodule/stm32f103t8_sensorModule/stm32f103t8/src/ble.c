/*
    BlueTooth Low Energy
		ble.c
		Purpose: making an interface for BLE
		
		@author: Eldin Zenderink, Willem Fikkert
		@version: 1.0
*/
//includes
#include "ble.h"
#include "delay.h"
#include "memman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//generate an AT command for the BLE module
void generateCommand(char* command, char* parameter, BleCon *con) {
	if (con->commandbuffer != NULL) {
		strcpy(con->commandbuffer, command);
		strcat(con->commandbuffer, parameter);
	}
}

//Initialize the ble, set program pin and power pin.
static uint8_t Ble_Init(BleCon *con) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	if (con->pinpower > 7) {
		con->gpio->CRH ^= ((0x7 << ((con->pinpower - 8) * 4)));
		con->gpio->CRH ^= ((0x3 << ((con->pinprogram - 8) * 4)));

	}
	else {
		con->gpio->CRL ^= (0x7 << (con->pinpower * 4));
		con->gpio->CRL ^= (0x3 << (con->pinprogram * 4));
	}
	con->Ble_EnableProgramMode(con);
	con->Ble_ON(con);
	delay_ms(500);
	return 1;
}

//Set the name of the BLE module, first generate an AT+NAME command with the name directly (so no spaces) behind it.
static uint8_t Ble_SetName(char* name, BleCon *con) {

	delay_ms(500);
	generateCommand("AT+NAME", name, con);
	con->com->USART_putstr(con->commandbuffer, con->com);

	uint16_t iCount = 0;
	while (1) {
		if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
			return 1;
		}
		else if (iCount > con->timeout || strstr(con->Ble_Receive(0, con), "RR") != NULL) {
			return 0;
		}

		delay_ms(100);
		iCount++;
	};
}

//Set the mode of the BLE module, first generate an AT+HOSTEN command with the mode directly (so no spaces) behind it.
static uint8_t Ble_SetMode(char cMode, BleCon *con) {
	con->mode = cMode;
	delay_ms(500);
	switch (cMode) {
	case '0':
		generateCommand("AT+HOSTEN", "0", con);
		con->com->USART_putstr(con->commandbuffer, con->com);
		break;
	case '1':
		generateCommand("AT+HOSTEN", "1", con);
		con->com->USART_putstr(con->commandbuffer, con->com);
		break;
	case '2':
		generateCommand("AT+HOSTEN", "2", con);
		con->com->USART_putstr(con->commandbuffer, con->com);
		break;
	case '3':
		generateCommand("AT+HOSTEN", "3", con);
		con->com->USART_putstr(con->commandbuffer, con->com);
		break;
	default:
		generateCommand("AT+HOSTEN", "0", con);
		con->com->USART_putstr(con->commandbuffer, con->com);
		break;
	}
	uint16_t iCount = 0;
	while (iCount < (con->timeout / 10)) {
		char* receive = con->Ble_Receive(0, con);
		if (strstr(receive, "OK") != NULL) {
			return 1;
		}
		con->passthrough->USART_putstr(receive, con->passthrough);
		if (strstr(con->Ble_Receive(0, con), "RR") != NULL) {
			return 0;
		}

		delay_ms(10);
		iCount++;
	};
	return 1;
}

//enables the power pin.
static uint8_t Ble_ON(BleCon *con) {
	con->gpio->BSRR = (1 << (con->pinpower));
	return 1;
}

//disables the power pin.
static uint8_t Ble_OFF(BleCon *con) {

	con->gpio->BSRR = 0;
	con->gpio->BSRR = (1 << ((con->pinpower) + 16));
	return 1;
}

//Enable the program pin.
static uint8_t Ble_EnableProgramMode(BleCon *con) {
	con->gpio->BSRR = (1 << (con->pinprogram + 16));
	return 1;
}

//Disables the program pin.
static uint8_t Ble_DisableProgramMode(BleCon *con) {
	con->gpio->BSRR = (1 << con->pinprogram);
	return 1;
}

//Set the password for the BLE module, generate an AT+PASS command with the password directly behind it.
static uint8_t Ble_SetPass(char* pass, BleCon *con) {
	generateCommand("AT+PASS", pass, con);
	con->Ble_Send(con->commandbuffer, 0, con);

	uint16_t iCount = 0;
	while (1) {
		if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
			return 1;
		}
		else if (iCount > con->timeout) {
			return 0;
		}

		delay_ms(100);
		iCount++;
	};

}

//Scan for slaves, generate an AT+SCAN command.
static Devices* Ble_ScanSlaves(BleCon *con) {


	generateCommand("AT+SCAN1", "", con);
	con->com->USART_putstr(con->commandbuffer, con->com);
	uint16_t iCount = 0;
	uint8_t iCountlines = 0;

	while (1) {
		char* rec = con->Ble_Receive(0, con);
		char *cMac;
		con->passthrough->USART_putstr("\n", con->passthrough);
		con->passthrough->USART_putstr(rec, con->passthrough);
		con->passthrough->USART_putstr("\n", con->passthrough);
		if (strstr(rec, "DEV") != NULL) {

			Devices *device = malloc(sizeof(Devices));
			if (iCount == 0) {
				con->firstAddress = MemMan_GetFreeBlock(13);
				cMac = (char*)con->firstAddress;
			}
			else {
				cMac = (char*)MemMan_GetFreeBlock(13);
			}
			uint8_t i;
			uint8_t iCountcommas = 0;
			uint8_t iStartindexdid = 0;
			uint8_t endindexdid = 0;
			for (i = 0; i < strlen(rec); i++) {
				if (rec[i] == '=') {
					strncpy(cMac, rec + i + 1, 12);
				}
				if (rec[i] == ',') {
					iCountcommas++;
				}

				if (iCountcommas >= 2) {
					iStartindexdid = i + 1;
					iCountcommas = 0;
				}
			}
			cMac[12] = '\0';


			if (i - iStartindexdid - 1 <= 10) {
				char *iDeviceid = (char*)MemMan_GetFreeBlock(10);
				strncpy(iDeviceid, rec + iStartindexdid , i - iStartindexdid - 1);
				iDeviceid[i - iStartindexdid - 1] = '\0';

				device->deviceid = iDeviceid;
				device->mac = cMac;
				device->index = iCountlines;

				con->devices[iCountlines] = *device;
				iCountlines++;
				con->numberOfDevices = iCountlines;
			}
		}
		if (strstr(rec, "STOP") != NULL) {
			return con->devices;
		}
	iCount++;
	};
	return 0;

}

//free devices, so free the memory which is taken by the devices.
static void Ble_FreeDevices(BleCon *con) {
	int i;
	for (i = 0; i < con->numberOfDevices + 1; i++) {
		free(&con->devices[i]);
	}
	MemMan_FreeBlock(con->firstAddress, con->numberOfDevices * 23);//23 = length of mac address + max length of name of ble device (allocating happens together so you can be sure that those are together as one memory block (due to how memman.h works)

	return;
}

//Get slaves, generate an AT+RSLV command, return the MAC addresses of the devices found.
static char* Ble_GetSlave(uint8_t index, BleCon *con) {
	char buf[2];
	sprintf(buf, "%d", index);
	generateCommand("AT+RSLV", buf, con);
	con->Ble_Send(con->commandbuffer, 0, con);


	uint16_t iCount = 0;
	while (1) {
		char* rec = con->Ble_Receive(0, con);
		if (strstr(rec, "MAC") != NULL) {
			return rec;
		}
		else if (iCount > con->timeout) {
			return 0;
		}
		delay_ms(100);
		iCount++;
	};
}

//connect to slave, generate an AT+BAND command with the mac address.
static uint8_t Ble_ConnectSlave(char *mac, BleCon *con) {
	generateCommand("AT+BAND", mac, con);
	con->Ble_Send(con->commandbuffer, 0, con);
	return 1;
}

//send data to the other device, if needAck is true, then it waits for an acknowledgement, if it reaches an timeout, it tries to send it again.
static uint8_t Ble_Send(char *str, uint8_t needAck, BleCon *con) {
	con->iAckSendSwitch = 0;
	uint16_t iCount = 0;

	while (1) {
		switch (con->iAckSendSwitch) {
		case 0:
			iCount = 0;
			con->com->USART_putstr(str, con->com);
			if (needAck) {
				con->iAckSendSwitch = 1;
			}
			else {
				con->iAckSendSwitch = 10;
			}
			break;
		case 1:
			if (strstr(con->Ble_Receive(0, con), "ACK") != NULL) {
				return 1;
			}
			else if (iCount > (con->timeout / 5)) {
				con->passthrough->USART_putstr("No ACK received!", con->passthrough);
				con->iAckSendSwitch = 0;
			}
			delay_ms(100);
			iCount++;
			break;
		case 10:
			return 1;
		default:
			con->iAckSendSwitch = 0;
		}
	}
	return 1;
}

//receive data from other device, if need ack is true an ack is send.
static char* Ble_Receive(uint8_t needAck, BleCon *con) {
	con->iAckReceiveSwitch = 0;

	while (1) {
		switch (con->iAckReceiveSwitch) {
		case 0:
			con->com->USART_getstr(con->receivebuffer, 1000, con->com);
			if (con->receivebuffer != NULL && needAck) {
				con->iAckReceiveSwitch = 1;
			}
			else {

				return con->receivebuffer;
			}
			break;

		case 1:
			con->com->USART_putstr("  ACK", con->com);

			return con->receivebuffer;
		default:
			con->iAckReceiveSwitch = 0;
			break;
		}
	}
}

//change the timeout to a value you like.
static uint8_t Ble_SetTimeout(uint16_t timeout, BleCon *con) {
	con->timeout = timeout;
	return 1;
}

//configure .h file with .c file.
BleCon *new_BleCon(char cEnergyMode, uint8_t pinpower, uint8_t pinprogram, GPIO_TypeDef *pingroup, SimpleUSART *com, SimpleUSART *passthrough) {
	char* buf1 = (char*)MemMan_GetFreeBlock(100);
	char* buf2 = (char*)MemMan_GetFreeBlock(100);
	uint8_t receiveAck = 0;
	uint32_t firstAddress = 0;
	uint8_t numberOfDevices = 0;
	char mode;
	uint8_t iAckSendSwitch = 0;
	uint8_t iAckReceiveSwitch = 0;
	Devices *devices = malloc(8 * sizeof(Devices));
	BleCon *con = malloc(sizeof(*con));
	con->Ble_ConnectSlave = Ble_ConnectSlave;
	con->Ble_DisableProgramMode = Ble_DisableProgramMode;
	con->Ble_EnableProgramMode = Ble_EnableProgramMode;
	con->Ble_Init = Ble_Init;
	con->Ble_OFF = Ble_OFF;
	con->Ble_ON = Ble_ON;
	con->Ble_Receive = Ble_Receive;
	con->Ble_ScanSlaves = Ble_ScanSlaves;
	con->Ble_Send = Ble_Send;
	con->Ble_SetPass = Ble_SetPass;
	con->Ble_SetMode = Ble_SetMode;
	con->Ble_SetName = Ble_SetName;
	con->Ble_SetTimeout = Ble_SetTimeout;
	con->Ble_GetSlave = Ble_GetSlave;
	con->cEnergyMode = cEnergyMode;
	con->com = com;
	con->passthrough = passthrough;
	con->gpio = pingroup;
	con->pinpower = pinpower;
	con->pinprogram = pinprogram;
	con->timeout = 50;
	con->receivebuffer = buf1;
	con->commandbuffer = buf2;
	con->AckReceived = receiveAck;
	con->firstAddress = firstAddress;
	con->Ble_FreeDevices = Ble_FreeDevices;
	con->numberOfDevices = numberOfDevices;
	con->devices = devices;
	con->mode = mode;
	con->iAckReceiveSwitch = iAckReceiveSwitch;
	con->iAckSendSwitch = iAckSendSwitch;
	delay_init();

	return con;
}
