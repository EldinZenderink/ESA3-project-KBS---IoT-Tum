/*
    mlabdb.c
    Purpose: Functions for mlabdb
    ToDo: Implement ssl, start is made, but its not in use at the moment
    @author Eldin Zenderink
    @version 1.0 01/01/18
*/


/******************************************************************************
  Includes
******************************************************************************/
#include "mlabdb.h"


/******************************************************************************
  Global Variables
******************************************************************************/

uint8_t bEnableDebug;
char* cHost;
char* cApiKey;
char* cApiEndPoint;
char* cSslFingerPrint;
HTTPClient* Http;


/******************************************************************************
   Local Function Prototypes
******************************************************************************/

/*
 * Function : To print the free heap together with a unique cIdentifier.
 * Parameter: char pointer contianing unique cIdentifier
 */
void GetHeapDB(char* cLine){
    Serial.print("cLine: ");
    Serial.print(cLine);
    Serial.print(", heap: ");
    Serial.print(ESP.getFreeHeap(), DEC);
    Serial.println();
}

/*
 * Function : To fully free a poiner and leave no dangeling pointers behind.
 * Parameter: char pointer 
 */
void FreeCharDB(char* ptr){
  free(ptr);
  ptr = NULL;
}

/*
 * Function : To cobine two char strings together into one
 * Parameter: 2x char pointer which will be combined 
 * Returns  : Combined string as char pointer
 */
char* CombineStringDB(char*   cStr1, char*   cStr2)
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
 * Function :to generate a Httprequest with either GET, POST, PUT or DELETE
 * Parameter: uint8_t, 3x char*, uint8_t iHttpRequestType = which request (0 = GET, 1 = POST, 2 = PUT, 3 = DELETE, default = GET), char* cBaseURL = base url without Http or query, char* extraurl = query, char* cBody = incase of POST/PUT the cBody to be send 
 * Returns  : char* pointing to response string
 */
char* GenerateHttpRequest(uint8_t iHttpRequestType, char* cBaseURL, char* cUrlExtra, char* cBody){
    Http = new HTTPClient();
    Http->setTimeout(3000);
    Http->begin(String("http://") + String(cBaseURL) + String(cUrlExtra)); //HTTPS
    Http->addHeader("X-Requested-With", "ESP8266-TUM");
    int iHttpCode = -1;
    switch(iHttpRequestType){
      case 0:
        iHttpCode = Http->GET();
        break;
      case 1:        
        Http->addHeader("Content-Type", "application/json");
        iHttpCode = Http->POST(cBody);
        break;
      case 2:          
        Http->addHeader("Content-Type", "application/json");
        iHttpCode = (int) Http->PUT(cBody);
        break;
      case 3:
        iHttpCode = (int) Http->sendRequest("DELETE");
        break;
      default:
        iHttpCode = Http->GET();
        break;      
    }
    if(iHttpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(bEnableDebug){          
          Serial.println("[HTTP] GET... code: \n");
          Serial.print( iHttpCode, DEC);
        }
        // file found at server
        if(iHttpCode == HTTP_CODE_OK) {
            String sPayload = Http->getString(); 
            Http->end();
            char*  cBuf = (char*)malloc(sizeof(char) * sPayload.length() + sizeof(char));
            if(cBuf == NULL){              
              return NULL;
            }
            sPayload.toCharArray(cBuf, sPayload.length() + 1);
            sPayload= "";
            return cBuf;
        }
    } else {
        if(bEnableDebug){
            Serial.println("[HTTP] GET... failed, error: %s\n");
            Serial.println(iHttpCode, DEC);
            Serial.println(Http->errorToString(iHttpCode).c_str());              
        }
    }
    Http->end();
    delete Http;
    return NULL;
}


/*
 * Function :constructor for MLABDB
 * Parameter: 3x char*, char* endPoint = mlabdb endpoint base url, char* key = api key, char* sslfp = ssl fingerprint incase ssl would be used, currently ESP is not compatible with the tls/ssl protocol used by mlab
 */
MLABDB::MLABDB(char*   cEndPoint, char*   cKey, char*   cSslFp)
{ 
    cApiEndPoint = cEndPoint;
    cApiKey = cKey;
    cSslFingerPrint = cSslFp;  
}

/*
 * Function :to get time and date from Http://convert-unix-time.com/api?timestamp=now&timezone=amsterdam&format=english12
 * Returns  : char* with date & time
 */
