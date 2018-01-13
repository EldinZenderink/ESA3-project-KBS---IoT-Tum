#include "ble.h"

/******************************************************************************
  Local Function Prototypes
******************************************************************************/
/*
 * Function : genereates cCommand to be send to ble module (JDY-08)
 * Parameter: 2x char pointer, 1x BleCon pointer, char* cCommand = the cCommand to send, char* cParameter, the cParameter to send, BleCon* con = the struct where info resides
 */
void GenerateCommand(char* cCommand, char* cParameter, BleCon *con){	
	if(con->cCommandBuffer != NULL){
		strcpy(con->cCommandBuffer, cCommand);
		strcat(con->cCommandBuffer, cParameter);
	}
}

/*
 * Function : sends setcName cCommand to ble module(JDY-08)
 * Parameter: 1x char pointer, 1x BleCon pointer, char* cName = the cName for the ble module,  BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1 or 0 depending on result/iTimeOut
 */
static uint8_t Ble_SetName(char* cName, BleCon *con){

  delay(500);
  GenerateCommand("AT+NAME", cName, con);
  con->com->USART_putstr(con->cCommandBuffer, con->com);

  uint16_t iTimeOutCount = 0;
  while (1) {
     char* cReceive = con->Ble_Receive(0, con);
     if(cReceive != NULL){
        if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
          return 1;
        } else if(strstr(con->Ble_Receive(0, con), "RR") != NULL){
          return 0;
        } else {
          return 0;
        }
     }
     else if (iTimeOutCount > (con->iTimeOut / 10)) {
      return 0;
     }

    delay(100);
    iTimeOutCount++;
  };
}

/*
 * Function : sends cMode cCommand to ble module(JDY-08)
 * Parameter: 1x char, 1x BleCon pointer, char CcMode = the cMode to be used,  BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1 or 0 depending on result/iTimeOut
 */
static uint8_t Ble_SetMode(char cMode, BleCon *con){


 con->cMode = cMode;
 delay(500);
  switch (cMode) {
  case '0':
    GenerateCommand("AT+HOSTEN", "0", con);
    con->com->USART_putstr(con->cCommandBuffer, con->com);
    break;
  case '1':
    GenerateCommand("AT+HOSTEN", "1", con);
    con->com->USART_putstr(con->cCommandBuffer, con->com);
    break;
  case '2':
    GenerateCommand("AT+HOSTEN", "2", con);
    con->com->USART_putstr(con->cCommandBuffer, con->com);
    break;
  case '3':
    GenerateCommand("AT+HOSTEN", "3", con);
    con->com->USART_putstr(con->cCommandBuffer, con->com);
    break;
  default:
    GenerateCommand("AT+HOSTEN", "0", con);
    con->com->USART_putstr(con->cCommandBuffer, con->com);
    break;
  }



  uint16_t iTimeOutCount = 0;
  while (iTimeOutCount < (con->iTimeOut / 10)) {
    char* cReceive = con->Ble_Receive(0, con);
    if(cReceive != NULL){
      if (strstr(cReceive, "OK") != NULL) {
        return 1;
      }else if (strstr(con->Ble_Receive(0, con), "RR") != NULL) {
        return 0;
      } else {
        return 0;   
      }
    }

    delay(10);
    iTimeOutCount++;
  };
  return 1;
}

/*
 * Function : sends cPassword cCommand to ble module(JDY-08)
 * Parameter: 1x char pointer, 1x BleCon pointer, char* cPass = the cPassword to set,  BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1 or 0 depending on result/iTimeOut 
 */
static uint8_t Ble_SetPass(char* cPass, BleCon *con){
	GenerateCommand("AT+PASS", cPass, con);
	con->Ble_Send(con->cCommandBuffer, 0, con);
	
	uint16_t iTimeOutCount = 0;
  while (1) {
    if (strstr(con->Ble_Receive(0, con), "OK") != NULL) {
      return 1;
    }
    else if (iTimeOutCount > con->iTimeOut) {
      return 0;
    }

    delay(100);
    iTimeOutCount++;
  };

}

/*
 * Function : scans for devices (in case of host cMode 1), returns devices found
 * Parameter: 1 1x BleCon pointer,BleCon* con = the struct where info resides
 * Returns  : Devices* pointer (struct) containing information about devices found or NULL in case of errors
 */
