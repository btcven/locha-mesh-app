/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
/**
   procedimientos para manejo de paquetes salientes
*/

#ifndef OUTCOMING_H
#define OUTCOMING_H

#include <string.h>
#include "route.h"
#include "packet.h"
#include "debugging.h"


extern char* id_node; // id unico del nodo
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

uint8_t routing_outcoming_PACKET_MSG(char* id_node_destino, char mensaje[]);


#endif // OUTCOMING_H