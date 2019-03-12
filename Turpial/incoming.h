/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
/**
   procedimientos para manejo de paquetes entrantes
*/

#ifndef INCOMING_H
#define INCOMING_H

#include <string.h>
#include "route.h"
#include "packet.h"

extern char*  id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[255];
extern nodo_t vecinos[255];

int routing_incoming_PACKET_MSG(char* id_node, packet_t packet_received){
  
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual
  
  return 0;
}

int routing_incoming_PACKET_JOIN(char* id_node, packet_t packet_received){
  
  return 0;
}

int routing_incoming_PACKET_BYE(char* id_node, packet_t packet_received){
  
  return 0;
}

int routing_incoming_PACKET_ROUTE(char* id_node, packet_t packet_received){
  
  return 0;
}

int routing_incoming_PACKET_NOT_DELIVERED(char* id_node, packet_t packet_received){
  
  return 0;
}



#endif // INCOMING_H
