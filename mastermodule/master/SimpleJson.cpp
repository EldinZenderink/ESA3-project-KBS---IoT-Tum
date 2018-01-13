/*
    SimpleJson.c
    Purpose: Doing Json the sensible way ;) (which is: my way :))

    @author Eldin Zenderink
    @version 1.0 05/01/18
*/
#include "SimpleJson.h"


/******************************************************************************
  Local Function Prototypes
******************************************************************************/

/*
 * Function : To print the free heap together with a unique identifier.
 * Parameter: char pointer contianing unique identifier
 */
void GetHeap(char* cLine){
    Serial.print("line: ");
    Serial.print(cLine);
    Serial.print(", heap: ");
    Serial.print(ESP.getFreeHeap(), DEC);
    Serial.println();
}

/*
 * Function : To fully free a poiner and leave no dangeling pointers behind.
 * Parameter: char pointer 
 */
void UltimateFree(void* vPtr) {
    free(vPtr);
    vPtr = NULL;
    return;
}

/*
 * Function : To cobine two char strings together into one
 * Parameter: 2x char pointer which will be cPartd 
 * Returns: char pointer pointing to cPartd string
 */
char*  CombineStringJson(char*   cStr1, char*   cStr2)
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
 * Function : to add a cField including value to an existing Json string
 * Parameter: 2x char pointer which will be cPartd 
 * Returns  : char pointer pointing to Json string
 */
char* AddToJson(char*cJson, char* cJsonPart){
    if(cJsonPart != NULL && cJson != NULL){ 
      uint16_t iEndIndex = strlen(cJson) - 1;    

      if(iEndIndex > 60000){
        return NULL;
      }
      char* cTempdata = (char*)calloc(strlen(cJsonPart) + iEndIndex + 2,sizeof(char));

      
      if(cTempdata == NULL){
        return NULL;
      }
      memset(cTempdata, '\0', strlen(cJsonPart) + iEndIndex + 2);
      strncpy (cTempdata,cJson, iEndIndex);
      char* cTempJson;
      if(iEndIndex > 1){
         char* cPart1 = CombineStringJson(cTempdata, ",");
         UltimateFree((void*)cTempdata); 
         char* cPart2 = CombineStringJson(cPart1, cJsonPart);
         UltimateFree((void*)cPart1);
         cTempJson =  CombineStringJson(cPart2,"}\0");
         UltimateFree((void*)cPart2);
         
      } else {
         char* cPart1 = CombineStringJson(cTempdata, cJsonPart);   
         UltimateFree((void*)cTempdata);
         cTempJson =  CombineStringJson(cPart1,"}\0");
         UltimateFree((void*)cPart1);
         
      }          
      return cTempJson;
      
    } else {      
      return NULL;
    }

}


/*
 * Function : tries to find a field in Json and returns the value behind it (do note, this is for integers, floats, booleans and strings, but not complete Objects nor arrays)
 * Parameter: 2x char pointer, char* cJson = Json string, char* cField = field to be found in Json string
 * Returns  : Value behind cField as char*, or NULL if it does not exist or some error occured
 */
char* GetValueFromJson(char* cJson, char* cField){
    if(cField != NULL &&cJson != NULL){ 
      char *cPointer = strstr(cJson, cField);
      if(cPointer) {
         uint16_t iIndex = cPointer -cJson;
         uint16_t iSearchIndex;         
         uint16_t cToReturnSize = strlen(cJson);
         uint16_t iWriteIndex = 0;
         uint8_t bFoundStartIndex = 0;
         char* cToReturn = (char*)calloc(cToReturnSize + 1, sizeof(char));
         for(iSearchIndex = iIndex; iSearchIndex < strlen(cJson); iSearchIndex++){
            if(cJson[iSearchIndex] == ':'){
               bFoundStartIndex = 1;     
            }

            if(bFoundStartIndex){
              if(cJson[iSearchIndex] == ',' ||cJson[iSearchIndex] == '}' ){                
                  cToReturn[iWriteIndex] = '\0';
                  return cToReturn;
               }
               if(iWriteIndex >= cToReturnSize - 1){
                  cToReturnSize = cToReturnSize + 100;
                  cToReturn = (char*) realloc(cToReturn, (cToReturnSize  * sizeof(char)) + sizeof(char));
                  if(cToReturn == NULL){
                    return NULL;
                  }
               }
               if(cJson[iSearchIndex] != '"' &&cJson[iSearchIndex] != ':'){
                 cToReturn[iWriteIndex] =cJson[iSearchIndex];
                 iWriteIndex++;                
               }
            }
         }
         UltimateFree((void*)cToReturn);
         return NULL;
         
      } else {
         return NULL;
      }
    } else {     
      return NULL;
    }
}


