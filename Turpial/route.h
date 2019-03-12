#ifndef ROUTE_H
#define ROUTE_H

typedef struct
{
    char* id;         // uniqueid
} nodo_t;


typedef struct
{
    packet_t paquete;         // uniqueid
    uint8_t prioridad;        // numero para asignar la prioridad a cada paquete 
} message_queue_t;

typedef struct
{
    nodo_t origen;  // uniqueid del destinatario
    nodo_t next_neighbor; // unique id del vecino mas cercano
    nodo_t destino; // unique id del vecino mas cercano
    uint8_t age;  // edad (ultima vez que se recibio/envio algo por esa ruta
} rutas_t;




#endif // ROUTE_H
