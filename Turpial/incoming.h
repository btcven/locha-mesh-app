/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
/**
   procedimientos para manejo de paquetes entrantes
*/

#ifndef INCOMING_H
#define INCOMING_H

#include <string.h>
#include "route.h"
#include "packet.h"
#include "debugging.h"

extern char* id_node; // id unico del nodo
extern packet_t Buffer_packet;
extern rutas_t routeTable[255];
extern nodo_t vecinos[255];
extern message_queue_t mensajes_salientes[255];
extern int total_vecinos;
extern int total_rutas; 
extern int total_mensajes_salientes; 

int routing_incoming_PACKET_MSG(char* id_node, packet_t packet_received){
  
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual


 // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
if (packet_received.header.to==id_node){
  // es un paquete para mi nodo
  process_received_packet(packet_t Buffer_packet);
  // se devuelve un packet_ACK por la misma ruta
  // se arma un packet_ACK
    packet_header_t header;
      packet_body_t body;

      header.type=ACK;
      header.from=id_node;
      header.to=packet_received.header.from;
      header.timestamp=millis();
      body.payload=Buffer_packet.body.payload;   // aqui deberia devolver el hash y en base al hash validar que efectivamente cuando se reciba el ACK elimine al que corresponda
      packet_t new_packet;
      new_packet.header=header;
      new_packet.body=body;
      packet_to_send(new_packet);  // se envia a la cola de mensajes salientes

      // se actualiza el age de la ruta desde el origen al destino y si no existe se crea
       update_route_age(packet_received.header.from, packet_received.header.to);
} else {
  // el paquete no es para mi, pero tengo que hacerle relay a mis vecinos
  // busco si tengo una ruta entre mi nodo y el destino del paquete (y se actualiza el age de la ruta al conseguirla o se crea si no existe)
  if (existe_ruta(id_node, packet_received.header.to,true)){
       packet_header_t header;
      packet_body_t body;

      header.type=ACK;
      header.from=id_node;
      header.to=packet_received.header.from;
      header.timestamp=millis();
  //    header.last_node=id_node;   // este parametro se encarga de determinar que no se devuelva el mismo paquete hacia el origen
      
      body=Buffer_packet.body;  
      packet_t new_packet;
      new_packet.header=header;
      new_packet.body=body;
      packet_to_send(new_packet);  // se envia a la cola de mensajes salientes
  } else {
    // si no existe ruta, falta determinar si me voy random por cualquiera de los nodos para intentar
  }

}
  
  return 0;
}

int routing_incoming_PACKET_JOIN(char* id_node, packet_t packet_received){
  // nuevo vecino de la tabla de vecinos
  vecinos[total_vecinos+1]=packet_received.header.from;
  total_vecinos++;
  // nueva ruta en la tabla de rutas
  nodo_t nodo1;
  nodo_t nodo2;
  rutas_t nueva_ruta;
  nodo1.id=packet_received.header.to;
  nodo2.id=packet_received.header.from;
  create_route(nodo1, nodo_t nodo2, nodo2);
  return 0;
}

int routing_incoming_PACKET_BYE(char* id_node, packet_t packet_received){
  // borra al vecino de la tabla de vecinos
  int i;
  int is_MSG=0;
   for (i = 1; i <= total_vecinos; i++) {
      if (vecinos[i].id==packet_received.header.from){
        is_MSG=i;
        break;
      }
   }
   if (is_MSG>0){
       for (i = is_MSG; i <= total_vecinos-1; i++) {
          vecinos[i]=vecinos[i+1];
       }
       total_vecinos=total_vecinos-1;
   }

  // se eliminan las rutas a ese nodo
  is_MSG=0;
  for (i = 1; i <= total_rutas; i++) {
    if (routeTable[i].origen==packet_received.header.from){
        is_MSG=i;
        break;
    }
    if (routeTable[i].destino==packet_received.header.from){
        is_MSG=i;
        break;
    }
    if (routeTable[i].origen==packet_received.header.to){
        is_MSG=i;
        break;
    }
    if (routeTable[i].destino==packet_received.header.to){
        is_MSG=i;
        break;
    }
    if (routeTable[i].next_neighbor==packet_received.header.to){
        is_MSG=i;
        break;
    }
     if (routeTable[i].next_neighbor==packet_received.header.from){
        is_MSG=i;
        break;
    }
  }
 if (is_MSG>0){
       for (i = is_MSG; i <= total_rutas-1; i++) {
          routeTable[i]=routeTable[i+1]; 
       }
       total_rutas=total_rutas-1;
   }
  
  return 0;
}

int routing_incoming_PACKET_ROUTE(char* id_node, packet_t packet_received){
  // este tipo de paquete permite adicionar nuevas rutas a la tabla de rutas
  
  
  return 0;
}

int routing_incoming_PACKET_NOT_DELIVERED(char* id_node, packet_t packet_received){
  // si no es para mi se reenvia el paquete a los vecinos por la ruta donde origino
  
  return 0;
}

int routing_incoming_PACKET_ACK(char* id_node, packet_t packet_received){
  // se verifica si en los mensajes enviados hay uno que tenga el mismo payload para borrarlo
  int i;
  int is_MSG=0;
   for (i = 1; i <= total_mensajes_salientes; i++) {
      if ((mensajes_salientes[i].header.to==id_node)and(mensajes_salientes[i].header.from==packet_received.header.to)){
          // se verifica que sea un ACK de un mensaje tipo MSG
          if (mensajes_salientes[i].header.type=MSG){
                  // se verifica que tenga el mismo payload (esto deberia ser con el hash pero por ahora a efectos del demo se usa solo el mismo payload)
                if (mensajes_salientes[i].body==packet_received.body){
                  // se recibio el ACK de que el mensaje MSG fue recibido correctamente
                  is_MSG=i;
                  break;
                }
           }
          if (is_MSG>0){
             // se borra el mensaje de la tabla de mensajes_salientes
              for (i = is_MSG; i <= total_mensajes_salientes-1; i++) {
                  mensajes_salientes[i]=mensajes_salientes[i+1];
              }
              total_mensajes_salientes=total_mensajes_salientes-1;
              DEBUG_PRINTLN(F("ACK del packet recibido exitosamente"));
          }
      }
   }
}
}


#endif // INCOMING_H
