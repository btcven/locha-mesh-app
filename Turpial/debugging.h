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

//uint8_t mostrar_packet(packet_t el_packet);
//uint8_t vaciar_tablas();
//uint8_t mostrar_vecinos(char* node_id, nodo_t vecinos[MAX_NODES], size_t tamano_arreglo);
//uint8_t mostrar_rutas(char* node_id, rutas_t routeTable[MAX_ROUTES], size_t tamano_arreglo);
//uint8_t mostrar_cola_mensajes(message_queue_t mensajes_salientes[MAX_MSG_QUEUE], size_t tamano_arreglo);
//uint8_t iniciar_vecinos_y_rutas(char *id_nodo, nodo_t vecinos[MAX_NODES], rutas_t routeTable[MAX_ROUTES], size_t size_vecinos, size_t size_rutas);
uint8_t show_debugging_info(nodo_t vecinos[MAX_NODES]);



#endif // DEBUGGING_H
