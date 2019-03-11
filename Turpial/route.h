#ifndef ROUTE_H
#define ROUTE_H

typedef struct
{
    char* id;         // uniqueid
} nodo_t;

typedef struct
{
    nodo_t origen;  // uniqueid del destinatario
    nodo_t next_neighbor; // unique id del vecino mas cercano
    nodo_t destino; // unique id del vecino mas cercano
    uint8_t age;  // edad (ultima vez que se recibio/envio algo por esa ruta
} rutas_t;

// verifica si el nodo a consultar esta en la tabla de vecinos
int es_vecino(char* id_nodo){

  
}

// se busca en la tabla de rutas si existe una ruta al destino
int existe_ruta(char* id_nodo_from, char* id_nodo_to){

  
}


#endif // ROUTE_H
