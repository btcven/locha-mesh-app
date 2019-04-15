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

// declaracion de librerias
#include <Arduino.h>
#include <string.h>
#include "memory_def.h"
#include "route.h"
#include "packet.h"

//declaracion de variables
extern char* id_node; // id unico del nodo
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting; 
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

// declaracion de funciones
void process_received_packet(char id_node[16], packet_t packet_temporal,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, int RSSI_recibido, int SNR_recibido);
uint8_t routing_incoming_PACKET_MSG(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_JOIN(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_BYE(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_ROUTE(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_NOT_DELIVERED(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_ACK(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_GOSSIP(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_TXN(char id_node[16], packet_t packet_received);
uint8_t routing_incoming_PACKET_HELLO(char id_node[16], packet_t packet_received);

#endif // INCOMING_H
