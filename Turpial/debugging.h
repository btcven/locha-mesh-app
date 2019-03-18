/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas para debugging de la app
*/

#ifndef DEBUGGING_H 
#define DEBUGGING_H

#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"
#include "route.h"


#ifdef DEBUG
  #define DEBUG_BEGIN(x)                Serial.begin (x)
  #define DEBUG_PRINT(x)                Serial.print (x)
  #define DEBUG_WRITE(x)                Serial.write (x)
  #define DEBUG_PRINT_DEC(x)            Serial.print (x, DEC)
  #define DEBUG_PRINT_HEX(x)            Serial.print (x, HEX)
  #define DEBUG_PRINTLN(x)              Serial.println (x)
  #define DEBUG_PRINT_STR(x)            Serial.print (F(x))
  #define DEBUG_PRINTLN_STR(x)          Serial.println (F(x))
#else
  #define DEBUG_BEGIN(x)
  #define DEBUG_PRINT(x)
  #define DEBUG_WRITE(x)
  #define DEBUG_PRINT_DEC(x)
  #define DEBUG_PRINT_HEX(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT_STR(x)
  #define DEBUG_PRINTLN_STR(x)
#endif

uint8_t show_debugging_info(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, String &rxValue, String &txValue);



#endif // DEBUGGING_H