/*
 * Function : tries to find a field inc Json and returns the value behind it (do note, this is for Json Objects only)
 * Parameter: 2x char pointer, char* cJson = Json string, char* cField = field to be found in Json string
 * Returns  : Value behind cField as char*, or NULL if it does not exist or some error occured
 */
char* GetObjectFromJson(char*cJson, char* cField){
    if(cField != NULL &&cJson != NULL && strstr(cJson, cField) != NULL){         
      char *cPointer = strstr(cJson, cField);
      if(cPointer) {
         uint16_t iIndex = cPointer -cJson;
         uint16_t  iSearchIndex;         
         uint16_t cToReturnSize = strlen(cJson);
         uint16_t iWriteIndex = 0;
         uint16_t iCountOpeningBrackets = 0;
         uint16_t iCountClosingBrackets = 0;
         uint8_t bFoundStartIndex = 0;
         char* cToReturn = (char*)calloc(cToReturnSize + 1, sizeof(char));
         for(iSearchIndex = iIndex; iSearchIndex < strlen(cJson); iSearchIndex++){
            if(cJson[iSearchIndex] == ':'){
               bFoundStartIndex = 1;     
            }

            if(bFoundStartIndex){
              
               if(iCountOpeningBrackets != 0 && iCountClosingBrackets != 0 && (iCountOpeningBrackets - iCountClosingBrackets) == 0){
                  cToReturn[iWriteIndex - 1] = '\0';
                  return cToReturn;
               }
               if(cJson[iSearchIndex] == '{'){                
                  iCountOpeningBrackets++;
               }
               if(cJson[iSearchIndex] == '}'){
                  iCountClosingBrackets++;
               }

               if(iWriteIndex == cToReturnSize){
                  cToReturnSize *= 2;
                  cToReturn  = (char*) realloc(cToReturn, (cToReturnSize * sizeof(char)));
                  if(cToReturn == NULL){
                    return NULL;
                  }
               }
               if(iSearchIndex < strlen(cJson)){                
                  cToReturn[iWriteIndex] =cJson[iSearchIndex + 1];
                  iWriteIndex++;  
               } else {             
                  UltimateFree((void*)cToReturn);
                  return NULL;   
               }
            }
         }
         UltimateFree((void*)cToReturn);
         return NULL;
         
      } else {
         return NULL;
      }
      
    } else {    
      return NULL;
    }
}

/*
 * Function : searches for a field in Json string, when found, replaces value behind the field with the current value.(do note, this is for integers, floats, booleans and strings, but not complete Json Objects or Json arrays)
 * Parameter: 3x char pointer, char*cJson = Json string, char* cField = field to be found in Json string, char* value = value to be used to replace value in Json string
 * Returns  : returns new Json string as char*, or NULL incase it doesn't find the field, or an error occurs
 */
