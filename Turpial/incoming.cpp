#include <Arduino.h>
#include "route.h"
#include "packet.h"
#include "general_functions.h"
#include "debugging.h"

extern char* id_node; // id unico del nodo
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

// esta funcion procesa el paquete recibido 
void process_received_packet(packet_t Buffer_packet){

}

uint8_t routing_incoming_PACKET_MSG(char* id_node, packet_t packet_received){
  
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual


 // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
if (packet_received.header.to==id_node){
  // es un paquete para mi nodo
  process_received_packet(Buffer_packet);
  // se devuelve un packet_ACK por la misma ruta
  // se arma un packet_ACK
    packet_header_t header;
      packet_body_t body;

      header.type=ACK;
      //header.from=id_node;
      copy_array(id_node, header.from, 16);
      copy_array(packet_received.header.from, header.to, 16);
     // header.to=packet_received.header.from;
      header.timestamp=millis();
      copy_array(Buffer_packet.body.payload, body.payload, 240);
    //  body.payload=Buffer_packet.body.payload;   // aqui deberia devolver el hash y en base al hash validar que efectivamente cuando se reciba el ACK elimine al que corresponda
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
      copy_array(id_node, header.from, 16);
    //  header.from=id_node;
    copy_array(packet_received.header.from, header.to, 16);
     // header.to=packet_received.header.from;
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

uint8_t routing_incoming_PACKET_JOIN(char* id_node, packet_t packet_received){
  // nuevo vecino de la tabla de vecinos
  copy_array(packet_received.header.from, vecinos[total_vecinos+1].id, 16);
//  vecinos[total_vecinos+1]=packet_received.header.from;
  total_vecinos++;
  // nueva ruta en la tabla de rutas
  nodo_t nodo1;
  nodo_t nodo2;
  rutas_t nueva_ruta;
 // nodo1.id=packet_received.header.to;
  copy_array(packet_received.header.to, nodo1.id, 16);
 // nodo2.id=packet_received.header.from;
   copy_array(packet_received.header.from, nodo2.id, 16);
  create_route(nodo1, nodo2, nodo2);
  return 0;
}

uint8_t routing_incoming_PACKET_BYE(char* id_node, packet_t packet_received){
  // borra al vecino de la tabla de vecinos
  uint8_t i;
  uint8_t is_MSG=0;
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
    if (strcmp(routeTable[i].origen.id, packet_received.header.from) == 0){
        is_MSG=i;
        break;
    }
    if (strcmp(routeTable[i].destino.id,packet_received.header.from)==0){
        is_MSG=i;
        break;
    }
    if (strcmp(routeTable[i].origen.id,packet_received.header.to)==0){
        is_MSG=i;
        break;
    }
    if (strcmp(routeTable[i].destino.id,packet_received.header.to)==0){
        is_MSG=i;
        break;
    }
    if (strcmp(routeTable[i].next_neighbor.id,packet_received.header.to)==0){
        is_MSG=i;
        break;
    }
     if (strcmp(routeTable[i].next_neighbor.id,packet_received.header.from)==0){
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

uint8_t routing_incoming_PACKET_ROUTE(char* id_node, packet_t packet_received){
  // este tipo de paquete permite adicionar nuevas rutas a la tabla de rutas
  
  
  return 0;
}

uint8_t routing_incoming_PACKET_NOT_DELIVERED(char* id_node, packet_t packet_received){
  // si no es para mi se reenvia el paquete a los vecinos por la ruta donde origino
  
  return 0;
}

uint8_t routing_incoming_PACKET_ACK(char* id_node, packet_t packet_received){
  // se verifica si en los mensajes enviados hay uno que tenga el mismo payload para borrarlo
  uint8_t i;
  uint8_t is_MSG=0;
   for (i = 1; i <= total_mensajes_salientes; i++) {
      if ((strcmp(mensajes_salientes[i].paquete.header.to,id_node)==0)and(strcmp(mensajes_salientes[i].paquete.header.from,packet_received.header.to)==0)){
          // se verifica que sea un ACK de un mensaje tipo MSG
          if (mensajes_salientes[i].paquete.header.type=MSG){
                  // se verifica que tenga el mismo payload (esto deberia ser con el hash pero por ahora a efectos del demo se usa solo el mismo payload)
                if (strcmp(mensajes_salientes[i].paquete.body.payload,packet_received.body.payload)==0){
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
