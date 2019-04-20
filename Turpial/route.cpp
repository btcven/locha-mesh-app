  /**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <sstream>
#include <string>
#include "route.h"
#include "hal/hardware.h"
#include "memory_def.h"
#include "general_functions.h"
#include "boards_def.h"
#include "radio.h"
#include "routing_incoming.h"
#include "routing_outcoming.h"
#include "debugging.h"

extern std::string txValue;
extern std::string rxValue;

extern char* id_node;
extern packet_t Buffer_packet;

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

extern uint8_t packet_timeout;

//deletes expired packets in message queue
uint8_t delete_older_packets(){
  uint8_t i;
  uint8_t j;
  if (total_mensajes_salientes>0){
       for (i = 1; i <= total_mensajes_salientes; i++) {
        Serial.print("adentro:");
        Serial.println(i);
            unsigned long xx = mensajes_salientes[i].paquete.header.timestamp;
            if (xx>0){
              if ((now()-xx)>packet_timeout){ 
                // se devuelve un packet NOT_DELIVERED a origen 
                // se borra de la cola de mensajes
                  for (j = i; i < total_mensajes_salientes; j++) {
                      mensajes_salientes[i]=mensajes_salientes[i+1];
                  }
                  Serial.print("entre con:");
                  Serial.println((String)(xx+packet_timeout));
                  Serial.print("y en salientes me quedan:");
                  Serial.println(total_mensajes_salientes);
                  total_mensajes_salientes--;
                  return 1;
              }
            }
       }
  }
   return 0;
}



void packet_processing_outcoming(message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE],uint8_t &total_mensajes_salientes,message_queue_t (&mensajes_waiting)[MAX_MSG_QUEUE],uint8_t &total_mensajes_waiting){
// aqui se deberia invocar radio_send del primer registro que consiga en el arreglo mensajes_salientes
 uint8_t i;
 uint8_t j;
 String msg_to_send="";
bool encontre;


 
 // se busca el primer mensaje saliente en la cola de mensajes cuyo tiempo de retry sea 0 o sea mayor a 30 seg de diferencia
 // se serializa en un String el mensaje saliente
 // se envia con el void radioSend
 // se marca en la tabla de salientes como enviado y reintentos++
 message_queue_t el_mensaje_saliente;
 if (total_mensajes_salientes>0){
  // se borran los mensajes expirados

  //DEBUG_PRINT(F("Deleting older packets ..."));
   // i=delete_older_packets();
 //   DEBUG_PRINT(i);
  //DEBUG_PRINT(F("Processing outcoming packets"));
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
            
                if ((mensajes_salientes[i].retry_timestamp+packet_timeout)>now()){
                  encontre=true;
                  break;
                } else {
                  // es un packet que vencio el tiempo de espera
                  DEBUG_PRINT(F("Hay packets en cola pero estan pendientes por reintentar en "));
                  DEBUG_PRINT(packet_timeout/1000);
                  DEBUG_PRINT(" segundos");
                }
            
          }
          
    }
    
    if (encontre){
      DEBUG_PRINTLN(F("Sending packet ..."));
        el_mensaje_saliente=mensajes_salientes[i];
        msg_to_send=packet_serialize(mensajes_salientes[i].paquete);
        uint8_t rpta_rad=radioSend(msg_to_send);
        // se coloca el radio nuevamente en modo receives (se hace por segunda vez porque detectamos algunos casos en donde el radio no cambio de modo dentro del radioSend()
        LoRa.receive();
        if (rpta_rad==0){ 
          // no se transmitio se hace un reintento 
          delay(500);
          rpta_rad=radioSend(msg_to_send);
          
        // se coloca el radio nuevamente en modo receives (se hace por segunda vez porque detectamos algunos casos en donde el radio no cambio de modo dentro del radioSend()
        LoRa.receive();
        }
        DEBUG_PRINTLN(F("Return to processing outcoming ..."));
        // se colcoa el packet como waiting si el tipo de packet es MSG
        if (total_mensajes_waiting<MAX_MSG_QUEUE){
            DEBUG_PRINTLN(F("Packet marked waiting ACK ..."));
            total_mensajes_waiting++;
            mensajes_waiting[total_mensajes_waiting].paquete=mensajes_salientes[i].paquete;
            mensajes_waiting[total_mensajes_waiting].retries=1;
            mensajes_waiting[total_mensajes_waiting].prioridad=1;
            mensajes_waiting[total_mensajes_waiting].retry_timestamp=millis();
  DEBUG_PRINT(F("Tengo mensajes waiting:"));
           DEBUG_PRINTLN((String)total_mensajes_waiting);
        }
        DEBUG_PRINT("total waiting pos1:");
            
            DEBUG_PRINTLN((String)total_mensajes_waiting);
        DEBUG_PRINTLN(F("Sended OK"));
        DEBUG_PRINT("total waiting pos1:");
            
            DEBUG_PRINTLN((String)total_mensajes_waiting);
        // se borra de la cola de mensajes
       for (j = i; j < total_mensajes_salientes; j++) {
            mensajes_salientes[j]=mensajes_salientes[j+1];
       }
       total_mensajes_salientes--;
       DEBUG_PRINT("total waiting pos1:");
            
            DEBUG_PRINTLN((String)total_mensajes_waiting);
       DEBUG_PRINTLN(F("Packet deleted from queue"));
 // se muestra el packet a ver si es el contenido deseado
            DEBUG_PRINTLN(F("Tengo el packet waiting:"));
            DEBUG_PRINT("FROM:");
            DEBUG_PRINTLN((String)mensajes_waiting[total_mensajes_waiting].paquete.header.from);
            DEBUG_PRINT("TO:");
            DEBUG_PRINTLN((String)mensajes_waiting[total_mensajes_waiting].paquete.header.to);
            DEBUG_PRINT("PAYLOAD:");
            DEBUG_PRINTLN((String)mensajes_waiting[total_mensajes_waiting].paquete.body.payload);
            DEBUG_PRINT("total final:");
            
            DEBUG_PRINTLN((String)total_mensajes_waiting);

      
      //  mensajes_salientes[i].retries=mensajes_salientes[i].retries+1;
      //  mensajes_salientes[i].retry_timestamp=now();
    }
   
 }
 
 

}


// verifica si el nodo a consultar esta en la tabla de vecinos
uint8_t es_vecino(char id_nodo[SIZE_IDNODE]){
  uint8_t i;
   char *pChar = (char*)"";
  if (!(compare_char(id_nodo,pChar))){
  
    for (i = 1; i <= total_vecinos; i++) {
        if (compare_char(vecinos[i].id,id_nodo)){
          return 1;
        }
    }
  }
  return 0;
}

// posicion de la ruta en la tabla de rutas en memoria
uint8_t pos_ruta(char id_nodo_from[SIZE_IDNODE], char id_nodo_to[SIZE_IDNODE]){
  uint8_t i;
  for (i = 1; i <= total_rutas; i++) {
  
      if ((compare_char(routeTable[i].origen.id,id_nodo_from))and(compare_char(routeTable[i].destino.id,id_nodo_to))){
        return i;
      }
      // el inverso tambien es la misma ruta
      if ((compare_char(routeTable[i].origen.id,id_nodo_to))and(compare_char(routeTable[i].destino.id,id_nodo_from))){
        return i;
      }
  }
  return 0;
}

// se busca en la tabla de rutas si existe una ruta al destino
uint8_t existe_ruta(char id_nodo_from[SIZE_IDNODE], char id_nodo_to[SIZE_IDNODE]){
  if (pos_ruta(id_nodo_from, id_nodo_to)>0){
    return 1;
  } 
  return 0;
  
}

uint8_t existe_ruta(char id_nodo_from[SIZE_IDNODE], char id_nodo_to[SIZE_IDNODE], bool update_route, struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas,struct rutas_blacklisted_t (&blacklist_routes)[MAX_NODES_BLACKLIST],uint8_t &total_rutas_blacklist){
 
 uint8_t pos_route=pos_ruta(id_nodo_from, id_nodo_to);

  // nuevo vecino de la tabla de vecinos
      uint8_t rpta1;
      if (!(compare_char(id_nodo_from,id_node))){ 
           rpta1=create_neighbor(id_nodo_from,vecinos,total_vecinos,blacklist_nodes,total_nodos_blacklist);
      } 
      if (!(compare_char(id_nodo_to,id_node))){
          rpta1=create_neighbor(id_nodo_to,vecinos,total_vecinos,blacklist_nodes,total_nodos_blacklist);
      }
      
  if (update_route){
     if (pos_route>0){
              routeTable[pos_route].age=millis();      
     } else {
        // no existe la ruta, se crea una nueva ruta
      
        // nueva ruta en la tabla de rutas
        nodo_t nodo1;
        nodo_t nodo2;
        rutas_t nueva_ruta;
        copy_array_locha(id_nodo_from, nodo1.id, SIZE_IDNODE);
        copy_array_locha(id_nodo_to, nodo2.id, SIZE_IDNODE);
        create_route(nodo1, nodo2, nodo2,vecinos,total_vecinos, blacklist_nodes,total_nodos_blacklist ,routeTable,total_rutas,blacklist_routes,total_rutas_blacklist);
        DEBUG_PRINTLN(F("ruta creada correctamente"));  
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
uint8_t update_route_age(char id_nodo_from[SIZE_IDNODE], char id_nodo_to[SIZE_IDNODE], struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas,struct rutas_blacklisted_t (&blacklist_routes)[MAX_NODES_BLACKLIST],uint8_t &total_rutas_blacklist){
  uint8_t respuesta=existe_ruta(id_nodo_from, id_nodo_to, true,routeTable,total_rutas,blacklist_routes,total_rutas_blacklist);
  return respuesta;
}

// create a new route on memory  
uint8_t create_route(nodo_t origen, nodo_t next_neighbor, nodo_t destino,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t (&blacklist_nodes)[MAX_NODES_BLACKLIST], uint8_t &total_nodos_blacklist , struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas,struct rutas_blacklisted_t (&blacklist_routes)[MAX_NODES_BLACKLIST],uint8_t &total_rutas_blacklist ){
  uint8_t i;
  rutas_t nueva_ruta;
  bool ejecute_correctamente=true;



  if (!(existe_ruta(origen.id,destino.id))){ 
// solo se agregan las rutas si origen y destino son distintos de vacio
  if ((((String)origen.id).length()>0) and (((String)destino.id).length()>0)){
  // se verifica que no exista previamente la ruta o el inverso de la ruta en las tablas
    for (i = 0; i < total_rutas; i++) {
      if ((compare_char(routeTable[i].origen.id,origen.id))and(compare_char(routeTable[i].destino.id,destino.id))and(compare_char(routeTable[i].next_neighbor.id,next_neighbor.id))){
          ejecute_correctamente=false;
          break;
      }
      if ((compare_char(routeTable[i].origen.id,destino.id))and(compare_char(routeTable[i].destino.id,origen.id))and(compare_char(routeTable[i].next_neighbor.id,next_neighbor.id))){
          ejecute_correctamente=false;
          break;
      }
    }

  } else { 
    // no se crea ruta cuando no se conoce el origen o el destino
   ejecute_correctamente=false;
  }
  
// si origen o destino es un nodo blacklisted no se crea la ruta
for (i = 0; i <= total_nodos_blacklist; i++) {
    if (compare_char(blacklist_nodes[i].id,origen.id)){
          ejecute_correctamente=false;
          break;
    }
    if (compare_char(blacklist_nodes[i].id,destino.id)){
          ejecute_correctamente=false;
          break;
    }
    if (compare_char(blacklist_nodes[i].id,next_neighbor.id)){
          ejecute_correctamente=false;
          break;
    }
}

// si la ruta esta blacklisted no se permite crearla
for (i = 0; i <= total_rutas_blacklist; i++) {
  
  if ((compare_char(blacklist_routes[i].from,origen.id))and(compare_char(blacklist_routes[i].to,destino.id))){
     ejecute_correctamente=false;
     break;
  }
  if ((compare_char(blacklist_routes[i].to,origen.id))and(compare_char(blacklist_routes[i].from,destino.id))){
     ejecute_correctamente=false;
     break;
  }
  if ((compare_char(blacklist_routes[i].from,origen.id))and(compare_char(blacklist_routes[i].to,next_neighbor.id))){
     ejecute_correctamente=false;
     break;
  }
   if ((compare_char(blacklist_routes[i].to,origen.id))and(compare_char(blacklist_routes[i].from,next_neighbor.id))){
     ejecute_correctamente=false;
     break;
  }
}
  
  if (ejecute_correctamente){
      DEBUG_PRINT("Creando ruta desde:");
      DEBUG_PRINT(origen.id);
    DEBUG_PRINT("-hasta:");
    DEBUG_PRINTLN(destino.id);
      nueva_ruta.origen=origen;
      nueva_ruta.destino=destino;
      nueva_ruta.next_neighbor=next_neighbor;
      if(strcmp(nueva_ruta.destino.id, nueva_ruta.next_neighbor.id) == 0){
        nueva_ruta.hops=1;  // es un vecino inmediato
      } else {
        nueva_ruta.hops=1;  // se desconoce la cantidad de saltos
      }
      
      nueva_ruta.age=millis();
      routeTable[total_rutas+1]=nueva_ruta;
      total_rutas++;
      return 0;
  } else {
    return 1;
  }
  
} else {
  // la ruta existe previamente
  return 1;
}
}

uint8_t delete_route_by_id(uint8_t id_to_delete, struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
   uint8_t i;
   if (id_to_delete>0){
      
      for (i = id_to_delete; i < total_rutas; i++) {
          routeTable[i]=routeTable[i+1];
      }
      total_rutas--;
     
     }
     return 0;
}

uint8_t delete_route(char id_nodo_from[SIZE_IDNODE], char id_nodo_to[SIZE_IDNODE], struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
    uint8_t i;
    uint8_t j;
    bool encontro_ruta=false;
    // se busca si existe en la tabla de rutas que id tiene y se elimina
    DEBUG_PRINT("Borrando desde:");
    DEBUG_PRINT(id_nodo_from);
    DEBUG_PRINT("-Borrando hasta:");
    DEBUG_PRINTLN(id_nodo_to);
     for (i = 0; i <= total_rutas; i++) {
        if ((compare_char(routeTable[i].origen.id,id_nodo_from))and(compare_char(routeTable[i].destino.id,id_nodo_to))){
            encontro_ruta=true;
            j=i;
            break;
        }
        // y la inversa
        if ((compare_char(routeTable[i].origen.id,id_nodo_to))and(compare_char(routeTable[i].destino.id,id_nodo_from))){
            encontro_ruta=true;
            j=i;
            break;
        }
        // se borran las rutas intermedias que puedan coincidir con el criterio de from y to
        if ((compare_char(routeTable[i].origen.id,id_nodo_from))and(compare_char(routeTable[i].next_neighbor.id,id_nodo_to))){
            encontro_ruta=true;
            j=i;
            break;
        }
        if ((compare_char(routeTable[i].destino.id,id_nodo_from))and(compare_char(routeTable[i].next_neighbor.id,id_nodo_to))){
            encontro_ruta=true;
            j=i;
            break;
        }
        // y las inversas
         if ((compare_char(routeTable[i].origen.id,id_nodo_to))and(compare_char(routeTable[i].next_neighbor.id,id_nodo_from))){
            encontro_ruta=true;
            j=i;
            break;
        }
        if ((compare_char(routeTable[i].destino.id,id_nodo_to))and(compare_char(routeTable[i].next_neighbor.id,id_nodo_from))){
            encontro_ruta=true;
            j=i;
            break;
        }
        
     }
     if (encontro_ruta){
       uint8_t rpta=delete_route_by_id(j,routeTable,total_rutas);   // se borra y se invoca recursivamente para garantizar que no queden otras rutas
       rpta=delete_route(id_nodo_from, id_nodo_to,routeTable,total_rutas);
     } else {
      DEBUG_PRINTLN(F("El nodo no tiene una ruta hacia ese destino"));
     }
     
     
     return 0;
    
}

uint8_t delete_neighbor(String id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos){
   char nombre_temporal[SIZE_IDNODE];
   uint8_t i;
   uint8_t j;
   id_node_neighbor.toCharArray(nombre_temporal, SIZE_IDNODE);
   bool encontre=false;
   for (i = 1; i <= total_vecinos; i++) {
        if (compare_char(vecinos[i].id,nombre_temporal)){
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
   uint8_t rpta=delete_route(id_node,nombre_temporal,routeTable,total_rutas);
   return 0;
                  
}

// create a new neighbor on memory  
uint8_t create_neighbor(char* id_node_neighbor,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, struct nodo_t blacklist[MAX_NODES_BLACKLIST], uint8_t total_nodos_blacklist ){
                  nodo_t nodo_vecino;
                  uint8_t i;
                  bool permitir_agregar=true;
                  char *pChar = (char*)"";
                  // no se permiten vecinos sin id
                  
                        if (!(compare_char(id_node_neighbor,pChar))){
                  
                       // se verifica que no exista previamente
                       if (es_vecino(id_node_neighbor)){
                        permitir_agregar=false;
                       }
                       for (i = 1; i <= total_vecinos; i++) {
                           if (compare_char(vecinos[i].id,id_node_neighbor)){
                                // existe previamente, no se crea de nuevo
                                permitir_agregar=false;
                                break;
                            }
                       }
                       
                        // se verifica que no exista en blacklist de nodos 
                       for (i = 1; i <= total_nodos_blacklist; i++) {
                            if (compare_char(blacklist[i].id,id_node_neighbor)){
                                // como esta en blacklist no se le permite agregar como un vecino valido
                                permitir_agregar=false;
                                break;
                            }
                       }
                        } else {
                          permitir_agregar=false;
                        }
                  
                 if (es_vecino(id_node_neighbor)){ 
                  permitir_agregar=false;
                 }
                 if ((compare_char(id_node_neighbor,pChar))){
                  permitir_agregar=false;
                 }
                 if (permitir_agregar){
                      copy_array_locha(id_node_neighbor, nodo_vecino.id, SIZE_IDNODE);
                      
                     if (!(compare_char(nodo_vecino.id ,pChar))){
                      if (!(es_vecino(nodo_vecino.id))){
                          total_vecinos++;
                          vecinos[total_vecinos] = nodo_vecino;
                          // aqui hay que agregar la ruta en caso de que no exista
                            
                      } 
                     }
                    return 0;
                 } else {
                  
                    return 1;
                 }
     }

// funcion para buscar el vecino con mas rutas activas
char* vecino_con_mas_rutas(rutas_t routeTable[MAX_ROUTES],uint8_t total_rutas){
  uint8_t i;
  uint8_t j;
  uint8_t total_rutas_tmp=0;
  uint8_t total_rutas_vecino_con_mas_rutas=0;
  uint8_t max_rutas=0;
  char* nombre_vecino;
  char* nombre_vecino_con_mas_rutas;
  
  DEBUG_PRINTLN(F("entrando a vecino con mas rutas"));
  
   for (i = 1; i <= total_rutas; i++) {
      total_rutas_tmp=0;
      if (!(compare_char(routeTable[i].origen.id,""))){
        DEBUG_PRINT("vecino a verificar:");
        DEBUG_PRINT((String)routeTable[i].origen.id);
          copy_array_locha(routeTable[i].origen.id, nombre_vecino, SIZE_IDNODE);
          DEBUG_PRINT("copiado");
          for (j = 1; j <= total_rutas; j++) {
            DEBUG_PRINT("comparando");
            DEBUG_PRINT((String)j);
            if (compare_char(routeTable[j].origen.id,nombre_vecino)){ 
              DEBUG_PRINT("es igual al origen");
                total_rutas_tmp++;
            } else { 
              DEBUG_PRINT("voy->");
              if (compare_char(routeTable[j].destino.id,nombre_vecino)){ 
                DEBUG_PRINT("es igual al destino");
                total_rutas_tmp++;
              } else {
                DEBUG_PRINT("dale->");
                if (!(compare_char(routeTable[i].next_neighbor.id,""))){
                  DEBUG_PRINT("pase->");
                  if (compare_char(routeTable[j].next_neighbor.id,nombre_vecino)){ 
                    DEBUG_PRINT("es igual al proximo");
                    total_rutas_tmp++;
                  }
                }
              }
            }
            DEBUG_PRINT("sigoooo");
          }
          DEBUG_PRINT(F("Total de rutas de este vecino:"));
          DEBUG_PRINTLN((String)total_rutas_tmp);
          if (total_rutas>total_rutas_vecino_con_mas_rutas){
            copy_array_locha(nombre_vecino, nombre_vecino_con_mas_rutas, SIZE_IDNODE);
            total_rutas_vecino_con_mas_rutas=total_rutas_tmp;
          }
      }
   }

   
   DEBUG_PRINTLN(F("saliendo de vecino con mas rutas"));
  return nombre_vecino_con_mas_rutas;
  
}


// coloca el mensaje recibido en Buffer_packet a la cola de mensajes salientes, ubicandolo segun su tipo/prioridad en la posicion de la cola de mensajes que le corresponda
uint8_t packet_to_send(packet_t packet_temp, message_queue_t (&mensajes_salientes_tmp)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp){
  // por ahora solo se agrega a la cola de paquetes salientes
  uint8_t rptsx;
  uint8_t i;
  bool exist_route=false;
  char* vecino_mas_conectado;
  bool send_route_request=false;
  
  // se ubica la ruta que va a usar el packet en curso
  //1) se busca en la tabla de rutas
  //2) si no se encuentra, se busca al vecino que tenga mas rutas hacia otros nodos en la tabla de rutas
  
   for (i = 1; i <= total_rutas; i++) {
        if (compare_char(routeTable[i].origen.id,packet_temp.header.to)){
            copy_array_locha(routeTable[i].origen.id, packet_temp.header.next_neighbor, SIZE_IDNODE);
            exist_route=true;
            break;
        }
        if (compare_char(routeTable[i].destino.id,packet_temp.header.to)){
            copy_array_locha(routeTable[i].destino.id, packet_temp.header.next_neighbor, SIZE_IDNODE);
            exist_route=true;
            break;
        }
   }
  if (!exist_route){
    // se busca el vecino con mas rutas
    vecino_mas_conectado=vecino_con_mas_rutas(routeTable, total_rutas);
    if (!(compare_char(vecino_mas_conectado,NULL))){
        copy_array_locha(vecino_mas_conectado, packet_temp.header.next_neighbor, SIZE_IDNODE);
        // a los demas vecinos se les puede enviar una valixa para ir buscando info con un packet ROUTE tipo RREQ 
        send_route_request=true;
    } 
  }
  
  message_queue_t nuevo_mensaje_en_cola;
  nuevo_mensaje_en_cola.paquete=packet_temp;
  nuevo_mensaje_en_cola.prioridad=1;
  nuevo_mensaje_en_cola.retries=0;
  nuevo_mensaje_en_cola.retry_timestamp=millis();
   if (total_mensajes_salientes_tmp<=MAX_MSG_QUEUE){
       total_mensajes_salientes_tmp=total_mensajes_salientes_tmp+1;
       mensajes_salientes_tmp[total_mensajes_salientes_tmp]=nuevo_mensaje_en_cola;
      
     
      DEBUG_PRINTLN(F("y se coloco en cola:"));
      rptsx=show_packet(nuevo_mensaje_en_cola.paquete,false);
      DEBUG_PRINTLN(F("Packet queue succesfully"));
  } else {
    // esta la cola de mensajes salientes llena, no se puede colocar mas nada alli
    
    return 1;
  }

if (send_route_request){
  // se envian solicitudes de info de ruta a los demas nodos vecinos, pero solo si packet origen distinto a id_node
}
  
  return 0;
}


// broadcast BYE packet to all neighbours
void broadcast_bye(char* id_node,struct nodo_t (vecinos)[MAX_NODES], uint8_t total_vecinos, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes){
    // se envia un packet para liberar recursos en los vecinos
    packet_t new_packet;
    uint8_t rpta;
    uint8_t i;
    
    for (i = 1; i <= total_vecinos; i++) {         
            new_packet=create_packet(id_node, BYE,id_node , vecinos[i].id, "","","");
            rpta=packet_to_send(new_packet,mensajes_salientes,total_mensajes_salientes);
    }
}


// funcion para proesar un mensaje BLE incoming
void BLE_incoming(char* uid2,char* msg_ble, char* timemsg, char* hash_msg, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp2){
  uint8_t i;
  uint8_t rpta;
  char *pChar = (char*)"";
  // si es un mensaje tipo broadcast se envia a todos los vecinos 
  DEBUG_PRINTLN(F("Starting BLE_incoming"));
  pChar=(char*)"broadcast";
    if (compare_char(uid2,pChar)){ 
       DEBUG_PRINTLN(F("its a broadcast"));
       // se envia el packet a todos los vecinos
       if (total_vecinos>0){ 
         for (i = 1; i <= total_vecinos; i++) {
          DEBUG_PRINTLN(F("enviando packet al vecino:"));
          DEBUG_PRINTLN(vecinos[i].id);
          // se arma el packet y se envia a cada vecino
          pChar=(char*)"MSG";
          if (existe_ruta(id_node,vecinos[i].id)){
            // solo si existe la ruta se envia al vecino el broadcast, asi se evita enviar packets a rutas blacklisted
              packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e(pChar), id_node, vecinos[i].id,"","", msg_ble);
              rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes_tmp2);
          } else {
            // no existe la ruta, se usa otro vecino para enviarse
            // se busca el vecino mas conectado para enviarlo por alli
            DEBUG_PRINTLN(F("voy a determinar en cual vecino voy a rebotar:"));
            char* el_vecino=vecino_con_mas_rutas(routeTable, total_rutas);
            DEBUG_PRINT("voy a rebotar en:");
            DEBUG_PRINTLN((String)el_vecino);
            packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e(pChar), id_node, vecinos[i].id,el_vecino,"", msg_ble);
            rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes_tmp2);
          }
          if (rpta==1){
                // la cola estaba llena y no se pudo agregar se le manda un mensaje al movil
                //txValue="{\"uid\":\"broadcast\",\"msg\":\"Gtww\",\"time\":1554012641512\",\"status\":NOT_DELIVERED}";
                
                String rpta_str="NOT DELIVERED";
                rpta_str=Json_return_error(rpta_str);
                DEBUG_PRINTLN(rpta_str);
                
                // enviar un mensaje via BLE a los clientes conectados
                // por ahora desactivado
               // txValue=rpta_str.c_str();
              } 
         }
       } else {
        String rpta_str="No tiene vecinos";
        rpta_str=Json_return_error(rpta_str);
          DEBUG_PRINTLN(rpta_str);
          // enviar un mensaje via BLE a los clientes conectados
          // por ahora desactivado
         // txValue=rpta_str.c_str();
       }
    } else {
       DEBUG_PRINT(F("type of packet received:"));
       DEBUG_PRINTLN(String(uid2));
       // por ahora todo lo que origina en BLE es tipo MSG
         pChar=(char*)"";
       if (compare_char(uid2,pChar)){
          pChar = (char*)"MSG";
          packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e(pChar), id_node, uid2,"","", msg);
          rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes_tmp2);
       }
    }
  DEBUG_PRINTLN(F("ready , packet sent to message queue"));
}


// funcion para completar con espacios en blanco al final el contenido de una variable
std::string completar_con_espacios(char* cadena, uint8_t largo){
  std::string cadena_tmp;
  cadena_tmp.append(cadena);
   // se completa con espacios en blanco despues del id_node para que todo mida exactamente lo mismo
                        for (uint8_t jj = 1; jj < largo-cadena_tmp.size(); jj++) {
                            cadena_tmp.append(" ");  
                        }
                        return cadena_tmp;
}


// funcion para serializar el contenido de la tabla vecinos
std::string serialize_vecinos(struct nodo_t (vecinos)[MAX_NODES], uint8_t total_vecinos, uint8_t maximo_caracteres){
    std::string rpta_str="";
    std::ostringstream s;
  // hay que excluir en este serialize, el to y el from del packet que se asume tienen comunicacion
    
    if (total_vecinos>0){ 
          s << (int)total_vecinos;
          rpta_str=s.str();
          for (uint8_t i = 1; i <= total_vecinos; i++) {
                     if (rpta_str.length()+sizeof(vecinos[i].id)>maximo_caracteres){ 
                        break;  // si es mayor al largo del payload se sale del serialize
                     } else {
                        rpta_str.append(completar_con_espacios(vecinos[i].id,(uint8_t)SIZE_IDNODE)); 
                     }
                     
                     
           }
    }
    return rpta_str;
}

// recibe X2NODO1NODO23NODO1NODO2NODO3  donde X es el total de rutas y 2 o 3 es la cantidad de nodos que componen la ruta
// prcedente de std::string serialize_rutas() y el resultado se va adicionando a la tabla de rutas_t
void deserialize_rutas(std::string ruta_recibida,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos,struct rutas_t (&routeTable)[MAX_ROUTES], uint8_t &total_rutas){
  
}

std::string serialize_rutas(struct nodo_t (vecinos)[MAX_NODES], uint8_t total_vecinos,struct rutas_t (routeTable)[MAX_ROUTES], uint8_t total_rutas, uint8_t maximo_caracteres){
    // si maximo_caracteres=0 se devuelve un solo std::string con todo
    // formato a devolver X2NODO1NODO23NODO1NODO2NODO3  donde X es el total de rutas y 2 o 3 es la cantidad de nodos que componen la ruta
    std::string rpta_str="";
    std::string id_node_tmp;
    std::ostringstream s;
    uint8_t j;
    
  // hay que excluir la ruta entre origen y destino del packet en curso que se asume como ya conocida y no necesita ser compartida
        if (total_rutas>0){ 
          s << (int)total_rutas;
          rpta_str=s.str();
              for (uint8_t i = 1; i <= total_rutas; i++) {
                if ((rpta_str.length()+sizeof(routeTable[i].origen.id))>maximo_caracteres){ 
                    if (maximo_caracteres!=0){  // cuando se recibe 0 es porque se debe devolver toda la cadena completa de rutas
                        break;  // si es mayor al largo del payload se sale del serialize
                    }
                } 
                if (compare_char(routeTable[i].next_neighbor.id,routeTable[i].destino.id)){
                          rpta_str.append("2");
                        } else {
                          rpta_str.append("3");
                        }
                        id_node_tmp.append(completar_con_espacios(routeTable[i].origen.id,(uint8_t)SIZE_IDNODE));

                        rpta_str.append(completar_con_espacios(routeTable[i].next_neighbor.id,(uint8_t)SIZE_IDNODE)); 
                         if (!(compare_char(routeTable[i].next_neighbor.id,routeTable[i].destino.id))){
                                rpta_str.append(completar_con_espacios(routeTable[i].destino.id,(uint8_t)SIZE_IDNODE));    
                         }
                     }
            }
              
        


    return rpta_str;
}
