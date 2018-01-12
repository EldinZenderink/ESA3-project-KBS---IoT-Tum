#pragma once

/*
    mlabdb.h
    Purpose: Setting up the mlabdb library

    @author Eldin Zenderink
    @version 1.0 01/01/18
*/

#ifndef MLABDB_H
#define MLABDB_H


/******************************************************************************
  Includes
******************************************************************************/
#include "Arduino.h"
#include <ESP8266HTTPClient.h>
#include "SimpleJson.h"

/******************************************************************************
  Class
******************************************************************************/
class MLABDB{
  public:
    MLABDB(char* cEndPoint, char* cKey, char* cSslFp);
    char* GetTimeStamp();
    char* GetDatabases();
    char* GetCollections(char* cDataBase);
    char* GetDocuments(char* cDataBase, char* cCollection);
    char* GetDocument(char* cDataBase, char* cCollection, char* cId);
    uint8_t UploadDocument(char* cDataBase, char* cCollection, char* cJson);
    uint8_t UpdateDocument(char* cDataBase, char* collection, char* cId, char* cJson);
    uint8_t UpsertFieldInDocument(char* cDataBase, char* cCollection, char* cId, char* cJson);
    uint8_t DeleteDocument(char* cDataBase, char* cCollection, char* cId);
    void EnableDebugging(uint8_t bEnDisable);
  private:
  
    uint8_t bEnableDebug;
    char* cHost;
    char* cApikey;
    char* cApiEndPoint;
    char* cSslFingerPrint;
    
};

#endif //MLABDB_H
