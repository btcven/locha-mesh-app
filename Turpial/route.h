/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef ROUTE_H
#define ROUTE_H

#include <Arduino.h>
#include "packet.h"


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


extern message_queue_t mensajes_salientes[255];
extern int total_mensajes_salientes;  
extern int total_vecinos;  
extern int total_rutas;
extern rutas_t routeTable[255];
extern nodo_t vecinos[255];

// verifica si el nodo a consultar esta en la tabla de vecinos
int es_vecino(char* id_nodo){
  int i;
  for (i = 1; i <= total_vecinos; i++) {
      if (vecinos[i].id==id_nodo){
        return 1;
      }
  }
  return 0;
}

// se busca en la tabla de rutas si existe una ruta al destino
int existe_ruta(char* id_nodo_from, char* id_nodo_to){
 int i;
  for (i = 1; i <= total_rutas; i++) {
      if ((routeTable[i].origen.id==id_nodo_from)and(routeTable[i].destino.id==id_nodo_to)){
        return 1;
      }
      // el inverso tambien es la misma ruta
      if ((routeTable[i].origen.id==id_nodo_to)and(routeTable[i].destino.id==id_nodo_from)){
        return 1;
      }
  }
  return 0;
  
}

// coloca el mensaje recibido en Buffer_packet a ka cika de mensajes salientes, ubicandolo segun su tipo/prioridad en la posicion de la cola de mensajes que le corresponda
void packet_to_send(packet_t Buffer_packet){
  // por ahora solo se agrega a la cola de paquetes salientes
  message_queue_t nuevo_mensaje_en_cola;
  nuevo_mensaje_en_cola.paquete=Buffer_packet;
  nuevo_mensaje_en_cola.prioridad=1;
  mensajes_salientes[total_mensajes_salientes+1]=nuevo_mensaje_en_cola;
  total_mensajes_salientes++;
}


#endif // ROUTE_H
