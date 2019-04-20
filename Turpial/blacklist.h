/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef BLACKLIST_H 
#define BLACKLIST_H

// declaracion de librerias
#include <Arduino.h>
using namespace std;

//declaracion de variables


// declaracion de funciones
uint8_t blacklist_node_add(char* id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t (&blacklist_nodes)[MAX_NODES_BLACKLIST], uint8_t &total_nodos_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas);
uint8_t blacklist_route_add(char* from, char* to,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct rutas_blacklisted_t (&blacklist_routes)[MAX_NODES_BLACKLIST], uint8_t &total_rutas_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas);


#endif   // BLACKLIST_H
