/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef ROUTE_H
#define ROUTE_H 

#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"


//extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
//extern rutas_t routeTable[MAX_ROUTES];
//extern nodo_t vecinos[MAX_NODES];
//extern nodo_t blacklist[MAX_NODES_BLACKLIST];
//extern uint8_t total_mensajes_salientes;  
//extern uint8_t total_vecinos;  
//extern uint8_t total_rutas;
//extern uint8_t total_nodos_blacklist;

typedef struct nodo_t
{
    char id[16];         // uniqueid
} nodo_t;


typedef struct message_queue_t
{
    packet_t paquete;         // uniqueid
    uint8_t prioridad;        // numero para asignar la prioridad a cada paquete 
} message_queue_t;

typedef struct rutas_t
{
    nodo_t origen;  // uniqueid del destinatario
    nodo_t next_neighbor; // unique id del vecino mas cercano
    nodo_t destino; // unique id del vecino mas cercano
    uint8_t age;  // edad (ultima vez que se recibio/envio algo por esa ruta
} rutas_t;


// definicion de voids
void packet_processing_incoming();
void packet_processing_outcoming();
uint8_t create_neighbor(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t blacklist[MAX_NODES_BLACKLIST], uint8_t total_nodos_blacklist);
uint8_t packet_to_send(packet_t Buffer_packet);
uint8_t create_route(nodo_t origen, nodo_t next_neighbor, nodo_t destino);
uint8_t update_route_age(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16], bool update_route);
uint8_t pos_ruta(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t es_vecino(char id_nodo[16]);




#endif // ROUTE_H
