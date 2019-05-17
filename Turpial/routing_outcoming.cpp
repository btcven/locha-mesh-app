/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

// declaracion de librerias
#include <Arduino.h>
#include "routing_outcoming.h"
#include "general_functions.h"
#include "debugging.h"

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

uint8_t routing_outcoming_PACKET_MSG(char* id_node_destino, char mensaje[]){
// 1) el destino es un vecino
// 2) si  no es vecino existe una ruta al vecino (en caso negativo se devuelve packet not delivered por ahora en este demo)
// 3) se arma el paquete
// 4) se envia el paquete
      char id_node_dest_temp[SIZE_IDNODE];
       copy_array_locha(id_node_destino,id_node_dest_temp, SIZE_IDNODE);
      char id_node_temp[SIZE_IDNODE];
       copy_array_locha(id_node,id_node_temp, SIZE_IDNODE);
       
      packet_header_t header;
      packet_body_t body;

  if (es_vecino(id_node_destino)){
// 1) el destino es un vecino
    //1.1) se arma el paquete y se envia por una ruta directa
      
      header.type=Buffer_packet.header.type;
     
      copy_array_locha(id_node_temp,header.from, SIZE_IDNODE);
      copy_array_locha(id_node_dest_temp,header.to, SIZE_IDNODE);
      header.timestamp=millis();
      
      copy_array_locha(mensaje,body.payload, SIZE_PAYLOAD);
      Buffer_packet.header=header; 
      Buffer_packet.body=body;
      uint8_t rptax=packet_to_send(Buffer_packet,mensajes_salientes,total_mensajes_salientes,total_vecinos, total_rutas);  // se envia a la cola de mensajes salientes
  } else {
    if (existe_ruta(id_node, id_node_destino)){
        // se arma el paquete y se envia por esa ruta 
         header.type=MSG;
      
       copy_array_locha(id_node_temp,header.from, SIZE_IDNODE);
      copy_array_locha(id_node_dest_temp,header.to, SIZE_IDNODE);
      header.timestamp=millis();
       copy_array_locha(mensaje,body.payload, SIZE_PAYLOAD);
      Buffer_packet.header=header;
      Buffer_packet.body=body;
      uint8_t rptax=packet_to_send(Buffer_packet,mensajes_salientes,total_mensajes_salientes,total_vecinos, total_rutas);  // se envia a la cola de mensajes salientes
        
      
    } else {
        // no existe una ruta al destino se devuelve un packet not delivered
        DEBUG_PRINTLN(F("PACKET NOT DELIVERED"));
    }
  }

  return 0;
}