char* UpdateValueInJson(char*cJson, char* cField, char* cValue){
    if(cField != NULL &&cJson != NULL){ 
      char *cPointer = strstr(cJson, cField);
      if(cPointer) {
         uint16_t iIndex = cPointer - cJson;         
         uint16_t iSearchIndex;         
         uint16_t cToReturnSize = strlen(cJson);
         uint16_t iWriteIndex = 0;
         uint8_t bFoundStartIndex = 0;
         char* cToReturn = (char*)calloc(cToReturnSize + 1, sizeof(char));
         for(iSearchIndex = iIndex; iSearchIndex < strlen(cJson); iSearchIndex++){
            if(cJson[iSearchIndex] == ':'){
               bFoundStartIndex = 1;     
            }

            if(bFoundStartIndex){
              if(cJson[iSearchIndex] == ',' ||cJson[iSearchIndex] == '}' ){                
                  cToReturn[iSearchIndex] = '\0';
                  break;
               }
               if(iWriteIndex == cToReturnSize){
                  cToReturnSize *= 2;
                  cToReturn  = (char*) realloc(cToReturn, (cToReturnSize * sizeof(char)));
                  if(cToReturn == NULL){
                    return NULL;
                  }
               }
               if(cJson[iSearchIndex] != '"' &&cJson[iSearchIndex] != ':'){
                 cToReturn[iWriteIndex] = cJson[iSearchIndex];
                 iWriteIndex++;                
               }
            }
         }

         uint16_t cEndIndexValue = strlen(cToReturn);
         iIndex = cPointer - cJson;
         char* cTempdata1 = (char*)calloc(iIndex + strlen(cField) + 3, sizeof(char));//+3 to include the : character and to make sure that there is enough length
        
         if(cTempdata1 == NULL){
           return NULL;
         }
         memset(cTempdata1, '\0', iIndex + strlen(cField) + 3); ;//+3 just to make sure that string end and \0 isn't being overwritten
         strncpy (cTempdata1,cJson, iIndex + strlen(cField) + 2);//+2 to include the : character, somehow not reaching till the end, its bad but it works
       
         char* cTempdata2 = (char*)calloc(strlen(cJson) - iSearchIndex + 2, sizeof(char));//+2 just to make sure that there is enough space
         if(cTempdata2 == NULL){
           return NULL;
         }
         memset(cTempdata2, '\0', strlen(cJson) - iSearchIndex + 1);//+1 just to make sure that string end and \0 isn't being overwritten
         strncpy (cTempdata2,cJson + iSearchIndex, strlen(cJson) - iSearchIndex); 
         
         char* cPart1 = CombineStringJson(cTempdata1, cValue);
         UltimateFree((void*)cTempdata1);
         
         char* cToReturnJson = CombineStringJson(cPart1, cTempdata2);

         UltimateFree((void*)cTempdata2); 
         UltimateFree((void*)cPart1);         
         UltimateFree((void*)cToReturn);

         return cToReturnJson;
         
      } else {
         return NULL;
      }
      
    } else {     
      return NULL;
    }
}

/*
 * Function : searches for a field in Json string, when found, replaces Object behind the field with the current Object.(do note, this is Objects ONLY)
 * Parameter: 3x char pointer, char*cJson = Json string, char* cField =  Field to be found in Json string, char* value = value to be used to replace value in Json string
 * Returns  : returns new Json string as char*, or NULL incase it doesn't find the  Field, or an error occurs
 */