char* MLABDB::GetTimeStamp(){  
    char* cBuf = GenerateHttpRequest(0, "convert-unix-time.com", "/api?timestamp=now&timezone=amsterdam&format=english12", NULL);  
    if(bEnableDebug){      
        Serial.println(cBuf);
    }
    if(cBuf == NULL){
      return NULL;
    }
    SimpleJson *TimeStampJson = parse_SimpleJson(400,cBuf);
    FreeCharDB(cBuf);
    if (TimeStampJson != NULL) {
        char* date =  TimeStampJson->GetFieldWithString("localDate", TimeStampJson);
        FreeSimpleJson(TimeStampJson);
        if(date == NULL){
          return NULL;
        }
        return date;
   
    } else {
       if(bEnableDebug){      
          Serial.println("COULD NOT PARSE JSON:(");
          Serial.println(cBuf);
       }
       return NULL;
    } 
    return NULL;
  
}

/*
 * Function :to get available cDataBases
 * Returns  : char* with cDataBase (cJson)array
 */
char* MLABDB::GetDatabases()
{   
    char*   cFullQuery = CombineStringDB("/api/1/cDataBases?apiKey=",cApiKey);
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2);  
    if(cFullUrl == NULL){
      return NULL; 
    }  
    char* cResponse = GenerateHttpRequest(0, "cors-anywhere.herokuapp.com", cFullUrl, NULL);   
    FreeCharDB(cFullUrl);   
     if(bEnableDebug){      
      Serial.println("requesting cDataBases with the following url:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }
    if(!cResponse || cResponse == NULL){  
      return NULL;
    } else {     
      return cResponse;
    }
}

/*
 * Function :to get available cCollection in cDataBase
 * Parameter:char*, char* cDataBase = string with cDataBase name
 * Returns  : char* with cCollection (cJson)array
 */
char* MLABDB::GetCollections(char*   cDataBase)
{
    char*   cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*   cPart2 =  CombineStringDB(cPart, "/cCollection");    
    FreeCharDB(cPart);
    char*   cPart3 =  CombineStringDB(cPart2, "?apiKey=");
    FreeCharDB(cPart2);
    char*   cFullQuery = CombineStringDB(cPart3,cApiKey);
    FreeCharDB(cPart3);
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2);  
    if(cFullUrl == NULL){
      return NULL; 
    }      
    char*   cResponse = GenerateHttpRequest(0, "cors-anywhere.herokuapp.com", cFullUrl, NULL);   
    FreeCharDB(cFullUrl);

    if(bEnableDebug){      
      Serial.println("requesting cCollection in cDataBase with the following url:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }
    if(!cResponse || cResponse == NULL){   
      return NULL;
    } else {
      return cResponse;
    }
}

/*
 * Function :to get available (cJson)documents in cDataBase
 * Parameter: 2x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name
 * Returns  : char* with cCollection (cJson)array
 */
char* MLABDB::GetDocuments(char*   cDataBase, char*   cCollection)
{
    char*   cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*   cPart2 =  CombineStringDB(cPart, "/collections/");
    FreeCharDB(cPart);
    char*   cPart3 =  CombineStringDB(cPart2, cCollection);
    FreeCharDB(cPart2);
    char*   cPart4 =  CombineStringDB(cPart3, "?apiKey=");
    FreeCharDB(cPart3);
    char*   cFullQuery = CombineStringDB(cPart4,cApiKey);
    FreeCharDB(cPart4);    
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2);  
    if(cFullUrl == NULL){
      return NULL; 
    }          
    char*   cResponse = GenerateHttpRequest(0, "cors-anywhere.herokuapp.com", cFullUrl, NULL);   
    FreeCharDB(cFullUrl);    
    if(bEnableDebug){      
      Serial.println("requesting cCollection in cDataBase with the following url:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }
    if(!cResponse || cResponse == NULL){        
      return NULL;
    } else {
      return cResponse;
    }
}

/*
 * Function :to get (cJson)document in cDataBase
 * Parameter: 3x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name, char* cId = cId of document you want
 * Returns  : char* with document cJson
 */
char*  MLABDB::GetDocument(char*   cDataBase, char*   cCollection, char*   cId)
{
    char*   cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*   cPart2 =  CombineStringDB(cPart, "/collections/");    
    FreeCharDB(cPart);
    char*   cPart3 =  CombineStringDB(cPart2, cCollection); 
    FreeCharDB(cPart2);   
    char*   cPart4 =  CombineStringDB(cPart3, "/");  
    FreeCharDB(cPart3);  
    char*   cPart5 =  CombineStringDB(cPart4, cId); 
    FreeCharDB(cPart4);  
    char*   cPart6 =  CombineStringDB(cPart5, "?apiKey="); 
    FreeCharDB(cPart5);
    char*   cFullQuery = CombineStringDB(cPart6,cApiKey);
    FreeCharDB(cPart6);
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2); 
    if(cFullUrl == NULL){
      return NULL; 
    }          
    char*   cResponse = GenerateHttpRequest(0, "cors-anywhere.herokuapp.com", cFullUrl, NULL);   
    FreeCharDB(cFullUrl);     
    if(bEnableDebug){      
      Serial.println("requesting document with the following url:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }
    if(!cResponse || cResponse == NULL){     
      return NULL;
    } else {
      return cResponse;
    }
  
}

/*
 * Function :to upload(cJson)document in cDataBase
 * Parameter: 3x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name, char* cJson = to insert into document and upload as document
 * Returns  : uint8_t with value 1 on succes, and 0 on error/fail
 */
uint8_t  MLABDB::UploadDocument(char*  cDataBase, char*  cCollection, char* cJson)
{
    
    char*  cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*  cPart2 =  CombineStringDB(cPart, "/collections/");
    FreeCharDB(cPart);
    char*  cPart3 =  CombineStringDB(cPart2, cCollection);  
    FreeCharDB(cPart2);
    char*  cPart4 =  CombineStringDB(cPart3, "?apiKey=");
    FreeCharDB(cPart3);
    char*  cFullQuery = CombineStringDB(cPart4,cApiKey);
    FreeCharDB(cPart4);    
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2); 
    if(cFullUrl == NULL){
      return NULL; 
    }       
    char* cResponse = GenerateHttpRequest(1, "cors-anywhere.herokuapp.com", cFullUrl, cJson);   
    FreeCharDB(cFullUrl);    
    if(bEnableDebug){      
      Serial.println("The following json document has been generated:");
      Serial.println(cJson);
      Serial.println("uploading json document to following endpoint:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    } 
    if(cResponse == NULL){
      if(bEnableDebug){      
        Serial.println("COULD NOT UPLOAD DOCUMENT :(");
      } 
      return 0;
    }
    FreeCharDB(cResponse);
    return 1;  
}

/*
 * Function : to update(cJson)document in cDataBase
 * Parameter: 4x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name, , char* cId = cId of document you want to update, char* cJson = cJson to update in document
 * Returns  : uint8_t with value 1 on succes, and 0 on error/fail
 */
uint8_t  MLABDB::UpdateDocument(char*  cDataBase, char*  cCollection,char*  cId, char* cJson)
{
    char*  cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*  cPart2 =  CombineStringDB(cPart, "/collections/");
    FreeCharDB(cPart);
    char*  cPart3 =  CombineStringDB(cPart2, cCollection);  
    FreeCharDB(cPart2);  
    char*  cPart4 =  CombineStringDB(cPart3, "/");
    FreeCharDB(cPart3);    
    char*  cPart5 =  CombineStringDB(cPart4, cId);
    FreeCharDB(cPart4); 
    char*  cPart6 =  CombineStringDB(cPart5, "?apiKey=");
    FreeCharDB(cPart5);
    char*  cFullQuery = CombineStringDB(cPart6,cApiKey); 
    FreeCharDB(cPart6);
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2); 
    if(cFullUrl == NULL){
      return NULL; 
    } 
    char*  cResponse = GenerateHttpRequest(2, "cors-anywhere.herokuapp.com", cFullUrl, cJson);   
     FreeCharDB(cFullUrl);
    if(bEnableDebug){      
      Serial.println("The following cJson document has been generated:");
      Serial.println(cJson);
      Serial.println("uploading cJson document to following endpoint:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }     
    if(cResponse == NULL){
      if(bEnableDebug){      
        Serial.println("COULD NOT UPDATE DOCUMENT :(");
      } 
      return 0;
    }
     FreeCharDB(cResponse);
    return 1;
  
}

/*
 * Function : to upsert field in cJson(cJson)document in cDataBase
 * Parameter: 4x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name, , char* cId = cId of document you want to upsert, char* cJson = cJson to upsert in document
 * Returns  : uint8_t with value 1 on succes, and 0 on error/fail
 */
uint8_t  MLABDB::UpsertFieldInDocument(char*  cDataBase, char*  cCollection, char*  cId, char* cJson)
{
    char*  cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*  cPart2 =  CombineStringDB(cPart, "/collections/");   
    FreeCharDB(cPart);
    char*  cPart3 =  CombineStringDB(cPart2, cCollection); 
    FreeCharDB(cPart2);   
    char*  cPart4 =  CombineStringDB(cPart3, "/");  
    FreeCharDB(cPart3);  
    char*  cPart5 =  CombineStringDB(cPart4, cId);
    FreeCharDB(cPart4);
    char*  cPart6 =  CombineStringDB(cPart5, "?apiKey=");
    FreeCharDB(cPart5);
    char*  cPart7 = CombineStringDB(cPart6,cApiKey);  
    FreeCharDB(cPart6);  
    char*  cFullQuery = CombineStringDB(cPart7, "&u=true");
    FreeCharDB(cPart7);
    char*   cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*   cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2); 
    if(cFullUrl == NULL){
      return NULL; 
    }       
    char*  cJsonPart = CombineStringDB("{\"$set\":", cJson);
    char*  cJsonToSend = CombineStringDB(cJsonPart, "}");
    FreeCharDB(cJsonPart);
    if(cJsonToSend  == NULL){
      return NULL; 
    }          
    char* cResponse = GenerateHttpRequest(2, "cors-anywhere.herokuapp.com", cFullUrl, cJsonToSend); 
    FreeCharDB(cJsonToSend);

    if(bEnableDebug){      
      Serial.println("The following cJson document has been generated:");
      Serial.println(cJsonToSend);
      Serial.println("uploading cJson document to following endpoint:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    } 
    if(cResponse == NULL){
      return NULL;
    }
  
    FreeCharDB(cResponse);
    return 1;
  
}

/*
 * Function : to delete (cJson)document in cDataBase
 * Parameter: 3x char*, char* cDataBase = string with cDataBase name, char* collection = string with collection name, , char* cId = cId of document you want to delete
 * Returns  : uint8_t with value 1 on succes, and 0 on error/fail
 */
uint8_t  MLABDB::DeleteDocument(char*  cDataBase, char*  cCollection, char*  cId)
{
    char*  cPart = CombineStringDB("/api/1/databases/", cDataBase);
    char*  cPart2 =  CombineStringDB(cPart, "/collections/");
    FreeCharDB(cPart);
    char*  cPart3 =  CombineStringDB(cPart2, cCollection); 
    FreeCharDB(cPart2);   
    char*  cPart4 =  CombineStringDB(cPart3, "/");  
    FreeCharDB(cPart3);  
    char*  cPart5 =  CombineStringDB(cPart4, cId);
    FreeCharDB(cPart4); 
    char*  cPart6 =  CombineStringDB(cPart5, "?apiKey=");
    FreeCharDB(cPart5);
    char*  cFullQuery = CombineStringDB(cPart6,cApiKey);   
    FreeCharDB(cPart6);
    char*  cFullQuery2 = CombineStringDB(cApiEndPoint, cFullQuery);  
    FreeCharDB(cFullQuery); 
    char*  cFullUrl =  CombineStringDB("/", cFullQuery2); 
    FreeCharDB(cFullQuery2); 
    if(cFullUrl == NULL){
      return NULL; 
    }        
    char*  cResponse = GenerateHttpRequest(3, "cors-anywhere.herokuapp.com", cFullUrl, NULL); 
    FreeCharDB(cFullUrl);
    if(bEnableDebug){      
      Serial.println("Deleting json document with  following endpoint:");
      Serial.println(cFullUrl);
      Serial.println("response:");
      Serial.println(cResponse);
    }   
    if(cResponse == NULL){
      if(bEnableDebug){      
        Serial.println("COULD NOT UPLOAD DOCUMENT :(");
      } 
      return 0;
    }
    FreeCharDB(cResponse); 
    return 1;
}

/*
 * Function : to enable debugging messages on ESP
 * Parameter: 1x uint8_t, uint8_t bEnDisable = enable or disable value/boolean (1/0)
 */
void MLABDB::EnableDebugging(uint8_t bEnDisable)
{
  bEnableDebug = bEnDisable;
}


