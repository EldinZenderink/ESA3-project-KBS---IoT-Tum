
/******************************************************************************
  Includes
******************************************************************************/
#include <ESP8266WiFi.h>
#include "mlabdb.h"
#include "SimpleJson.h"
#include "usart.h"
#include "ble.h"

/******************************************************************************
  Global VariaBleGlobals
******************************************************************************/
MLABDB DbGlobal("https://api.mlab.com", "zV5Ff3TEUTaSDvUoWVu3Y1q_1YkFZQNL", "E9 79 8D 5A 7E 2A DF 4B C8 FB 29 64 65 01 3F 29 8F 86 2E 92");
SimpleUSART *UsartGlobal;
BleCon *BleGlobal;
SimpleJson *JsonGlobal;
SimpleJson *SensorsJsonGlobal;

/* MASTER SPECIFIC INFORMATION */
static char* cFullIdGlobal = "T-BuildingSanitairT1_5";
static char* cMasterIdGlobal = "SanitairT1.5";
static char* cBuildingIdGlobal= "T-Building";
static char* cFloorIdGlobal= "1";

/* WIFI INFO */
static char* cSsidGlobal     = "EldinTaBleGlobalt";
static char* cPasswordGlobal = "05200205";

/* ENABLE/DISABLE DEBUGGING */
static uint8_t iEnableDebugging = 0;

/******************************************************************************
  Function Prototypes
******************************************************************************/
/*
 * Function: print string on new line when debugging is enaBleGlobald
 * Parameter: char pointer to string
 */
void PrintLineString(char* cToPrint){
  if(iEnableDebugging){
    Serial.println(cToPrint);
  }
}

/*
 * Function: print string on current line when debugging is enaBleGlobald
 * Parameter: char pointer to string
 */
void PrintString(char* cToPrint){
  if(iEnableDebugging){
    Serial.print(cToPrint);
  }
}

/*
 * Function: print int on new line when debugging is enaBleGlobald
 * Parameter: integer to print
 */
void PrintLineInt(int iToPrint){
  if(iEnableDebugging){
    Serial.println(iToPrint,DEC);
  }
}

/*
 * Function: print int on current line when debugging is enaBleGlobald
 * Parameter: integer to print
 */
void PrintInt(int iToPrint){
  if(iEnableDebugging){
    Serial.print(iToPrint,DEC);
  }
}

/*
 * Function : To fully free a poiner and leave no dangeling pointers behind.
 * Parameter: char pointer 
 */
void FreeChar(char* cFreeThis) {
  free(cFreeThis);
  cFreeThis = NULL;
}

/*
 * Function : To print the free heap together with a unique identifier.
 * Parameter: char pointer contianing unique identifier
 */
void GetHeapMain(char* cLine){
    PrintString("line-main: ");
    PrintString(cLine);
    PrintString(", heap: ");
    PrintInt(ESP.getFreeHeap());
    PrintLineString("");;
}

/*
 * Function : To cobine two char strings together into one
 * Parameter: 2x char pointer which will be combined 
 * Returns  : Combined string as char pointer
 */
char* CombineString(char*   cStr1, char*   cStr2)
{
  if (cStr1 == NULL || cStr2 == NULL) {
    return NULL;
  }
  int iNewSize = strlen(cStr1)  + strlen(cStr2) + 1;

  char * cNewBuffer = (char *)malloc(iNewSize);
  if (cNewBuffer != NULL) {

    strcpy(cNewBuffer, cStr1);
    strcat(cNewBuffer, cStr2);

    return cNewBuffer;
  } else {
    return NULL;
  }

}

/*
 * Function : Sets up wifi connection with access point
 * Parameter: 2x char pointer which will be combined 
 */
void SetupWifi(char* cPassword, char* cSsid){
    PrintLineString("");;
    PrintString("connecting to ");
    PrintLineString(cSsid);  
    
    WiFi.begin(cSsid, cPassword);
  
    int iTimeOutCount = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      PrintString(".");
  
      if (iTimeOutCount > 20) {
        //start wifi connection
        delay(1000);
        PrintLineString("COULD NOT FIND WIFI HOTSPOT, RETRYING:");
        WiFi.begin(cSsidGlobal, cPassword);
        iTimeOutCount = 0;
      }
      iTimeOutCount++;
    }
    
    PrintLineString("");
    PrintLineString("WiFi connected");
  
    delay(1000);
  
}

/*
 * Function : Setup the ESP to do the following: 
 *            1. Setup serial.  
 *            2. Connect to wifi. 
 *            3. EnaBleGlobal/DisaBleGlobal debugging in Database Library
 *            4. Get time & cDate.
 *            5. Generate base json. 
 *            6. Insert basejson into database (let database handle if it exists or not)
 *            7. Setup SimpleUsart (UsartGlobal.h) library (only needed for BleGlobal.h)
 *            8. Setup BleCon (BleGlobal.h)
 *            9. Setup BLE Module (JDY-08) through BleCon (BleGlobal.h)
 *            
 * Parameter: 2x char pointer which will be combined 
 */
