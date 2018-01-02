#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "mlabdb.h"


char*   apiKey;
char*   apiEndPoint;
char*   sslfingerpint;
uint8_t enableDebug = 0;



char*   MLABDB::combineString(char*   str1, char*   str2)
{
    // Determine new size
   int newSize = strlen(str1)  + strlen(str2) + 1; 

   // Allocate new buffer
   char * newBuffer = (char *)malloc(newSize);

   // do the copy and concat
   strcpy(newBuffer,str1);
   strcat(newBuffer,str2); // or strncat

   // store new pointer
   return newBuffer;
  
}

char*   MLABDB::generateGetRequest(char*   url)
{

    HTTPClient http;
    http.begin("http://cors-anywhere.herokuapp.com/" + String(url)); //HTTPS
    http.addHeader("X-Requested-With", "ESP8266-TUM");
    int httpCode = http.GET();
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(enableDebug){
          
          Serial.println("[HTTP] GET... code: \n");
          Serial.print( httpCode, DEC);

        }
        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            
            http.end();
            char*   buf = (char*)malloc(sizeof(char) * payload.length() + 1);
            if(!buf){
              return NULL;
            }
            payload.toCharArray(buf, payload.length() + 1);
            return buf;
        }
    } else {
        if(enableDebug){
            Serial.println("[HTTP] GET... failed, error: %s\n");
            Serial.println(httpCode, DEC);
            Serial.println(http.errorToString(httpCode).c_str());              
        }
    }

    http.end();
    return NULL;
  
}

char*   MLABDB::generatePostRequest(char*   url, char*   body)
{


    HTTPClient http;
    http.begin("http://cors-anywhere.herokuapp.com/" + String(url)); //HTTPS
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Requested-With", "ESP8266-TUM");
    int httpCode = http.POST(body);
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(enableDebug){
          
          Serial.println("[HTTP] POST... code: \n");
          Serial.print( httpCode, DEC);

        }
        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            
            http.end();
            char*   buf = (char*)malloc(sizeof(char) * payload.length() + 1);
            if(!buf){
              return NULL;
            }
            payload.toCharArray(buf, payload.length() + 1);
            return buf;
        }
    } else {
        if(enableDebug){
            Serial.println("[HTTP] GET... failed, error: %s\n");
            Serial.println(httpCode, DEC);
            Serial.println(http.errorToString(httpCode).c_str());              
        }
    }

    http.end();
    return NULL;
}

char*   MLABDB::generatePutRequest(char*   url, char*   body)
{


    HTTPClient http;
    http.begin("http://cors-anywhere.herokuapp.com/" + String(url)); //HTTPS
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Requested-With", "ESP8266-TUM");
    //int httpCode = http.POST(body);
    int httpCode = (int) http.sendRequest("PUT", body);
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(enableDebug){
          
          Serial.println("[HTTP] POST... code: \n");
          Serial.print( httpCode, DEC);

        }
        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            
            http.end();
            char*   buf = (char*)malloc(sizeof(char) * payload.length() + 1);
            if(!buf){
              return NULL;
            }
            payload.toCharArray(buf, payload.length() + 1);
            return buf;
        }
    } else {
        if(enableDebug){
            Serial.println("[HTTP] GET... failed, error: %s\n");
            Serial.println(httpCode, DEC);
            Serial.println(http.errorToString(httpCode).c_str());              
        }
    }

    http.end();
    return NULL;
}

char*   MLABDB::generateDeleteRequest(char*   url)
{

    HTTPClient http;
    http.begin("http://cors-anywhere.herokuapp.com/" + String(url)); //HTTPS
    http.addHeader("X-Requested-With", "ESP8266-TUM");
    int httpCode =http.sendRequest("DELETE");
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(enableDebug){
          
          Serial.println("[HTTP] GET... code: \n");
          Serial.print( httpCode, DEC);

        }
        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            String payload = http.getString();
            
            
            http.end();
            char*   buf = (char*)malloc(sizeof(char) * payload.length() + 1);
            if(!buf){
              return NULL;
            }
            payload.toCharArray(buf, payload.length() + 1);
            return buf;
        }
    } else {
        if(enableDebug){
            Serial.println("[HTTP] GET... failed, error: %s\n");
            Serial.println(httpCode, DEC);
            Serial.println(http.errorToString(httpCode).c_str());              
        }
    }

    http.end();
    return NULL;
  
}

