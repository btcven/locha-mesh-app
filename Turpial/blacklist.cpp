/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include "general_functions.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"
using namespace std;

extern char* id_node;
extern packet_t Buffer_packet;

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes; 
extern uint8_t total_mensajes_waiting; 
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;

// blacklist a node on memory  
uint8_t blacklist_add(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t (&blacklist)[MAX_NODES_BLACKLIST], uint8_t &total_nodos_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
    // 1) add to blacklist table
    // 2) delete blacklisted node from vecinos
    // 3) delete all routes to blacklisted node on routeTable
    char nombre_temporal[16];
    nodo_t nodo_vecino;
    uint8_t i;
    
    id_node_neighbor.toCharArray(nombre_temporal, 16);
    memcpy(nodo_vecino.id, nombre_temporal, 16);
    total_nodos_blacklist++;
    blacklist[total_nodos_blacklist] = nodo_vecino;
    uint8_t rpta=delete_neighbor(nombre_temporal,vecinos, total_vecinos);
    rpta=delete_route(id_node, nombre_temporal);
    return 0;
}
