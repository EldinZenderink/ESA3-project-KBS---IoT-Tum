
#ifndef MLABDB_H
#define MLABDB_H
#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

typedef struct CharArray{
  char* string;  
}CharArray;

class MLABDB{
  public:
    MLABDB(char* endPoint, char* key, char* sslfp);
    JsonArray* getDatabase();
    JsonArray* getColections(char* database);
    JsonArray* getDocuments(char* database, char* collections);
    JsonObject* getDocument(char* database, char* collections, char* id);
    uint8_t uploadDocument(char* database, char* collections, JsonObject* json);
    uint8_t  updateDocument(char* database, char* collection, char* id, JsonObject* json);
    uint8_t  updateFieldInDocument(char* database, char* collections, char* id, JsonObject* json);
    uint8_t  deleteDocument(char* database, char* collections, char* id);
    void enableDebugging(uint8_t endisable);
  private:
  
    uint8_t enableDebug;
    char* host;
    char* apikey;
    char* apiEndPoint;
    char* sslfingerprint;
    char* combineString(char* str1, char* str2);
    char* generateGetRequest(char* url);
    char* generatePostRequest(char* url, char* body);
    char* generatePutRequest(char* url, char* body);
    char* generateDeleteRequest(char* url);
    
};

#endif //MLABDB_H