MLABDB::MLABDB(char*   endPoint, char*   key, char*   sslfp)
{ 
    apiEndPoint = endPoint;
    apiKey = key;
   sslfingerprint = sslfp;
  
}

JsonArray* MLABDB::getDatabase()
{   
  
   // DynamicJsonBuffer jsonBuffer(100);
    char*   fullquery = MLABDB::combineString("/api/1/databases?apiKey=",apiKey); 
    char*   fullurl = MLABDB::combineString(apiEndPoint, fullquery);    
    char*   dbs = MLABDB::generateGetRequest(fullurl);
     if(enableDebug){      
      Serial.println("requesting databases with the following url:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    }
    if(!dbs){      
      return NULL;
    } else {
      /* JsonObject& root = jsonBuffer.parseObject(String(dbs));
      if (root.success()) {
        if(root["success"] == true){
            const char*   resultjson = root["body"]; 
            DynamicJsonBuffer resultBuffer(100);
            JsonArray& toreturn = resultBuffer.parseArray(resultjson);
            free(fullquery);       
            free(fullurl);
            free(dbs);
            return &toreturn;
        } else {
            if(enableDebug){      
               Serial.println("Proxy Parsing Failed");
            }
            free(fullquery);       
            free(fullurl);
            free(dbs);
            return NULL;
        }
       
      } else {
         if(enableDebug){      
            Serial.println("COULD NOT PARSE JSON :(");
         }
      } */
      DynamicJsonBuffer jsonBuffer(100);
      JsonArray& root = jsonBuffer.parseArray(String(dbs));
      if (root.success()) {
          free(fullquery);       
          free(fullurl);
          free(dbs);
          return &root;
     
      } else {
         if(enableDebug){      
            Serial.println("COULD NOT PARSE ARRAY :(");
            Serial.println(dbs);
         }
      } 
    }
   
    free(fullquery);       
    free(fullurl);
    free(dbs);

    return NULL;
  
}

JsonArray* MLABDB::getColections(char*   database)
{
   
    //DynamicJsonBuffer jsonBuffer(100);
    char*   partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*   partofquery2 =  MLABDB::combineString(partofquery, "/collections");
    char*   partofquery3 =  MLABDB::combineString(partofquery2, "?apiKey=");
    char*   fullquery = MLABDB::combineString(partofquery3,apiKey);    
    char*   fullurl = MLABDB::combineString(apiEndPoint, fullquery);    
    char*   dbs = MLABDB::generateGetRequest(fullurl);

    if(enableDebug){      
      Serial.println("requesting collections in database with the following url:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    }
    if(!dbs){      
      return NULL;
    } else {
      DynamicJsonBuffer jsonBuffer(100);
      JsonArray& root = jsonBuffer.parseArray(String(dbs));
      if (root.success()) {
          free(partofquery);
          free(partofquery2);
          free(partofquery3);
          free(fullquery);       
          free(fullurl);
          free(dbs);
          return &root;
     
      } else {
         if(enableDebug){      
            Serial.println("COULD NOT PARSE ARRAY :(");
            Serial.println(dbs);
         }
      } 
    }
    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(fullquery);       
    free(fullurl);
    free(dbs);

    return NULL;
  
}

JsonArray* MLABDB::getDocuments(char*   database, char*   collection)
{

    //DynamicJsonBuffer jsonBuffer(100);
    char*   partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*   partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*   partofquery3 =  MLABDB::combineString(partofquery2, collection);
    char*   partofquery4 =  MLABDB::combineString(partofquery3, "?apiKey=");
    char*   fullquery = MLABDB::combineString(partofquery4,apiKey);    
    char*   fullurl = MLABDB::combineString(apiEndPoint, fullquery);    
    char*   dbs = MLABDB::generateGetRequest(fullurl);

    
    if(enableDebug){      
      Serial.println("requesting collections in database with the following url:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    }
    if(!dbs){      
      return NULL;
    } else {
        DynamicJsonBuffer jsonBuffer(100);
        JsonArray& root = jsonBuffer.parseArray(String(dbs));
        if (root.success()) {
             free(partofquery);
            free(partofquery2);
            free(partofquery3);
            free(partofquery4);
            free(fullquery);       
            free(fullurl);
            free(dbs);
            return &root;
       
        } else {
           if(enableDebug){      
              Serial.println("COULD NOT PARSE ARRAY :(");
              Serial.println(dbs);
           }
        } 
    }
    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery4);
    free(fullquery);       
    free(fullurl);
    free(dbs);

    return NULL;
}

JsonObject*  MLABDB::getDocument(char*   database, char*   collections, char*   id)
{
   

    //DynamicJsonBuffer jsonBuffer;
    char*   partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*   partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*   partofquery3 =  MLABDB::combineString(partofquery2, collections);    
    char*   partofquery4 =  MLABDB::combineString(partofquery3, "/");    
    char*   partofquery5 =  MLABDB::combineString(partofquery4, id);
    char*   partofquery6 =  MLABDB::combineString(partofquery5, "?apiKey=");
    char*   fullquery = MLABDB::combineString(partofquery6,apiKey);    
    char*   fullurl = MLABDB::combineString(apiEndPoint, fullquery);    
    char*   dbs = MLABDB::generateGetRequest(fullurl);
    
    if(enableDebug){      
      Serial.println("requesting document with the following url:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    }
    if(!dbs){      
      return NULL;
    } else {
      DynamicJsonBuffer jsonBuffer(100);
      JsonObject& root = jsonBuffer.parseObject(String(dbs));
      if (root.success()) {
          free(partofquery);
          free(partofquery2);
          free(partofquery3);
          free(partofquery4);   
          free(partofquery5);
          free(partofquery6);
          free(fullquery);       
          free(fullurl);
          free(dbs);
          return &root;
     
      } else {
         if(enableDebug){      
            Serial.println("COULD NOT PARSE ARRAY :(");
            Serial.println(dbs);
         }
      } 
    }
    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery4);    
    free(partofquery5);
    free(partofquery6);
    free(fullquery);       
    free(fullurl);
    free(dbs);
    return NULL;
  
}

uint8_t  MLABDB::uploadDocument(char*  database, char*  collections, JsonObject* json)
{
    
    char*  partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*  partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*  partofquery3 =  MLABDB::combineString(partofquery2, collections);  
    char*  partofquery6 =  MLABDB::combineString(partofquery3, "?apiKey=");
    char*  fullquery = MLABDB::combineString(partofquery6,apiKey);    
    char*  fullurl = MLABDB::combineString(apiEndPoint, fullquery); 
    String jsonstr;
    json->printTo(jsonstr);   
    char*  jsonstring = (char*) malloc(sizeof(char) * jsonstr.length() + 1);
    jsonstr.toCharArray(jsonstring, jsonstr.length() + 1);
    char*  dbs = MLABDB::generatePostRequest(fullurl, jsonstring);

    if(enableDebug){      
      Serial.println("The following json document has been generated:");
      Serial.println(jsonstring);
      Serial.println("uploading json document to following endpoint:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    } 

    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery6);
    free(fullquery);       
    free(fullurl);
    free(jsonstring);
    free(dbs);

    return 1;
  
}

uint8_t  MLABDB::updateDocument(char*  database, char*  collections,char*  id, JsonObject* json)
{

    char*  partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*  partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*  partofquery3 =  MLABDB::combineString(partofquery2, collections);    
    char*  partofquery4 =  MLABDB::combineString(partofquery3, "/");    
    char*  partofquery5 =  MLABDB::combineString(partofquery4, id);
    char*  partofquery6 =  MLABDB::combineString(partofquery5, "?apiKey=");
    char*  fullquery = MLABDB::combineString(partofquery6,apiKey);    
    char*  fullurl = MLABDB::combineString(apiEndPoint, fullquery); 
    String jsonstr;
    json->printTo(jsonstr);   
    char*  jsonstring = (char*) malloc(sizeof(char) * jsonstr.length() + 1);
    jsonstr.toCharArray(jsonstring, jsonstr.length() + 1);

    
    char*  dbs = MLABDB::generatePutRequest(fullurl, jsonstring);

    if(enableDebug){      
      Serial.println("The following json document has been generated:");
      Serial.println(jsonstring);
      Serial.println("uploading json document to following endpoint:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    } 

    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery4);    
    free(partofquery5);
    free(partofquery6);
    free(fullquery);       
    free(fullurl);
    free(jsonstring);
    free(dbs);

    return 1;
  
}

uint8_t  MLABDB::updateFieldInDocument(char*  database, char*  collections, char*  id, JsonObject* json)
{
    char*  partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*  partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*  partofquery3 =  MLABDB::combineString(partofquery2, collections);    
    char*  partofquery4 =  MLABDB::combineString(partofquery3, "/");    
    char*  partofquery5 =  MLABDB::combineString(partofquery4, id);
    char*  partofquery6 =  MLABDB::combineString(partofquery5, "?apiKey=");
    char*  fullquery = MLABDB::combineString(partofquery6,apiKey);    
    char*  fullurl = MLABDB::combineString(apiEndPoint, fullquery); 
    String jsonstr;
    json->printTo(jsonstr);   
    char*  jsonstring = (char*) malloc(sizeof(char) * jsonstr.length() + 1);
    jsonstr.toCharArray(jsonstring, jsonstr.length() + 1);

    char*  partofjsontosend = MLABDB::combineString("{\"$set\":", jsonstring);
    char*  fulljsontosend = MLABDB::combineString(partofjsontosend, "}");
    
    
    char*  dbs = MLABDB::generatePutRequest(fullurl, fulljsontosend);

    if(enableDebug){      
      Serial.println("The following json document has been generated:");
      Serial.println(fulljsontosend);
      Serial.println("uploading json document to following endpoint:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    } 

    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery4);    
    free(partofquery5);
    free(partofquery6);
    free(fullquery);       
    free(fullurl);
    free(jsonstring);
    free(partofjsontosend);
    free(fulljsontosend);
    free(dbs);

    return 1;
  
}

uint8_t  MLABDB::deleteDocument(char*  database, char*  collections, char*  id)
{
    char*  partofquery = MLABDB::combineString("/api/1/databases/", database);
    char*  partofquery2 =  MLABDB::combineString(partofquery, "/collections/");
    char*  partofquery3 =  MLABDB::combineString(partofquery2, collections);    
    char*  partofquery4 =  MLABDB::combineString(partofquery3, "/");    
    char*  partofquery5 =  MLABDB::combineString(partofquery4, id);
    char*  partofquery6 =  MLABDB::combineString(partofquery5, "?apiKey=");
    char*  fullquery = MLABDB::combineString(partofquery6,apiKey);    
    char*  fullurl = MLABDB::combineString(apiEndPoint, fullquery); 
    char*  dbs = MLABDB::generateDeleteRequest(fullurl);

    if(enableDebug){      
      Serial.println("Deleting json document with  following endpoint:");
      Serial.println(fullurl);
      Serial.println("response:");
      Serial.println(dbs);
    } 
    free(partofquery);
    free(partofquery2);
    free(partofquery3);
    free(partofquery4);    
    free(partofquery5);
    free(partofquery6);
    free(fullquery);       
    free(fullurl);
    free(dbs); 
    return 1;
}

void MLABDB::enableDebugging(uint8_t endisable)
{
  enableDebug = endisable;
}


