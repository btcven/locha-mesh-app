#include <Arduino.h>
#include "hardware.h"
#include "general_functions.h"
#include "debugging.h"
#include "boards_def.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"

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
    
    
   
}
