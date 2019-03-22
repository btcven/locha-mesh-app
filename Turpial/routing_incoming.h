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
#include <Arduino.h>
#include <string.h>
#include "route.h"
#include "packet.h"
#include "general_functions.h"


extern char* id_node; // id unico del nodo
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

uint8_t routing_incoming_PACKET_MSG(char* id_node, packet_t packet_received);
uint8_t routing_incoming_PACKET_JOIN(char* id_node, packet_t packet_received);
uint8_t routing_incoming_PACKET_BYE(char* id_node, packet_t packet_received);
uint8_t routing_incoming_PACKET_ROUTE(char* id_node, packet_t packet_received);
uint8_t routing_incoming_PACKET_NOT_DELIVERED(char* id_node, packet_t packet_received);
uint8_t routing_incoming_PACKET_ACK(char* id_node, packet_t packet_received);

// definicion de voids
void process_received_packet(packet_t Buffer_packet);


#endif // INCOMING_H
