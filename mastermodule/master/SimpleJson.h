#pragma once
/*
    SimpleJson.h
    Purpose: Doing json the sensible way ;) (which is: my way :))

    @author Eldin Zenderink
    @version 1.0 05/01/18
*/


#ifndef _SIMPLEJSON_H_
#define _SIMPLEJSON_H_

/******************************************************************************
  Includes
******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "Arduino.h"

/******************************************************************************
  Structs
******************************************************************************/
typedef struct SimpleJson{
   //attributes
   char* cBuffer;
   uint16_t iMinSize;
    
   //methods
   uint8_t(*AddFieldWithString)(char*, char*, struct SimpleJson *);
   uint8_t(*AddFieldWithInt)(char*,int, struct SimpleJson *);
   uint8_t(*AddFieldWithFloat)(char*, float, struct SimpleJson *);
   uint8_t(*AddFieldWithBoolean)(char*,uint8_t , struct SimpleJson *);
   uint8_t(*AddFieldWithObject)(char*, struct SimpleJson*, struct SimpleJson *);

   uint8_t(*ReplaceFieldWithString)(char*, char*, struct SimpleJson *);
   uint8_t(*ReplaceFieldWithInt)(char*,int, struct SimpleJson *);
   uint8_t(*ReplaceFieldWithFloat)(char*, float, struct SimpleJson *);
   uint8_t(*ReplaceFieldWithBoolean)(char*,uint8_t, struct SimpleJson *);
   uint8_t(*ReplaceFieldWithObject)(char*, struct SimpleJson*, struct SimpleJson *);

   char*(*GetFieldWithString)(char*, struct SimpleJson *);
   int(*GetFieldWithInt)(char*, struct SimpleJson *);
   float(*GetFieldWithFloat)(char*, struct SimpleJson *);
   uint8_t(*GetFieldWithBoolean)(char*, struct SimpleJson *);
   struct SimpleJson*(*GetFieldWithObject)(char*, struct SimpleJson *);

   char*(*ToString)(struct SimpleJson *);
}SimpleJson;


/******************************************************************************
  Function prototypes
******************************************************************************/
SimpleJson* new_SimpleJson(uint16_t iMinSize);

SimpleJson* parse_SimpleJson(uint16_t iMinSize, char* cJsonString);

void FreeSimpleJson(struct SimpleJson *);

#endif // _SIMPLEJSON_H_