char* UpdateObjectInJson(char*cJson, char* cField, char* cValue){
    if(cField != NULL && cJson != NULL){ 
      char *cPointer = strstr(cJson, cField);
      if(cPointer) {
         uint16_t iIndex = cPointer -cJson;
         uint16_t iSearchIndex;         
         uint16_t cToReturnSize = strlen(cJson);
         uint16_t iWriteIndex = 0;
         uint16_t iCountOpeningBrackets = 0;
         uint16_t iCountClosingBrackets = 0;
         uint8_t bFoundStartIndex = 0;         
         char* cToReturn = (char*)calloc(cToReturnSize + 1, sizeof(char));
         for(iSearchIndex = iIndex; iSearchIndex < strlen(cJson); iSearchIndex++){
            if(cJson[iSearchIndex] == ':'){
               bFoundStartIndex = 1;     
            }

            if(bFoundStartIndex){
              
               if(iCountOpeningBrackets != 0 && iCountClosingBrackets != 0 && (iCountOpeningBrackets - iCountClosingBrackets) == 0){
                  cToReturn[iWriteIndex] = '\0';
                  break;
               }
               if(cJson[iSearchIndex] == '{'){                
                  iCountOpeningBrackets++;
               }
               if(cJson[iSearchIndex] == '}'){
                  iCountClosingBrackets++;
               }

               if(iWriteIndex == cToReturnSize){
                  cToReturnSize *= 2;
                  cToReturn  = (char*) realloc(cToReturn, (cToReturnSize * sizeof(char)));
                  if(cToReturn == NULL){
                    return NULL;
                  }
               }
               if(iSearchIndex + 1 < strlen(cJson)){
                
                  cToReturn[iWriteIndex] = cJson[iSearchIndex];
                  iWriteIndex++;  
               } else {                 
                  UltimateFree((void*)cToReturn);
                  return NULL;   
               }
            }
         }
         
         uint16_t cEndIndexValue = strlen(cToReturn);      
         UltimateFree((void*)cToReturn);
         iIndex = cPointer - cJson;
         char* cTempdata1 = (char*)calloc(iIndex + strlen(cField) + 3, sizeof(char));
         if(cTempdata1 == NULL){
           return NULL;
         }
         memset(cTempdata1, '\0', iIndex + strlen(cField) + 3);//+3 just to make sure that string end and \0 isn't being overwritten
         strncpy (cTempdata1,cJson, iIndex + strlen(cField) + 2);//+2 to include the : character, somehow not reaching till the end, its bad but it works
         
         char* cTempdata2 = (char*)calloc(strlen(cJson) - iSearchIndex + 2, sizeof(char));//+2 just to make sure that there is enough space
         if(cTempdata2 == NULL){
           return NULL;
         }
         memset(cTempdata2, '\0', strlen(cJson) - iSearchIndex + 1);//+1 just to make sure that string end and \0 isn't being overwritten
         strncpy (cTempdata2,cJson + iSearchIndex, strlen(cJson) - iSearchIndex); 
         char* cPart1 = CombineStringJson(cTempdata1, cValue);
         UltimateFree((void*)cTempdata1);
         char* cToReturnJson = CombineStringJson(cPart1, cTempdata2);
         

         UltimateFree((void*)cTempdata2); 
         UltimateFree((void*)cPart1);   

         return cToReturnJson;
         
      } else {
         return NULL;
      }
      
    } else {      
      return NULL;
    }
}


/*
 * Function : sets up all the data needed for adding a cField with string to the currentcJson
 * Parameter: 2x char pointer, 1x SimpleJson pointer, char* cField =  Field where it needs to be added, char* value = value that needs to be added, SimpleJson *Json correct struct where current Json resides
 * Returns  : returns 1 on succes, on error
 */
static uint8_t AddFieldWithString(char* cField, char* cValue, SimpleJson *Json)
{ 
  
  char* cPart1 = CombineStringJson("\"", cField);
  char* cPart2 = CombineStringJson(cPart1, "\":\"");
  UltimateFree((void*)cPart1);
  char* cPart3 = CombineStringJson(cPart2, cValue);
  UltimateFree((void*)cPart2);
  char* cJsonPart =  CombineStringJson(cPart3,"\"\0");
  UltimateFree((void*)cPart3);
  char* cReplacedJson= AddToJson(Json->cBuffer, cJsonPart);
  UltimateFree((void*)Json->cBuffer);
  UltimateFree((void*)cJsonPart);
  Json->cBuffer = cReplacedJson;
  if(Json->cBuffer == NULL){
    return 0;
  } 

  return 1;
}

