#include "ble.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



void generateCommand(char* command, char* parameter, BleCon *con){	
	if(con->commandbuffer != NULL){
		strcpy(con->commandbuffer, command);
		strcat(con->commandbuffer, parameter);
	}
}

static uint8_t Ble_SetName(char* name, BleCon *con){
	

  delay(500);
  generateCommand("AT+NAME", name, con);
  con->com->USART_putstr(con->commandbuffer, con->com);

  uint16_t count = 0;
  while (1) {
    if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
      return 1;
    }
    else if (count > (con->timeout / 10) || strstr(con->Ble_Receive(0, con), "RR") != NULL) {
      return 0;
    }

    delay(100);
    count++;
  };
}

static uint8_t Ble_SetMode(char cMode, BleCon *con){


 con->mode = cMode;
 delay(500);
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



  uint16_t count = 0;
  while (count < (con->timeout / 10)) {
    char* receive = con->Ble_Receive(0, con);
    if (strstr(receive, "OK") != NULL) {
      return 1;
    }
    if (strstr(con->Ble_Receive(0, con), "RR") != NULL) {
      return 0;
    }

    delay(10);
    count++;
  };
  return 1;
}

static uint8_t Ble_SetPass(char* pass, BleCon *con){
	generateCommand("AT+PASS", pass, con);
	con->Ble_Send(con->commandbuffer, 0, con);
	
	uint16_t count = 0;
  while (1) {
    if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
      return 1;
    }
    else if (count > con->timeout) {
      return 0;
    }

    delay(100);
    count++;
  };

}


static Devices* Ble_ScanSlaves(BleCon *con){
  
  
  generateCommand("AT+SCAN1","", con);
  con->Ble_Send(con->commandbuffer, 0, con);   
  uint16_t count = 0;
  uint8_t countlines = 0;
  
  while(1){
    char* rec = con->Ble_Receive(0, con);
    char *mac;
    if(strstr(rec, "DEV") != NULL){
  
      Devices *device = (Devices*)malloc(sizeof(Devices) );
      if(count == 0){
        con->firstAddress = MemMan_GetFreeBlock(13);
        mac = (char*)con->firstAddress;
      }else{
        mac = (char*)MemMan_GetFreeBlock(13);
      }
      
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
      if(i - startindexdid - 1 <=10){
        char *deviceid = (char*)MemMan_GetFreeBlock(10);
        strncpy(deviceid, rec + startindexdid - 0, i - startindexdid - 1);
        deviceid[i - startindexdid - 1] = '\0';
        
        device->deviceid = deviceid;
        device->mac = mac;
        device->index= countlines;
        
        con->devices[countlines] = *device;
        countlines++; 
        con->numberOfDevices = countlines;
      }     
    }
    if(strstr(rec, "STOP") != NULL){        
      return con->devices;  
    } 
    count++;
  };
  
  return 0;
  
}



static void Ble_FreeDevices(BleCon *con){
  int i;
  for(i = 0; i < con->numberOfDevices +1; i++){
    free(&con->devices[i]);
  }
  MemMan_FreeBlock(con->firstAddress, con->numberOfDevices*23);
  
  return;
}


static char* Ble_GetSlave(uint8_t index, BleCon *con){
		char buf[2];
	  sprintf(buf, "%d", index);
		generateCommand("AT+RSLV",buf, con);
		con->Ble_Send(con->commandbuffer, 0, con);		
	
			
		uint16_t count = 0;
		while(1){
		 char* rec = con->Ble_Receive(0,con);
			if(strstr(rec, "MAC") != NULL){
				return rec;
			} else if(count > con->timeout){
				return 0;
			}
			
			delay(100);
			count++;
		};
}

static uint8_t Ble_ConnectSlave(char *mac, BleCon *con){
	generateCommand("AT+BAND",mac, con);	
	con->Ble_Send(con->commandbuffer,0, con);	
	return 1;	
}

static uint8_t Ble_Send(char *str,  uint8_t needAck, BleCon *con){
  con->iAckSendSwitch = 0;
  uint16_t count = 0;

  while (1) {
    switch (con->iAckSendSwitch) {
    case 0:
      count = 0;
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
      else if (count > (con->timeout / 5)) {
        con->iAckSendSwitch = 0;
      }

      delay(100);
      count++;
      break;
    case 10:
      return 1;
    default:
      con->iAckSendSwitch = 0;
    }
  }
}


static char* Ble_Receive(uint8_t needAck, BleCon *con){
	con->iAckReceiveSwitch = 0;

  while (1) {
    switch (con->iAckReceiveSwitch) {
    case 0:
      con->com->USART_getstr(con->receivebuffer, 1000, con->com);
      if (strlen(con->receivebuffer) != 0 && needAck) {
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

static uint8_t Ble_SetTimeout(uint16_t timeout, BleCon *con){
	con->timeout = timeout;
	return 1;
}

BleCon *new_BleCon(SimpleUSART *com){
  uint8_t receiveAck = 0;
  uint32_t firstAddress = 0;
  uint8_t numberOfDevices = 0;
  char mode;
  uint8_t iAckSendSwitch = 0;
  uint8_t iAckReceiveSwitch = 0;
	char* buf1 = (char*)malloc(sizeof(char) * 100);
	char* buf2 = (char*)malloc(sizeof(char) * 100);
	BleCon *con = (BleCon*)malloc(sizeof(*con));
	con->Ble_ConnectSlave = Ble_ConnectSlave;
	con->Ble_Receive = Ble_Receive;
	con->Ble_ScanSlaves = Ble_ScanSlaves;
	con->Ble_Send = Ble_Send;
	con->Ble_SetPass = Ble_SetPass;
	con->Ble_SetMode = Ble_SetMode;
	con->Ble_SetName = Ble_SetName;
	con->Ble_SetTimeout = Ble_SetTimeout;	
	con->Ble_GetSlave = Ble_GetSlave;
	con->com = com;
	con->timeout = 10;
  con->receivebuffer = buf1;
	con->commandbuffer = buf2;
  con->mode = mode;
  con->iAckReceiveSwitch = iAckReceiveSwitch;
  con->iAckSendSwitch = iAckSendSwitch;  
  con->numberOfDevices = numberOfDevices;  
  con->AckReceived = receiveAck;
  con->firstAddress = firstAddress;

	return con;
}
