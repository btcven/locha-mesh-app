/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef ROUTE_H
#define ROUTE_H

// declaracion de librerias
#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"


// declaracion de tipos de datos
typedef struct nodo_t
{
    char id[16]; // uniqueid
} nodo_t;

typedef struct message_queue_t
{
    packet_t paquete;  // packet completo, incluyendo header y body
    uint8_t prioridad; // numero para asignar la prioridad a cada paquete
    uint8_t retries;   // numero de reintentos de envio
    unsigned long retry_timestamp;   // hora del ultimo reintento
    
} message_queue_t;

typedef struct rutas_t
{
    nodo_t origen;        // uniqueid del destinatario
    nodo_t next_neighbor; // unique id del vecino mas cercano
    nodo_t destino;       // unique id del vecino mas cercano
    uint8_t age;          // edad (ultima vez que se recibio/envio algo por esa ruta)
    uint8_t hops;         // cantidad de saltos entre origen y destino
    int RSSI_packet;   //  RSSI de este paquete 
    int SNR_packet;    // SNR del este paquete
} rutas_t;

//declaracion de variables
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting; 
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;

// definicion de funciones
void packet_processing_outcoming(message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE],uint8_t &total_mensajes_salientes,message_queue_t (&mensajes_waiting)[MAX_MSG_QUEUE],uint8_t &total_mensajes_waiting);
void BLE_incoming(char* uid2,char* msg_ble, char* timemsg, char* hash_msg, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp2);
void broadcast_bye(char* id_node,struct nodo_t (vecinos)[MAX_NODES], uint8_t total_vecinos, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes);
uint8_t create_neighbor(char* id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t blacklist[MAX_NODES_BLACKLIST], uint8_t total_nodos_blacklist );
uint8_t packet_to_send(packet_t packet_temp, message_queue_t (&mensajes_salientes_tmp)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp);
uint8_t create_route(nodo_t origen, nodo_t next_neighbor, nodo_t destino);
uint8_t update_route_age(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16], bool update_route);
uint8_t pos_ruta(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t es_vecino(char id_nodo[16]);
uint8_t delete_neighbor(String id_node_neighbor, struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos);
uint8_t delete_route(char id_nodo_from[16], char id_nodo_to[16]);
uint8_t delete_route_by_id(uint8_t id_to_delete);
uint8_t delete_older_packets();


#endif // ROUTE_H