/*
 * Function : sets up all the data needed for adding a cField with intto the currentcJson
 * Parameter: 1x char pointer, 1x cInteger, 1x SimpleJson pointer, char* cField = cField where it needs to be added, int value = value that needs to be added, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t AddFieldWithInt(char* cField, int iValue, SimpleJson *Json)
{
  char* cInteger = (char*)malloc(sizeof(char) * 12);
  sprintf(cInteger, "%d", iValue);
  char* cPart1 = CombineStringJson("\"", cField);
  char* cPart2 = CombineStringJson(cPart1, "\":");
  UltimateFree((void*)cPart1);
  char* cPart3 = CombineStringJson(cPart2, cInteger);
  UltimateFree((void*)cPart2);
  UltimateFree((void*)cInteger);
  char* cJsonPart =  CombineStringJson(cPart3,"\0");  
  UltimateFree((void*)cPart3);  

  if(cJsonPart == NULL){
    return 0;
  }
  char* cReplacedJson= AddToJson(Json->cBuffer, cJsonPart); 
  UltimateFree((void*)Json->cBuffer);  
  UltimateFree((void*)cJsonPart);
  Json->cBuffer = cReplacedJson;    
  if(Json->cBuffer == NULL){
    return 0;
  } 

  return 1;
  
}

/*
 * Function : sets up all the data needed for adding a cField with float to the currentcJson
 * Parameter: 1x char pointer, 1x float, 1x SimpleJson pointer, char* cField = cField where it needs to be added, float value = value that needs to be added, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
//todo fix float issue on ESP
static uint8_t AddFieldWithFloat(char* cField, float fValue, SimpleJson *Json){
  
  char* cFloatString = (char*)malloc(sizeof(char) * 24);  
  dtostrf(fValue, 2, 2, &cFloatString[strlen(cFloatString)]);  
  char* cPart1 = CombineStringJson("\"", cField);
  char* cPart2 = CombineStringJson(cPart1, "\":");
  UltimateFree((void*)cPart1);
  char* cPart3 = CombineStringJson(cPart2, cFloatString);
  UltimateFree((void*)cPart2);
  UltimateFree((void*)cFloatString);
  char* cJsonPart =  CombineStringJson(cPart3,"\0");
  UltimateFree((void*)cPart3);
  if(cJsonPart == NULL){
    return 0;
  }
  char* cReplacedJson= AddToJson(Json->cBuffer, cJsonPart);
  UltimateFree((void*)Json->cBuffer);
  UltimateFree((void*)cJsonPart);
  Json->cBuffer = cReplacedJson;
  if(Json->cBuffer == NULL){
    return 0;
  } 

  return 1;
}

/*
 * Function : sets up all the data needed for adding a cField with boolean to the currentcJson
 * Parameter: 1x char pointer, 1x uint8_t cInteger, 1x SimpleJson pointer, char* cField = cField where it needs to be added, uint8_t value = value that needs to be added(1 = true, 0 = false), SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t AddFieldWithBoolean(char* cField, uint8_t bValue, SimpleJson *Json){
  char* cBooleanToText;
  if(bValue){
    cBooleanToText = (char*)malloc( sizeof(char) * 5);
    strcpy(cBooleanToText, "true\0");
  } else {
    cBooleanToText = (char*)malloc( sizeof(char) * 6);
    strcpy(cBooleanToText, "false\0");
  }
  char* cPart1 = CombineStringJson("\"", cField);
  char* cPart2 = CombineStringJson(cPart1, "\":");
  UltimateFree((void*)cPart1);
  char* cPart3 = CombineStringJson(cPart2, cBooleanToText);
  UltimateFree((void*)cPart2);
  UltimateFree((void*)cBooleanToText);
  char* cJsonPart =  CombineStringJson(cPart3,"\0");
  UltimateFree((void*)cPart3);

  if(cJsonPart == NULL){
    return 0;
  }
  char* cReplacedJson= AddToJson(Json->cBuffer, cJsonPart);
  UltimateFree((void*)Json->cBuffer);
  UltimateFree((void*)cJsonPart);
  Json->cBuffer = cReplacedJson;

  
  if(Json->cBuffer == NULL){
    return 0;
  } 

  return 1;
}

/*
 * Function : sets up all the data needed for adding a cField with Object to the currentcJson
 * Parameter: 1x char pointer, 2x SimpleJson pointer, char* cField = cField where it needs to be added, SimpleObject* Object = Object that needs to be added, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t AddFieldWithObject(char* cField, SimpleJson* Object, SimpleJson *Json)
{
  char* cPart1 = CombineStringJson("\"", cField);
  char* cPart2 = CombineStringJson(cPart1, "\":");
  UltimateFree((void*)cPart1);
  char* cPart3 = CombineStringJson(cPart2, Object->cBuffer);
  UltimateFree((void*)cPart2);
  char* cJsonPart =  CombineStringJson(cPart3,"\0");
  UltimateFree((void*)cPart3);
  if(cJsonPart == NULL){
    return 0;
  }
  char* cReplacedJson = AddToJson(Json->cBuffer, cJsonPart);
  UltimateFree((void*)Json->cBuffer);
  UltimateFree((void*)cJsonPart);
  Json->cBuffer = cReplacedJson;
  if(Json->cBuffer == NULL){
    return 0;
  } 

  return 1;
}

/*
 * Function : sets up all the data needed for replacing a cField with string to the currentcJson
 * Parameter: 2x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to be replaced, char* value = value that needs to be replaced, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, on error
 */