static Devices* Ble_ScanSlaves(BleCon *con){  
  GenerateCommand("AT+SCAN1","", con);
  con->Ble_Send(con->cCommandBuffer, 0, con);   
  uint16_t iTimeOutCount = 0;
  uint8_t iTimeOutCountlines = 0;
  
  while(1){
    char* cReceive = con->Ble_Receive(0, con);
    char *cMac;
    if(strstr(cReceive, "DEV") != NULL){
  
      Devices *device = (Devices*)malloc(sizeof(Devices) );
      if(device == NULL){
        return NULL;
      }
      if(iTimeOutCount == 0){
        con->iFirstAddress = MemMan_GetFreeBlock(13);
        cMac = (char*)con->iFirstAddress;
      }else{
        cMac = (char*)MemMan_GetFreeBlock(13);
      }
      
      uint8_t iCount;
      uint8_t iTimeOutCountcommas = 0;
      uint8_t iStartIndexId = 0;
      uint8_t endiIndexdid = 0;
      for(iCount =0; iCount < strlen(cReceive); iCount++){
         if(cReceive[iCount] == '='){
           strncpy(cMac, cReceive + iCount + 1, 12);
         }
         if(cReceive[iCount] == ','){
           iTimeOutCountcommas++;
         }
         
         if(iTimeOutCountcommas >= 2){
           iStartIndexId = iCount + 1;
           iTimeOutCountcommas = 0;
         }
      }
      cMac[12] = '\0';
      if(iCount - iStartIndexId - 1 <=10){
        char *cDeviceId = (char*)MemMan_GetFreeBlock(10); //allocate memory for deviceid, deviceid will always be <= 10;
        strncpy(cDeviceId, cReceive + iStartIndexId, iCount - iStartIndexId - 1);
        cDeviceId[iCount - iStartIndexId - 1] = '\0';
        
        device->cDeviceId = cDeviceId;
        device->cMac = cMac;
        device->iIndex= iTimeOutCountlines;
        
        con->devices[iTimeOutCountlines] = *device;
        iTimeOutCountlines++; 
        con->iNumberOfDevices = iTimeOutCountlines;
      }     
    }
    if(strstr(cReceive, "STOP") != NULL){        
      return con->devices;  
    } 
    iTimeOutCount++;
  };
  
  return 0;
  
}


/*
 * Function : specialized functions to free memory in custom memorymanagement library (memman.h) when it was allocated for devices struct
 * Parameter: 1 1x BleCon pointer,BleCon* con = the struct where info resides
 */
static void Ble_FreeDevices(BleCon *con){
  int iCount;
  for(iCount = 0; iCount< con->iNumberOfDevices +1; iCount++){
    free(&con->devices[iCount]);
  }
  MemMan_FreeBlock(con->iFirstAddress, con->iNumberOfDevices*23); //23 = length of mac address + max length of deviceid of ble device (allocating happens together so you can be sure that those are together as one memory block (due to how memman.h works)
  return;
}

/*
 * Function : return slave cMac after scanning depending on iIndex
 * Parameter: 1x uint8_t, 1x BleCon pointer, uint8_t iIndex= iIndex in scanned slaves (max 8 hence uint8_t), BleCon* con = the struct where info resides
 * Returns  : char* with cMac address
 */
static char* Ble_GetSlave(uint8_t iIndex, BleCon *con){
		char cBuf[2];
	  sprintf(cBuf, "%d", iIndex);
		GenerateCommand("AT+RSLV",cBuf, con);
		con->Ble_Send(con->cCommandBuffer, 0, con);		
	
			
		uint16_t iTimeOutCount = 0;
		while(1){
		 char* cReceive = con->Ble_Receive(0,con);
			if(strstr(cReceive, "MAC") != NULL){
				return cReceive;
			} else if(iTimeOutCount > con->iTimeOut){
				return 0;
			}
			
			delay(100);
			iTimeOutCount++;
		};
}

/*
 * Function : connects to a slave indentified with cMac address
 * Parameter: 1x char pointer, 1x BleCon pointer,char *cMac= cMac address to connect to, BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1
 * TODO: determine if connection is succesful
 */ 
