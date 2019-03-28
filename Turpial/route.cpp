#include <Arduino.h>
#include "hal/hardware.h"
#include "general_functions.h"
#include "boards_def.h"
#include "memory_def.h"
#include "route.h"
#include "packet.h"
#include "routing_incoming.h"
#include "routing_outcoming.h"

extern char* id_node;
extern packet_t Buffer_packet;

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;




void packet_processing_incoming(){
  // se procesa el packet que fue recibido por el radio lora y que esta en Buffer_packet
  
  process_received_packet(Buffer_packet);    // este void esta en routing_incoming.h
  
}

void packet_processing_outcoming(){

  
}


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
  uint8_t i;
  rutas_t nueva_ruta;
  bool ejecute_correctamente=true;
  
  // se verifica que no exista previamente la ruta o el inverso de la ruta en las tablas
    for (i = 0; i < total_rutas; i++) {
      if ((routeTable[i].origen.id==origen.id)and(routeTable[i].destino.id==destino.id)and(routeTable[i].next_neighbor.id==next_neighbor.id)){
          ejecute_correctamente=false;
          break;
      }
      if ((routeTable[i].origen.id==destino.id)and(routeTable[i].destino.id==origen.id)and(routeTable[i].next_neighbor.id==next_neighbor.id)){
          ejecute_correctamente=false;
          break;
      }
    }

    
  if (ejecute_correctamente){
      nueva_ruta.origen=origen;
      nueva_ruta.destino=destino;
      nueva_ruta.next_neighbor=next_neighbor;
      nueva_ruta.age=millis();
      routeTable[total_rutas+1]=nueva_ruta;
      total_rutas++;
      return 0;
  } else {
    return 1;
  }
  
}

uint8_t delete_route_by_id(uint8_t id_to_delete){
   uint8_t i;
   if (id_to_delete>0){
      
      for (i = id_to_delete; i < total_rutas; i++) {
          routeTable[i]=routeTable[i+1];
      }
      total_rutas--;
     
     }
     return 0;
}

uint8_t delete_route(char id_nodo_from[16], char id_nodo_to[16]){
    uint8_t i;
    uint8_t j;
    bool encontro_ruta=false;
    // se busca si existe en la tabla de rutas que id tiene y se elimina
     for (i = 0; i <= total_rutas; i++) {
        if ((routeTable[i].origen.id==id_nodo_from)and(routeTable[i].destino.id==id_nodo_to)){
            encontro_ruta=true;
            j=i;
            break;
        }
        // y la inversa
        if ((routeTable[i].origen.id==id_nodo_to)and(routeTable[i].destino.id==id_nodo_from)){
            encontro_ruta=true;
            j=i;
            break;
        }
        // se borran las rutas intermedias que puedan coincidir con el criterio de from y to
        if ((routeTable[i].origen.id==id_nodo_from)and(routeTable[i].next_neighbor.id==id_nodo_to)){
            encontro_ruta=true;
            j=i;
            break;
        }
        if ((routeTable[i].destino.id==id_nodo_from)and(routeTable[i].next_neighbor.id==id_nodo_to)){
            encontro_ruta=true;
            j=i;
            break;
        }
        // y las inversas
         if ((routeTable[i].origen.id==id_nodo_to)and(routeTable[i].next_neighbor.id==id_nodo_from)){
            encontro_ruta=true;
            j=i;
            break;
        }
        if ((routeTable[i].destino.id==id_nodo_to)and(routeTable[i].next_neighbor.id==id_nodo_from)){
            encontro_ruta=true;
            j=i;
            break;
        }
        
     }
     if (encontro_ruta){
       uint8_t rpta=delete_route_by_id(j);   // se borra y se invoca recursivamente para garantizar que no queden otras rutas
       rpta=delete_route(id_nodo_from, id_nodo_to);
     }
     
     return 0;
    
}

uint8_t delete_neighbor(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos){
   char nombre_temporal[16];
   uint8_t i;
   uint8_t j;
   id_node_neighbor.toCharArray(nombre_temporal, 16);
   bool encontre=false;
   for (i = 1; i <= total_vecinos; i++) {
        if (vecinos[i].id==nombre_temporal){
          encontre=true;
          break;
        }
   }
   if (encontre) {
         for (j = i; i < total_vecinos; j++) {
              vecinos[i]=vecinos[i+1];
         }
         total_vecinos--;
   }
   uint8_t rpta=delete_route(id_node,nombre_temporal);
   return 0;
                  
}

// create a new neighbor on memory  
uint8_t create_neighbor(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t blacklist[MAX_NODES_BLACKLIST], uint8_t total_nodos_blacklist ){
   nodo_t nodo_vecino;
   
                  char nombre_temporal[16];
                  uint8_t i;
                  bool permitir_agregar=true;
                  id_node_neighbor.toCharArray(nombre_temporal, 16);
            
                 // se verifica que no exista previamente
                 for (i = 1; i <= total_vecinos; i++) {
                     if (vecinos[i].id==nombre_temporal){
                          // existe previamente, no se crea de nuevo
                          permitir_agregar=false;
                          break;
                      }
                 }
                 // se verifica que no exista en blacklist de nodos 
                 for (i = 1; i <= total_nodos_blacklist; i++) {
                      if (blacklist[i].id==nombre_temporal){
                          // como esta en blacklist no se le permite agregar como un vecino valido
                          permitir_agregar=false;
                          break;
                      }
                 }
                 if (permitir_agregar){
                      // usamos memcpy ocupando la misma direccion de memoria
                      memcpy(nodo_vecino.id, nombre_temporal, 16);
                      // ***
                      total_vecinos++;
                      vecinos[total_vecinos] = nodo_vecino;
                      
    return 0;
                 } else {
                  
    return 1;
                 }
                  
 
}

// coloca el mensaje recibido en Buffer_packet a la cola de mensajes salientes, ubicandolo segun su tipo/prioridad en la posicion de la cola de mensajes que le corresponda
uint8_t packet_to_send(packet_t Buffer_packet){
  // por ahora solo se agrega a la cola de paquetes salientes
  message_queue_t nuevo_mensaje_en_cola;
  nuevo_mensaje_en_cola.paquete=Buffer_packet;
  nuevo_mensaje_en_cola.prioridad=1;
  mensajes_salientes[total_mensajes_salientes+1]=nuevo_mensaje_en_cola;
  total_mensajes_salientes++;
  return 0;
}

// funcion para proesar un mensaje BLE incoming
void BLE_incoming(char* uid,char* msg, double timemsg){
  uint8_t i;
  uint8_t rpta;
  // si es un mensaje tipo broadcast se envia a todos los vecinos 
    if (String(uid)=="broadcast"){ 
       for (i = 1; i <= total_vecinos; i++) {
        // se arma el packet y se envia a cada vecino
        packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e("MSG"), id_node, vecinos[i].id, msg);
          rpta=packet_to_send(tmp_packet);
       }
    }
}
