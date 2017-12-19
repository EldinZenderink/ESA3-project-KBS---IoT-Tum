#include "ble.h"
#include "delay.h"
#include "memman.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void generateCommand(char* command, char* parameter, BleCon *con){	
	if(con->commandbuffer != NULL){
		strcpy(con->commandbuffer, command);
		strcat(con->commandbuffer, parameter);		
	}
}

static uint8_t Ble_Init(BleCon *con){	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
	if(con->pinpower > 7){			
		con->gpio->CRH ^= ((0x7 << ((con->pinpower - 8) * 4)));
		con->gpio->CRH ^= ((0x3 << ((con->pinprogram - 8) * 4)));
		
	} else {
		con->gpio->CRL ^= (0x7 << (con->pinpower * 4));
		con->gpio->CRL ^= (0x3 << (con->pinprogram * 4));
	}		
	con->Ble_EnableProgramMode(con);
	con->Ble_ON(con);
	delay_ms(500);
	return 1;
}

static uint8_t Ble_SetName(char* name, BleCon *con){
	
	delay_ms(500);
	generateCommand("AT+NAME", name, con);
	con->com->USART_putstr(con->commandbuffer, con->com);		
	
	uint16_t count = 0;
	while(1){
		if(strstr(con->Ble_Receive(con), "OK") != NULL){
			return 1;
		} else if(count > con->timeout  || strstr(con->Ble_Receive(con), "RR") != NULL){
			return 0;
		}
		
		delay_ms(100);
		count++;
	};
}