void setup() {
  //1. Setup serial.  
  Serial.begin(115200);

  //2. Setup WIFI
  SetupWifi(cPasswordGlobal, cSsidGlobal);

  //3. EnaBleGlobal/DisaBleGlobal debugging in Database Library
  DbGlobal.EnableDebugging(iEnableDebugging);

  //4. Get time & cDate.
  char* cTimeStamp;
  while (1) {
    cTimeStamp = DbGlobal.GetTimeStamp();
    if(cTimeStamp== NULL){      
      FreeChar(cTimeStamp);
      PrintLineString("COULD NOT GET DATE, RETRYING");
    } else {
      break;
    }
    delay(1000);
  }
  char* cDate = strtok(cTimeStamp, " ");  

  //5. Generate base json. 
  JsonGlobal = new_SimpleJson(800);

  JsonGlobal->AddFieldWithString("SanitairID", cMasterIdGlobal, JsonGlobal);
  JsonGlobal->AddFieldWithString("BuildingID", cBuildingIdGlobal, JsonGlobal);
  JsonGlobal->AddFieldWithString("FloorID", cFloorIdGlobal, JsonGlobal);

  SensorsJsonGlobal = new_SimpleJson(800);

  JsonGlobal->AddFieldWithObject("Sensors", SensorsJsonGlobal, JsonGlobal);

  //6. Insert basejson into database (let database handle if it exists or not)  
  SimpleJson *withDate = new_SimpleJson(600);
    
  char*  cPart1 = CombineString(cDate, ".");  
  FreeChar(cPart1);
  char*  cPart2 = CombineString(cPart1, cFullIdGlobal);
  withDate->AddFieldWithObject(cPart2, JsonGlobal, withDate);
  FreeChar(cPart2);
  
  char* cJsonToSend = withDate->ToString(withDate);
  
  FreeSimpleJson(withDate);
  
  if (cJsonToSend == NULL) {
    PrintLineString("UNEXPECTED NULL, RESTARTING");
    ESP.reset();
  }

  while (!DbGlobal.UpsertFieldInDocument("tum", "sanitair", "FullDB", cJsonToSend)) {
    PrintLineString("COULD NOT UPSERT JSON, RETRYING");
    delay(1000);
  }  
  FreeChar(cJsonToSend);

  //7. Setup SimpleUsart (UsartGlobal.h) library (only needed for BleGlobal.h)
  UsartGlobal = new_SimpleUSART();
  if (UsartGlobal == NULL) {
    PrintLineString("UNEXPECTED NULL, RESTARTING");
    ESP.reset();
  }
  UsartGlobal->USART_init(UsartGlobal);

  //7. Setup BleCon (BleGlobal.h)
  BleGlobal = new_BleCon(UsartGlobal);
  if (BleGlobal == NULL) {
    PrintLineString("UNEXPECTED NULL, RESTARTING");
    ESP.reset();
  }

  //9. Setup BLE Module (JDY-08) through BleCon (BleGlobal.h)
  BleGlobal->Ble_SetMode('0', BleGlobal);
  delay(500);
  BleGlobal->Ble_SetName("TEST", BleGlobal);
  delay(500);
}


/*
 * Function : inifinite loop to handle everything necesary for the program to work
 *            1. Receive data from BleGlobal module  
 *            2. Get time & cDate.
 *            3. Generate json from received data
 *            4. Insert received data as json into sensor json
 *            5. Get the already stored sensor data and sum up the uses, then upsert data as json into sensor json
 *            6. Replace current sensor json in base json with new sensor json
 *            7. Upsert basejson into database (let database handle if it exists or not)
 *            
 * Parameter:0
 * Extra info: contains method to print the freeheap, but it will only print once debugging mode is enaBleGlobald
 */
