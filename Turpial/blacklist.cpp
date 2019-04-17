/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include "general_functions.h"
#include "boards_def.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"
using namespace std;

extern char* id_node;
extern packet_t Buffer_packet;

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes; 
extern uint8_t total_mensajes_waiting; 
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_rutas_blacklist;
extern uint8_t total_nodos_blacklist;

extern nodo_t blacklist_nodes[MAX_NODES_BLACKLIST];
extern uint32_t total_blacklist_nodes ;  // size of blacklisted nodes table

extern rutas_blacklisted_t blacklist_routes[MAX_NODES_BLACKLIST];
extern uint32_t total_blacklist_routes ;  // size of blacklisted routes table

// blacklist a node 
uint8_t blacklist_node_add(char* id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t (&blacklist_nodes)[MAX_NODES_BLACKLIST], uint8_t &total_nodos_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
    // 1) add to blacklist node table
    // 2) delete blacklisted node from vecinos
    // 3) delete all routes to blacklisted node on routeTable
    char nombre_temporal[16];
    nodo_t nodo_vecino;
    uint8_t i;
       copy_array_locha(id_node_neighbor,nodo_vecino.id, 16);
 //   id_node_neighbor.toCharArray(nombre_temporal, 16);
 //   memcpy(nodo_vecino.id, nombre_temporal, 16);
    total_nodos_blacklist++;
    blacklist_nodes[total_nodos_blacklist] = nodo_vecino;
    uint8_t rpta=delete_neighbor(id_node_neighbor,vecinos, total_vecinos);
    return delete_route(id_node, id_node_neighbor);
}

// blacklist a route 
uint8_t blacklist_route_add(char* from, char* to,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct rutas_blacklisted_t (&blacklist_routes)[MAX_NODES_BLACKLIST], uint8_t &total_rutas_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
    // 1) add to blacklist route table
    // 2) delete blacklisted node from vecinos
    // 3) delete all routes to blacklisted node on routeTable
 
    rutas_blacklisted_t route_to_block;
    uint8_t i;
    copy_array_locha(from, route_to_block.from, 16);
    copy_array_locha(to, route_to_block.to, 16);
    total_blacklist_routes++;
    blacklist_routes[total_blacklist_routes]=route_to_block;
    // se elimina la ruta en caso de que exista
    
    return delete_route(from, to);
}