static uint8_t Ble_SetMode(char cMode, BleCon *con){
	delay_ms(500);
	switch(cMode){
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
	
	
	
	uint16_t count = 0;
	while(1){
		if(strstr(con->Ble_Receive(con), "OK") != NULL){
			return 1;
		} else if(count > con->timeout  || strstr(con->Ble_Receive(con), "RR") != NULL){
			return 0;
		}
		
		delay_ms(100);
		count++;
	}; 
	return 1;
}

static uint8_t Ble_ON(BleCon *con){
	con->gpio->BSRR = (1 << (con->pinpower));
	return 1;	
}

static uint8_t Ble_OFF(BleCon *con){
	
	con->gpio->BSRR =  0;
	con->gpio->BSRR = (1 << ((con->pinpower ) + 16));
	return 1;	
}

static uint8_t Ble_EnableProgramMode(BleCon *con){	
	con->gpio->BSRR = (1 << (con->pinprogram + 16));
	return 1;		
}

static uint8_t Ble_DisableProgramMode(BleCon *con){	
	con->gpio->BSRR = (1 << con->pinprogram);	
	return 1;	
}

static uint8_t Ble_SetPass(char* pass, BleCon *con){
	generateCommand("AT+PASS", pass, con);
	con->Ble_Send(con->commandbuffer, con);		
	
	uint16_t count = 0;
	while(1){
		if(strstr(con->Ble_Receive(con), "OK") != NULL){
			return 1;
		} else if(count > con->timeout){
			return 0;
		}
		
		delay_ms(100);
		count++;
	};

}

static Devices* Ble_ScanSlaves(BleCon *con){
	
	
	generateCommand("AT+SCAN1","", con);
	con->Ble_Send(con->commandbuffer, con);		
	Devices *devices = malloc(8*sizeof(Devices) );
	uint16_t count = 0;
	uint8_t countlines = 0;
	while(1){
		char* rec = con->Ble_Receive(con);
		con->passthrough->USART_putstr("\n", con->passthrough);
	  con->passthrough->USART_putstr(rec, con->passthrough);
		con->passthrough->USART_putstr("\n", con->passthrough);
		if(strstr(rec, "DEV") != NULL){
	
			Devices *device = malloc(sizeof(Devices) );
			char *mac = (char*)MemMan_GetFreeBlock(13);
		
			
		
			
			uint8_t i;
			uint8_t countcommas = 0;
			uint8_t startindexdid = 0;
			uint8_t endindexdid = 0;
			for(i =0; i < strlen(rec); i++){
				 if(rec[i] == '='){
					 strncpy(mac, rec + i + 1, 12);
				 }
				 if(rec[i] == ','){
					 countcommas++;
				 }
				 
				 if(countcommas >= 2){
					 startindexdid = i + 1;
					 countcommas = 0;
				 }
				 
					 

			}
			mac[12] = '\0';
		
			char *deviceid = (char*)MemMan_GetFreeBlock(30);
			strncpy(deviceid, rec + startindexdid - 0, i - startindexdid - 1);
			deviceid[i - startindexdid - 1] = '\0';
			
			device->deviceid = deviceid;
			device->mac = mac;
			device->index= countlines;
			
			devices[countlines] = *device;
			countlines++;			
		}
		if(strstr(rec, "STOP") != NULL){				
			return devices;	
		} 
		
		
	
		
		count++;
	};
	
	return 0;
	
}

static char* Ble_GetSlave(uint8_t index, BleCon *con){
		char buf[2];
	  sprintf(buf, "%d", index);
		generateCommand("AT+RSLV",buf, con);
		con->Ble_Send(con->commandbuffer, con);		
	
			
		uint16_t count = 0;
		while(1){
		 char* rec = con->Ble_Receive(con);
			if(strstr(rec, "MAC") != NULL){
				return rec;
			} else if(count > con->timeout){
				return 0;
			}
			
			delay_ms(100);
			count++;
		};
}

static uint8_t Ble_ConnectSlave(char *mac, BleCon *con){
	generateCommand("AT+BAND",mac, con);	
	con->Ble_Send(con->commandbuffer, con);	
	return 1;	
}

static uint8_t Ble_Send(char *str, BleCon *con){
	con->com->USART_putstr(str, con->com);
	
		
	return 1;	
}

static uint8_t Ble_Send_For_Ack(char *str, BleCon *con){
	con->com->USART_putstr(str, con->com);
	
	while(!con->Ble_Check_Ack(con));
	con->passthrough->USART_putstr("\n Ack Received", con->passthrough);
		
	return 1;	
}

static char* Ble_Receive(BleCon *con){
	con->com->USART_getstr(con->receivebuffer, 1000, con->com);
	return con->receivebuffer;	
}

static uint8_t Ble_SetTimeout(uint16_t timeout, BleCon *con){
	con->timeout = timeout;
	return 1;
}

static uint8_t Ble_Check_Ack(BleCon *con){
	uint16_t count = 0;
	while(1){
		if(strstr(con->Ble_Receive(con), "ACK") != NULL){
			return 1;
		} else if(count > con->timeout){
			return 0;
		}
		
		delay_ms(100);
		count++;
	};
}

static uint8_t Ble_Send_Ack(BleCon * con){
	con->com->USART_putstr("  ACK", con->com);
	
}

static char* Ble_Receive_Ack(BleCon * con){
	con->com->USART_getstr(con->receivebuffer, 1000, con->com);
	
	
	//if(con->receivebuffer[0] != NULL){
		con->Ble_Send_Ack(con);
		
	/*}else{
		con->passthrough->USART_putstr("\n receivebuffer = null", con->passthrough);
		con->passthrough->USART_putstr(&con->receivebuffer[0], con->passthrough);
	}
	*/
	return con->receivebuffer;
}




BleCon *new_BleCon(char cEnergyMode, uint8_t pinpower, uint8_t pinprogram, GPIO_TypeDef *pingroup, SimpleUSART *com,  SimpleUSART *passthrough){
	char* buf1 = (char*)MemMan_GetFreeBlock(100);
	char* buf2 = (char*)MemMan_GetFreeBlock(100);
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
	con->Ble_Check_Ack = Ble_Check_Ack;
	con->Ble_Send_Ack = Ble_Send_Ack;
	con->Ble_Receive_Ack = Ble_Receive_Ack;
	con->Ble_Send_For_Ack = Ble_Send_For_Ack;
	delay_init();

	return con;
}