static uint8_t ReplaceFieldWithString(char* cField, char* cValue, SimpleJson *Json)
  {
    char* cPart1 = CombineStringJson("\"", cValue);
    char* cPart2 = CombineStringJson(cPart1, "\"\0");
    UltimateFree((void*)cPart1);
    
    if(cPart2 == NULL){
      return 0;
    }
    char* cTemp = UpdateValueInJson(Json->cBuffer, cField, cPart2);  
    UltimateFree((void*)cPart2);
    if(cTemp == NULL){
      return 0;
    }
    UltimateFree(Json->cBuffer);
    Json->cBuffer = cTemp;
    return 1;
}

/*
 * Function : sets up all the data needed for replacing a cField with int to the currentcJson
 * Parameter: 1x char pointer, 1x cInteger, 1x SimpleJson pointer, char* cField = cField where it needs to be replaced, int value = value that needs to be replaced, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t ReplaceFieldWithInt(char* cField, int iValue, SimpleJson *Json)
{
    char* cInteger = (char*)malloc(sizeof(char) * 12);
    if(cInteger == NULL){
      return 0;
    }
    sprintf(cInteger, "%d", iValue);

    char* cTemp = UpdateValueInJson(Json->cBuffer, cField, cInteger);    
    
    UltimateFree((void*)cInteger);
    if(cTemp == NULL){
      return 0;
    }
    UltimateFree(Json->cBuffer);
    Json->cBuffer = cTemp;
    return 1;
}

/*
 * Function : sets up all the data needed for replacing a cField with float to the currentcJson
 * Parameter: 1x char pointer, 1x float, 1x SimpleJson pointer, char* cField = cField where it needs to be replaced, float value = value that needs to be replaced, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
//todo fix float issue on ESP
static uint8_t ReplaceFieldWithFloat(char* cField, float fValue, SimpleJson *Json)
{
    char* cFloatString = (char*)malloc(sizeof(char) * 24);  
    if(cFloatString == NULL){
      return 0;
    }
    dtostrf(fValue, 2, 2, &cFloatString[strlen(cFloatString)]); 
    char* cTemp = UpdateValueInJson(Json->cBuffer, cField, cFloatString);    
    
    UltimateFree((void*)cFloatString);
    if(cTemp == NULL){
      return 0;
    }
    UltimateFree(Json->cBuffer);
    Json->cBuffer = cTemp;
    return 1;
}

/*
 * Function : sets up all the data needed for replacing a cField with boolean to the currentcJson
 * Parameter: 1x char pointer, 1x uint8_t cInteger, 1x SimpleJson pointer, char* cField = cField where it needs to be replaced, uint8_t value = value that needs to be replaced(1 = true, 0 = false), SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t ReplaceFieldWithBoolean(char* cField, uint8_t bValue, SimpleJson *Json)
{
    char* cBooleanToText;
    if(bValue){
      cBooleanToText = (char*)malloc( sizeof(char) * 5);
      
      if(cBooleanToText == NULL){
        return 0;
      }
      strcpy(cBooleanToText, "true\0");
    } else {
      cBooleanToText = (char*)malloc( sizeof(char) * 6); 
      
      if(cBooleanToText == NULL){
        return 0;
      }
      strcpy(cBooleanToText, "false\0");
    }
    char* cTemp = UpdateValueInJson(Json->cBuffer, cField, cBooleanToText);    
    
    UltimateFree((void*)cBooleanToText);
    if(cTemp == NULL){
      return 0;
    }
    UltimateFree(Json->cBuffer);
    Json->cBuffer = cTemp;
    return 1;
  
}

/*
 * Function : sets up all the data needed for adding a cField with Object to the currentcJson
 * Parameter: 1x char pointer, 2x SimpleJson pointer, char* cField = cField where it needs to be added, SimpleObject* Object = Object that needs to be added, SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns 1 on succes, 0 on error
 */
