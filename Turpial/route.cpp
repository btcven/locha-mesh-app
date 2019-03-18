#include <Arduino.h>
#include "hardware.h"
#include "general_functions.h"
#include "boards_def.h"
#include "memory_def.h"
#include "route.h"
#include "packet.h"


extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];

// verifica si el nodo a consultar esta en la tabla de vecinos
uint8_t es_vecino(char id_nodo[16]){
  uint8_t i;
  for (i = 1; i <= total_vecinos; i++) {
      if (vecinos[i].id==id_nodo){
        return 1;
      }
  }
  return 0;
}

// posicion de la ruta en la tabla de rutas en memoria
uint8_t pos_ruta(char id_nodo_from[16], char id_nodo_to[16]){
  uint8_t i;
  for (i = 1; i <= total_rutas; i++) {
      if ((routeTable[i].origen.id==id_nodo_from)and(routeTable[i].destino.id==id_nodo_to)){
        return i;
      }
      // el inverso tambien es la misma ruta
      if ((routeTable[i].origen.id==id_nodo_to)and(routeTable[i].destino.id==id_nodo_from)){
        return i;
      }
  }
  return 0;
}

// se busca en la tabla de rutas si existe una ruta al destino
uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16]){
  if (pos_ruta(id_nodo_from, id_nodo_to)>0){
    return 1;
  } 
  return 0;
  
}

uint8_t existe_ruta(char id_nodo_from[16], char id_nodo_to[16], bool update_route){
 
 uint8_t pos_route=pos_ruta(id_nodo_from, id_nodo_to);

  
  if (update_route){
     if (pos_route>0){
              routeTable[pos_route].age=millis();      
     } else {
      // no existe la ruta, se crea una nueva ruta
      
     }
  }
        // si existe la ruta se actualiza el 
      if (pos_route>0){
        return 1;
      } else {
        return 0;
      }
        
  
}

// update age of a route in routeTable , if didnt exist 
uint8_t update_route_age(char id_nodo_from[16], char id_nodo_to[16]){
  uint8_t respuesta=existe_ruta(id_nodo_from, id_nodo_to, true);
  return respuesta;
}

// create a new route on memory  
uint8_t create_route(nodo_t origen, nodo_t next_neighbor, nodo_t destino){
  rutas_t nueva_ruta;
  nueva_ruta.origen=origen;
  nueva_ruta.destino=destino;
  nueva_ruta.next_neighbor=next_neighbor;
  nueva_ruta.age=millis();
  routeTable[total_rutas+1]=nueva_ruta;
  total_rutas++;
  return 0;
}


// create a new neighbor on memory  
uint8_t create_neighbor(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos){
   nodo_t nodo_vecino;
   
                  char nombre_temporal[16];
                  id_node_neighbor.toCharArray(nombre_temporal, 16);
                 
                  // usamos memcpy ocupando la misma direccion de memoria
                  memcpy(nodo_vecino.id, nombre_temporal, 16);
                  // ***
                  total_vecinos++;
                  vecinos[total_vecinos] = nodo_vecino;
                  
//   id_node_neighbor.replace("  "," ");  // se elimina cualquier doble espacio en el input
//   id_node_neighbor.trim();
//   if (id_node_neighbor.length()>16){
//        id_node_neighbor.substring(0,16);
//   }
//   char* nombre_temporal;
 //  id_node_neighbor.toCharArray(nombre_temporal, 16);
//  id_node_neighbor.toCharArray(nodo_vecino.id, 16);
 //  nodo_vecino.id = nombre_temporal;
//   total_vecinos++;
//--   vecinos[total_vecinos] = nodo_vecino;
   
 
   
    return 0;
}

// coloca el mensaje recibido en Buffer_packet a ka cika de mensajes salientes, ubicandolo segun su tipo/prioridad en la posicion de la cola de mensajes que le corresponda
uint8_t packet_to_send(packet_t Buffer_packet){
  // por ahora solo se agrega a la cola de paquetes salientes
  message_queue_t nuevo_mensaje_en_cola;
  nuevo_mensaje_en_cola.paquete=Buffer_packet;
  nuevo_mensaje_en_cola.prioridad=1;
  mensajes_salientes[total_mensajes_salientes+1]=nuevo_mensaje_en_cola;
  total_mensajes_salientes++;
  return 0;
}
