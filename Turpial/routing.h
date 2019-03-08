/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef ROUTING_H
#define ROUTING_H

#include <stdint.h>
//#include <iostream>
//#include <cstdio>
//#include <cstdlib>
//#include <string>
//#include <algorithm>
//#include <random>
//#include <chrono>
//#include <fstream>

#define HEADER_LENGTH 16;

/**
 * @brief enums the packet type
 * 
 */
typedef enum packet_type_e
{
    EMPTY = 0,
    NET_JOIN = 1,
    NET_BYE = 2,
    NET_ROUTE = 3,
    PACKET_ACK = 4,
    PACKET_MSG = 5,  
    PACKET_TXN = 6,  //paquetes de LN, BTC TX
    PACKET_HELLO = 7,
    PACKET_GOSSIP = 8,
    PACKET_NOTDELIVERED = 9
};

/**
 * @brief enums the packet status
 * 
 */
typedef enum packet_status_e
{
    WAITING_ACK = 0,
    WAITING_RETRY = 1
};

/**
 * @brief quality of a given link.
 * 
 */
typedef struct
{
    uint8_t last_rssi;     //  fuerza relativa de la se�al.
    uint8_t last_snr;      //  relacion ruido / se�al
    uint8_t response_time; //  el tiempo de respuesta entre mensaje enviado y llegada de ACK
} quality_link_t;

/**
 * @brief enums the given services of a peer
 * 
 */
typedef enum services_e
{
    PEER = 0,
    ROUTER = 1,   // offers as router for the locha mesh (salida a internet)
    MSG_PUSH = 2, // offers as gateway for msgs, pushing msgs to internet??
    TX_PUSH = 3,  // offers as gateway for bitcoin txs, pushing txs by a dedicated service. sending ACK when the work it's done.
    ELECTRUM = 4,  // offers as a customized  electrum server.
    NODE_TYPE_PUBLIC = 5, // if node is open 
    NODE_TYPE_PRIVATE = 6, // if node is private 
    NODE_EXTENDED_CAPACITY = 7 // nodo con soporte Zerotier para salir a internet y conectarse via zerotier a otras redes que puedan ser mesh 
};

/**
 * @brief constructs a packet
 * 
 */
typedef struct
{
    uint8_t max_hops;       // max_hops no entra en el calculo de hash.
    uint8_t length_msg;     // longitud del mensaje.
    char* from;             // direccion del remitente
    char* to;               // enviamos el mensaje al id: to
    uint8_t sequence[2];    // por ejemplo {1, 2} representa parte 1 de 2 en total
    char* payload;          // es la secuencia del mensaje
    uint8_t msg_hash;       // first 4 bytes.
    packet_type_e type;
} packet_t;


/**
 * @brief structs neighbors table.
 * 
 */
typedef struct
{
    char* id;                   // uniqueid
    uint8_t offers[5];          // servicios que ofrece
    uint8_t age;                // edad conocida
    uint8_t hops_away;          // saltos de distancia (metrica)
    quality_link_t quality;     // calidad de enlace
    uint8_t connectivity_index; // indice de conectividad autocalculado por cada nodo
} neighbor_entry_t;

/**
 * @brief struct, entry to route table.
 * 
 */
typedef struct
{
    neighbor_entry_t receiver;      // uniqueid del destinatario
    neighbor_entry_t next_neighbor; // unique id del vecino mas cercano
    uint8_t age;                    // edad del path
    quality_link_t quality;         // calidad del path
} route_to_node_t;


typedef struct
{
  
  uint8_t retries;
  uint8_t packet_status;
  uint8_t lastseen;
} packets_status_t;

typedef struct
{
  packet_t el_packet;
  uint8_t retries;
  packet_status_e packet_status;
  uint8_t lastseen;
} packets_in_transit_t;

/**
 * @brief   enum the reasons.
 *          incomplete list!!
 * 
 */
typedef enum blacklisted_for
{
    BLOCK_FORWARD = 0,  // bloquea paquetes
    BLOCK_BACKWARD = 1, // bloquea paquetes de vuelta, ACK
    SPAMING = 2,        // cumple mesajes/tiempo pero envia mensajes de valor nulo o pernicioso 
    FLOODING = 3        // hace multicast de cualquier paquete que le es entregado, o envía mensajes sin cumplir la regla de mesnajes/time

};

/**
 * @brief entry to blacklist table
 * La primera y segunda vez que un peer entra en la blacklist 
 * es de forma temporal, si since > X : el peer sale de la lista
 * 
 * Tras la tercera entrada el valor locked se establece en true
 * y ya no es necesario monitorizar el valor since.
 */
typedef struct
{
    neighbor_entry_t peer;  // no creo q sea necesario duplicar este dato, con su id puede ser suficiente
    uint8_t reasons[2];     // blacklisted por.
    uint8_t since;          // blacklisted desde.
    uint8_t attempt;        // intento número.
    bool locked;            // bloqueado.
} blacklist_entry_t;

#endif // ROUTING_H