void loop() {

    //1. Receive data from BleGlobal module  
    GetHeapMain("1");
    char* cReceived = BleGlobal->Ble_Receive(1, BleGlobal); //alloc 1
    GetHeapMain("2");
    
    delay(500);
    
    if (cReceived != NULL && strlen(cReceived) > 10) {
        
        //2. Get time & cDate.
        char* cTimeStamp1;
        while ((cTimeStamp1 = DbGlobal.GetTimeStamp()) == NULL) { //alloc 2
          delay(1000);
          FreeChar(cTimeStamp1); //free 1
          GetHeapMain("3-1");
        }

        //3. Generate json from received data
        GetHeapMain("3");    
        SimpleJson *ReceivedSensorJson = parse_SimpleJson(100, cReceived);//alloc 3
        GetHeapMain("4");

        
        if(ReceivedSensorJson != NULL){
          
          char* cId = ReceivedSensorJson->GetFieldWithString("id", ReceivedSensorJson); //alloc 4
          GetHeapMain("5");
          
          char* cCurrentSensorJson = SensorsJsonGlobal->ToString(SensorsJsonGlobal); //alloc 5
          GetHeapMain("6");
          
          if(strstr(cCurrentSensorJson, cId) == NULL){
            
            // 4. Insert received data as json into sensorjson
            ReceivedSensorJson->AddFieldWithString("latestupcDate",cTimeStamp1, ReceivedSensorJson);
            GetHeapMain("6-1");
            
            SensorsJsonGlobal->AddFieldWithObject(cId, ReceivedSensorJson, SensorsJsonGlobal);
            GetHeapMain("6-2");
            
          } else{
            
            //5. Get the already stored sensor data and sum up the uses, then upsert data as json into sensorjson
            SimpleJson *LocalSensorJson = SensorsJsonGlobal->GetFieldWithObject(cId, SensorsJsonGlobal);//alloc 6
            GetHeapMain("6-1");
            
            if(LocalSensorJson != NULL){
                   
                uint32_t iCurrent = LocalSensorJson->GetFieldWithInt("us", LocalSensorJson);
                GetHeapMain("6-2");
                 
                uint32_t iToAdd = ReceivedSensorJson->GetFieldWithInt("us",ReceivedSensorJson);
                GetHeapMain("6-3");
                 
                uint32_t iNewCurrent = iCurrent + iToAdd;
                GetHeapMain("6-4");
                 
                LocalSensorJson->ReplaceFieldWithInt("us", iNewCurrent, LocalSensorJson);                
                GetHeapMain("6-5");
                 
                LocalSensorJson->ReplaceFieldWithString("latestupcDate",cTimeStamp1, LocalSensorJson);                
                GetHeapMain("6-6");
                
                SensorsJsonGlobal->ReplaceFieldWithObject(cId, LocalSensorJson, SensorsJsonGlobal);                
                GetHeapMain("6-7");
              
            } 
            
            FreeSimpleJson(LocalSensorJson);   //free 1           
            GetHeapMain("free 6-1");
            
          }
          
          if(SensorsJsonGlobal != NULL){ 
              //6. Replace current sensor json in base json with new sensor json                       
              GetHeapMain("7");
              JsonGlobal->ReplaceFieldWithObject("Sensors", SensorsJsonGlobal, JsonGlobal);
          }  
          
          //7. Upsert basejson into database (let database handle if it exists or not)
          GetHeapMain("8");
          SimpleJson *withDate = new_SimpleJson(600);  //alloc 5        
          GetHeapMain("9");
          
          char* cDate1 = strtok(cTimeStamp1, " ");  //alloc ?
          GetHeapMain("10");  
               
          char*  cPart1 = CombineString(cDate1, ".");  //alloc 6
          GetHeapMain("11");   
           
          FreeChar(cTimeStamp1); //free 2
          GetHeapMain("free 3");  
            
          char*  cPart2 = CombineString(cPart1, cFullIdGlobal); //alloc 6 
          GetHeapMain("12");    
          
          FreeChar(cPart1);//free 3      
          GetHeapMain("free 11");    
          
          withDate->AddFieldWithObject(cPart2, JsonGlobal, withDate); //alloc 6
          GetHeapMain("13");
             
          FreeChar(cPart2); //free 4         
          GetHeapMain("free 12");
            
          char* cJsonToSend = withDate->ToString(withDate);  //alloc 6  
          GetHeapMain("14");
             
          FreeSimpleJson(withDate); //free 5           
          GetHeapMain("free 9");
                      
          if (cJsonToSend == NULL || withDate == NULL) {
            PrintLineString("UNEXPECTED NULL, RESTARTING");
            ESP.reset();
          }
          
          while (!DbGlobal.UpsertFieldInDocument("tum", "sanitair", "FullDB", cJsonToSend)) {
            PrintLineString("COULD NOT UPSERT JSON, RETRYING"); 
            GetHeapMain("15");  
            delay(1000);
          }
                 
          GetHeapMain("16");  
          FreeChar(cJsonToSend);//free 6 
          GetHeapMain("free 14"); 
          
          FreeChar(cReceived); //free 7 
          GetHeapMain("free 2");
          
          FreeSimpleJson(ReceivedSensorJson); //free 8
          GetHeapMain("free 4");
          
          FreeChar(cId); //free 9
          GetHeapMain("free 5");
          
          FreeChar(cCurrentSensorJson); //free10
          GetHeapMain("free 6");

        } else {
          PrintLineString("Received sensor data is not json ;(");
          PrintLineString(cReceived);
          FreeChar(cReceived);//free 1 
          GetHeapMain("free 2");
        }
        
    }else{
        FreeChar(cReceived);//free 1 
    }
}


