/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef DEBUGGING_H
#define DEBUGGING_H


// Rutinas para debugging de la app

// declaracion de librerias
#include <Arduino.h>
#include "packet.h"
#include "route.h"

using namespace std;

// la siguiente linea se comenta para produccion, solo es para hacer debugging en desarrollo, esta variable desactiva todos los comandos via consola serial
#define DEBUG true

extern char *id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas;
extern uint8_t total_mensajes_salientes;
extern uint8_t total_mensajes_waiting;
extern String remote_debugging;

// se redevine la salida serial si el debug esta activado
#ifdef DEBUG
  #define DEBUG_BEGIN(x) Serial.begin(x)
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_WRITE(x) Serial.write(x)
  #define DEBUG_PRINT_DEC(x) Serial.print(x, DEC)
  #define DEBUG_PRINT_HEX(x) Serial.print(x, HEX)
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINT_STR(x) Serial.print(F(x))
  #define DEBUG_PRINTLN_STR(x) Serial.println(F(x))
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

// declaracion de funciones
uint8_t show_debugging_info(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, String &remote_debugging);
uint8_t delete_packet(uint8_t id_to_delete, message_queue_t (&mensajes_salientes_tmp)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp);
uint8_t show_packet(packet_t el_packet, bool show_size);

#endif // DEBUGGING_H