static uint8_t ReplaceFieldWithObject(char* cField, SimpleJson* Object, SimpleJson *Json)
{
    char* cTemp = UpdateObjectInJson(Json->cBuffer, cField, Object->cBuffer);
    if(cTemp == NULL){
      return 0;
    }
    UltimateFree(Json->cBuffer);
    Json->cBuffer = cTemp;
    return 1;
       
}

/*
 * Function : retreive a string value by cField within thecJson string.
 * Parameter: 1x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to retreive value,  SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns char* with value or NULL incase of error
 */
static char* GetFieldWithString(char* cField, SimpleJson *Json)
{
   char* cTemp = GetValueFromJson(Json->cBuffer, cField);
   if(cTemp == NULL){
    return NULL;
   }
   char* cToReturn = (char*)malloc(strlen(cTemp) * sizeof(char) + sizeof(char));
   strcpy(cToReturn, cTemp);
   UltimateFree(cTemp);
   return cToReturn;
}

/*
 * Function : retreive a int value by cField within thecJson string.
 * Parameter: 1x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to retreive value,  SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns int with value
 */
static int GetFieldWithInt(char* cField, SimpleJson *Json)
{
  int iToReturn;
  char* cIntAsString =  GetValueFromJson(Json->cBuffer, cField);
  if(cIntAsString == NULL){
    return 0;
  } 
  iToReturn = atoi(cIntAsString);
  UltimateFree((void*)cIntAsString);
  return iToReturn;
  
}

/*
 * Function : retreive a float value by cField within thecJson string.
 * Parameter: 1x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to retreive value,  SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns float with value
 */
static float GetFieldWithFloat(char* cField, SimpleJson *Json)
{ 
  float cToReturn;
  char* cFloatAsString =  GetValueFromJson(Json->cBuffer, cField);
  if(cFloatAsString == NULL){
    return 0;
  } 
  cToReturn = atof(cFloatAsString);
  UltimateFree((void*)cFloatAsString);
  return cToReturn;
}

/*
 * Function : retreive a boolean value by cField within thecJson string.
 * Parameter: 1x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to retreive value,  SimpleJson *Json correct struct where currentcJson resides
 * Returns  : returns uint8_t with value 1 or 0 
 */
static uint8_t GetFieldWithBoolean(char* cField, SimpleJson *Json)
{
  char* cBoolAsString =  GetValueFromJson(Json->cBuffer, cField);
  if(cBoolAsString == NULL){
    return NULL;
  } 
  if(strstr(cBoolAsString, "true") != NULL){
    UltimateFree(cBoolAsString);
    return 1;
  } else {
    UltimateFree(cBoolAsString);
    return 0;
  }
}

/*
 * Function : retreive a Object value by cField within the Json string.
 * Parameter: 1x char pointer, 1x SimpleJson pointer, char* cField = cField where it needs to retreive value,  SimpleJson *Json correct struct where current Json resides
 * Returns  : returns SimpleJson or NULL incase of error
 */
static SimpleJson* GetFieldWithObject(char* cField, SimpleJson *Json)
{
  char* cObjectAsString =  GetObjectFromJson(Json->cBuffer, cField);
  if(cObjectAsString == NULL){
    return NULL;
  }
  SimpleJson* ToReturn = parse_SimpleJson(strlen(cObjectAsString) + 100, cObjectAsString);
  UltimateFree((void*)cObjectAsString);
  return ToReturn;
}


/*
 * Function : return the full Json string as is within the struct
 * Parameter: 1x SimpleJson pointer, SimpleJson *Json = correct struct where current Json resides
 * Returns  : returns char* with full Json or NULL incase of error
 */
static char* ToString(SimpleJson *Json){
  if(Json->cBuffer != NULL){    
    char* cToReturn = (char*)malloc(strlen(Json->cBuffer)*sizeof(char) + sizeof(char));
    strcpy(cToReturn, Json->cBuffer);
    return cToReturn;  
  } else {
    return NULL;  
  }
}

/*
 * Function : constructor for SimpleJson, initializes Json buffer (string) and initializes all the fuctions
 * Parameter: 1x uint16_t , uint16_t iMinSize = minimum buffer size (is dynamic, so it will increase if limit is exceeded)
 * Returns  : returns SimplJson* or NULL in case of error
 */
