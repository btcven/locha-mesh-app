#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include "route.h"
#include "hal/hardware.h"
#include "memory_def.h"
#include "general_functions.h"
#include "boards_def.h"
#include "radio.h"
#include "routing_incoming.h"
#include "routing_outcoming.h"
#include "debugging.h"

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

extern uint8_t packet_timeout;

//deletes expired packets in message queue
uint8_t delete_older_packets(){
  uint8_t i;
  uint8_t j;
   for (i = 1; i <= total_mensajes_salientes; i++) {
        unsigned long xx = mensajes_salientes[i].paquete.header.timestamp;
        if ((xx+packet_timeout)<now()){ 
          // se devuelve un packet NOT_DELIVERED a origen 
          // se borra de la cola de mensajes
            for (j = i; i < total_mensajes_salientes; j++) {
                mensajes_salientes[i]=mensajes_salientes[i+1];
            }
            total_mensajes_salientes--;
            return 1;
        }
   }
   return 0;
}



void packet_processing_outcoming(){
// aqui se deberia invocar radio_send del primer registro que consiga en el arreglo mensajes_salientes
 uint8_t i;
 String msg_to_send="";
bool encontre;
 // se borran los mensajes expirados
 do
{
    i=delete_older_packets();
} while (i>0);

 
 // se busca el primer mensaje saliente en la cola de mensajes cuyo tiempo de retry sea 0 o sea mayor a 30 seg de diferencia
 // se serializa en un String el mensaje saliente
 // se envia con el void radioSend
 // se marca en la tabla de salientes como enviado y reintentos++
 message_queue_t el_mensaje_saliente;
 if (total_mensajes_salientes>0){
  
  encontre=false;
    for (i = 1; i <= total_mensajes_salientes; i++) {
      DEBUG_PRINT(F("Sending packet "));
      DEBUG_PRINT(i);
      DEBUG_PRINT(F("/"));
      DEBUG_PRINTLN(total_mensajes_salientes);
          if (mensajes_salientes[i].retry_timestamp==0){
                encontre=true;
                break;
          } else {
            if (mensajes_salientes[i].retry_timestamp>0){
                if ((mensajes_salientes[i].retry_timestamp+packet_timeout)<now()){
                  encontre=true;
                  break;
                }
            }
          }
          
    }
    if (encontre){
      DEBUG_PRINTLN(F("Sending packet ..."));
        el_mensaje_saliente=mensajes_salientes[i];
        msg_to_send=packet_serialize(mensajes_salientes[i].paquete);
//        radioSend(msg_to_send);
        mensajes_salientes[i].retries=mensajes_salientes[i].retries+1;
        mensajes_salientes[i].retry_timestamp=now();
    }
 }
 
 

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
uint8_t packet_to_send(packet_t packet_temp){
  // por ahora solo se agrega a la cola de paquetes salientes
  uint8_t rptsx;
  
  message_queue_t nuevo_mensaje_en_cola;
  nuevo_mensaje_en_cola.paquete=packet_temp;
  nuevo_mensaje_en_cola.prioridad=1;
  nuevo_mensaje_en_cola.retries=0;
  nuevo_mensaje_en_cola.retry_timestamp=0;
  mensajes_salientes[total_mensajes_salientes+1]=nuevo_mensaje_en_cola;
  total_mensajes_salientes++;
   DEBUG_PRINTLN(F("se recibio:"));
  rptsx=show_packet(packet_temp,false);
  DEBUG_PRINTLN(F("y se coloco en cola:"));
  rptsx=show_packet(nuevo_mensaje_en_cola.paquete,false);
   DEBUG_PRINTLN(F("en la ultima posicion de la cola esta:"));
  rptsx=show_packet(mensajes_salientes[total_mensajes_salientes].paquete,false);
  DEBUG_PRINTLN(F("Packet queue succesfully"));
  return 0;
}

// funcion para proesar un mensaje BLE incoming
void BLE_incoming(char* uid2,char* msg, double timemsg){
  uint8_t i;
  uint8_t rpta;
  // si es un mensaje tipo broadcast se envia a todos los vecinos 
  DEBUG_PRINTLN(F("Starting BLE_incoming"));
//  Serial.print("tengo:");
//  Serial.println(uid2);
    if (String(uid2)=="broadcast"){ 
       DEBUG_PRINTLN(F("its a broadcast"));
       if (total_vecinos>0){ 
         for (i = 1; i <= total_vecinos; i++) {
          DEBUG_PRINTLN(F("enviando packet al vecino:"));
          DEBUG_PRINTLN(vecinos[i].id);
          // se arma el packet y se envia a cada vecino
          packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e("MSG"), id_node, vecinos[i].id, msg);
         
          DEBUG_PRINTLN("Packet received:");
          DEBUG_PRINT("type:");
          DEBUG_PRINTLN(convertir_packet_type_e_str(tmp_packet.header.type));
          DEBUG_PRINT("from:");
          DEBUG_PRINTLN(tmp_packet.header.from);
          DEBUG_PRINT("to:");
          DEBUG_PRINTLN(tmp_packet.header.to);
          DEBUG_PRINT("payload:");
          DEBUG_PRINTLN(tmp_packet.body.payload);
          
          rpta=packet_to_send(tmp_packet);
         }
       } else {
          DEBUG_PRINTLN(F("this node has no neigbors"));
       }
    } else {
       DEBUG_PRINT("its other type of packet:");
       DEBUG_PRINTLN(String(uid2));
       // por ahora todo lo que origina en BLE es tipo MSG
       if (String(uid2)!=""){
          packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e("MSG"), id_node, uid2, msg);
          rpta=packet_to_send(tmp_packet);
       }
    }
  DEBUG_PRINTLN(F("ready , packet sent to message queue"));
}