static uint8_t Ble_ConnectSlave(char *cMac, BleCon *con){
	GenerateCommand("AT+BAND",cMac, con);	
	con->Ble_Send(con->cCommandBuffer,0, con);	
	return 1;	
}

/*
 * Function : send a string to the ble module
 * Parameter: 1x char pointer, 1x uint8_t, 1x BleCon pointer,char *str= string to send, uint8_t needAck = to wait for cReceiveeiving end to acknowledge cReceive, BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1 on ack or when no needAck is set always 1 after send.
 * TODO: need a failedtosend check that / iTimeOut / max retries etc.
 */ 
static uint8_t Ble_Send(char *str,  uint8_t needAck, BleCon *con){
  con->iAckSendSwitch = 0;
  uint16_t iTimeOutCount = 0;

  while (1) {
    switch (con->iAckSendSwitch) {
    case 0:
      iTimeOutCount = 0;
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
      else if (iTimeOutCount > con->iTimeOut) {
        con->iAckSendSwitch = 0;
      }

      delay(100);
      iTimeOutCount++;
      break;
    case 10:
      return 1;
    default:
      con->iAckSendSwitch = 0;
    }
  }
}

/*
 * Function : receive a string from the ble module
 * Parameter: 1x uint8_t, 1x BleCon pointer, uint8_t needAck = to wait for cReceiveeiving end to acknowledge receive, BleCon* con = the struct where info resides
 * Returns  : uint8_t with 1 on ack or when no needAck is set always 1 after send. Returns 0 if the received value is larger than the current buffer size of 100 chars;
 * TODO: need a failedtosend check that / iTimeOut / max retries etc.
 */ 
static char* Ble_Receive(uint8_t needAck, BleCon *con){
	con->iAckReceiveSwitch = 0;
  char* cToReturn;
  while (1) {
    switch (con->iAckReceiveSwitch) {
    case 0:
     
      con->cReceiveBuffer = (char*)realloc(con->cReceiveBuffer, sizeof(char) * 100); //current buffer size = 100 hardcoded
      con->com->USART_getstr(con->cReceiveBuffer, 1000, con->com); 
      if(strlen(con->cReceiveBuffer) > 0 && strlen(con->cReceiveBuffer )< 101){        
        cToReturn = (char*)malloc(sizeof(char) * strlen(con->cReceiveBuffer) + sizeof(char));      
        strcpy(cToReturn, con->cReceiveBuffer);
        cToReturn[strlen(con->cReceiveBuffer) + 1] = '\0'; 
      } else {
        cToReturn = NULL;
      }
      if (strlen(con->cReceiveBuffer) != 0 && needAck) {
        con->iAckReceiveSwitch = 1;
      }
      else {       
        return cToReturn;
      }
      break;

    case 1:
      con->com->USART_putstr("  ACK", con->com);
      return cToReturn;
    default:
      con->iAckReceiveSwitch = 0;
      break;
    }
  }	
}

/*
 * Function : set iTimeOut for ble module
 * Parameter: 1x uint16_t, 1x BleCon pointer, uint16_t iTimeOut = amount of time to wait until iTimeOut is reached, BleCon* con = the struct where info resides
 */
static void Ble_SetTimeout(uint16_t iTimeOut, BleCon *con){
	con->iTimeOut = iTimeOut / 100;
}

/*
 * Function : constructor for ble module library
 * Parameter: 1x SimpleUSART pointer, used for communicating with the BLE module through SimpleUsart (usart.h) (made porting easier/ quicker)
 * Returns  : BleCon pointer
 */
BleCon *new_BleCon(SimpleUSART *com){;
	char* cBuf1 = (char*)malloc(sizeof(char) * 100);
	char* cBuf2 = (char*)malloc(sizeof(char) * 100);

  if(cBuf1 == NULL || cBuf2 == NULL){
    return NULL;
  }
  
	BleCon *con = (BleCon*)malloc(sizeof(*con));
  if(con == NULL){
    return NULL;
  }
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
	con->iTimeOut = 10;
  con->cReceiveBuffer = cBuf1;
	con->cCommandBuffer = cBuf2;
  con->cMode = 0;
  con->iAckReceiveSwitch = 0;
  con->iAckSendSwitch = 0;  
  con->iNumberOfDevices = 0;  
  con->iAckReceived = 0;
  con->iFirstAddress = 0;

	return con;
}