SimpleJson *new_SimpleJson(uint16_t iMinSize)
{
 
  SimpleJson *Json = (SimpleJson*)malloc(sizeof(SimpleJson));
  if(Json == NULL){
    return NULL;
  }
  Json->ToString = ToString;
  Json->AddFieldWithString  = AddFieldWithString;
  Json->AddFieldWithInt   = AddFieldWithInt;
  Json->AddFieldWithFloat  = AddFieldWithFloat;
  Json->AddFieldWithBoolean = AddFieldWithBoolean;
  Json->AddFieldWithObject  = AddFieldWithObject;
  
  Json->ReplaceFieldWithString  = ReplaceFieldWithString;
  Json->ReplaceFieldWithInt   = ReplaceFieldWithInt;
  Json->ReplaceFieldWithFloat  = ReplaceFieldWithFloat;
  Json->ReplaceFieldWithBoolean = ReplaceFieldWithBoolean;
  Json->ReplaceFieldWithObject  = ReplaceFieldWithObject;
  
  Json->GetFieldWithString   = GetFieldWithString;
  Json->GetFieldWithInt    = GetFieldWithInt;
  Json->GetFieldWithFloat   = GetFieldWithFloat;
  Json->GetFieldWithBoolean  = GetFieldWithBoolean ;
  Json->GetFieldWithObject   = GetFieldWithObject;
  
  Json->cBuffer = (char*)calloc(iMinSize + 1, sizeof(char));  
  if(Json->cBuffer == NULL){
    return NULL;
  }
  memset(Json->cBuffer, '\0', iMinSize);
  strcpy(Json->cBuffer, "{}\0");
   
  Json->iMinSize = iMinSize;
  return Json;  
}

/*
 * Function : constructor for SimpleJson, initializes Json buffer (string), copies cBuffer into buffer and initializes all  the fuctions
 * Parameter: 1x uint16_t , uint16_t iMinSize = minimum buffer size (is dynamic, so it will increase if limit is exceeded)
 * Returns  : returns SimplJson* or NULL in case of error
 */
SimpleJson *parse_SimpleJson( uint16_t iMinSize, char* cJsonString)
{
  if(cJsonString == NULL){
    return NULL;
  }
  SimpleJson *Json = (SimpleJson*)malloc(sizeof(SimpleJson));
  if(Json == NULL){
    return NULL;
  }  
  Json->ToString = ToString;
  Json->AddFieldWithString  = AddFieldWithString;
  Json->AddFieldWithInt   = AddFieldWithInt;
  Json->AddFieldWithFloat  = AddFieldWithFloat;
  Json->AddFieldWithBoolean = AddFieldWithBoolean;
  Json->AddFieldWithObject  = AddFieldWithObject;
  
  Json->ReplaceFieldWithString  = ReplaceFieldWithString;
  Json->ReplaceFieldWithInt   = ReplaceFieldWithInt;
  Json->ReplaceFieldWithFloat  = ReplaceFieldWithFloat;
  Json->ReplaceFieldWithBoolean = ReplaceFieldWithBoolean;
  Json->ReplaceFieldWithObject  = ReplaceFieldWithObject;
  
  Json->GetFieldWithString   = GetFieldWithString;
  Json->GetFieldWithInt    = GetFieldWithInt;
  Json->GetFieldWithFloat   = GetFieldWithFloat;
  Json->GetFieldWithBoolean  = GetFieldWithBoolean ;
  Json->GetFieldWithObject   = GetFieldWithObject;

  Json->iMinSize = iMinSize;
  Json->cBuffer = (char*)calloc(iMinSize + 1, sizeof(char));
  if(Json->cBuffer == NULL){
    return NULL;
  }
  memset(Json->cBuffer, '\0', Json->iMinSize);
  strcpy(Json->cBuffer, cJsonString);

  return Json;  
}

/*
 * Function : frees SimpleJson struct and the allocated arrays/buffers inside SimpleJson
 * Parameter: 1x SimpleJson pointer, SimpleJson *Json correct struct where current Json resides
 * Returns  : returns SimplJson* or NULL in case of error
 */
void FreeSimpleJson(SimpleJson* Json){
  UltimateFree((void*)Json->cBuffer);
  UltimateFree((void*)Json);
}

