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
        if (rpta_rad==0){ 
          // no se transmitio se hace un reintento 
          delay(500);
          rpta_rad=radioSend(msg_to_send);
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
uint8_t es_vecino(char id_nodo[16]){
  uint8_t i;
  for (i = 1; i <= total_vecinos; i++) {
      if ((String)vecinos[i].id==(String)id_nodo){
        return 1;
      }
  }
  return 0;
}

// posicion de la ruta en la tabla de rutas en memoria
uint8_t pos_ruta(char id_nodo_from[16], char id_nodo_to[16]){
  uint8_t i;
  for (i = 1; i <= total_rutas; i++) {
      if (((String)routeTable[i].origen.id==(String)id_nodo_from)and((String)routeTable[i].destino.id==(String)id_nodo_to)){
        return i;
      }
      // el inverso tambien es la misma ruta
      if (((String)routeTable[i].origen.id==(String)id_nodo_to)and((String)routeTable[i].destino.id==(String)id_nodo_from)){
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
  
// solo se agregan las rutas si origen y destino son distintos de vacio
  if ((((String)origen.id).length()>0) and (((String)destino.id).length()>0)){
  // se verifica que no exista previamente la ruta o el inverso de la ruta en las tablas
    for (i = 0; i < total_rutas; i++) {
      if (((String)routeTable[i].origen.id==(String)origen.id)and((String)routeTable[i].destino.id==(String)destino.id)and((String)routeTable[i].next_neighbor.id==(String)next_neighbor.id)){
          ejecute_correctamente=false;
          break;
      }
      if (((String)routeTable[i].origen.id==(String)destino.id)and((String)routeTable[i].destino.id==(String)origen.id)and((String)routeTable[i].next_neighbor.id==(String)next_neighbor.id)){
          ejecute_correctamente=false;
          break;
      }
    }

  } else { 
    // no se crea ruta cuando no se conoce el origen o el destino
   ejecute_correctamente=false;
  }
  if (ejecute_correctamente){
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
uint8_t packet_to_send(packet_t packet_temp, message_queue_t (&mensajes_salientes_tmp)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp){
  // por ahora solo se agrega a la cola de paquetes salientes
  uint8_t rptsx;
  
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
  return 0;
}

// funcion para proesar un mensaje BLE incoming
void BLE_incoming(char* uid2,char* msg_ble, char* timemsg, char* hash_msg, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp2){
  uint8_t i;
  uint8_t rpta;
  // si es un mensaje tipo broadcast se envia a todos los vecinos 
  DEBUG_PRINTLN(F("Starting BLE_incoming"));
  DEBUG_PRINTLN(F("recibi msg:"));
  DEBUG_PRINTLN(msg_ble);
  Serial.print("tengo:");
  Serial.println(uid2);
    if (String(uid2)=="broadcast"){ 
       DEBUG_PRINTLN(F("its a broadcast"));
       if (total_vecinos>0){ 
         for (i = 1; i <= total_vecinos; i++) {
          DEBUG_PRINTLN(F("enviando packet al vecino:"));
          DEBUG_PRINTLN(vecinos[i].id);
          // se arma el packet y se envia a cada vecino
          packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e("MSG"), id_node, vecinos[i].id, msg_ble);
         
          DEBUG_PRINTLN("Packet received:");
          DEBUG_PRINT("type:");
          DEBUG_PRINTLN(convertir_packet_type_e_str(tmp_packet.header.type));
          DEBUG_PRINT("from:");
          DEBUG_PRINTLN(tmp_packet.header.from);
          DEBUG_PRINT("to:");
          DEBUG_PRINTLN(tmp_packet.header.to);
          DEBUG_PRINT("payload:");
          DEBUG_PRINTLN(tmp_packet.body.payload);
         DEBUG_PRINT("timestamp:");
          DEBUG_PRINTLN(tmp_packet.header.timestamp);
         
          
          rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes_tmp2);
          if (rpta==1){
            // la cola estaba llena y no se pudo agregar se le manda un mensaje al movil
            //txValue="{\"uid\":\"broadcast\",\"msg\":\"Gtww\",\"time\":1554012641512\",\"status\":NOT_DELIVERED}";
            txValue="NOT DELIVERED";
          }
         }
       } else {
          DEBUG_PRINTLN(F("This node has no neigbours"));
       }
    } else {
       DEBUG_PRINT(F("its other type of packet:"));
       DEBUG_PRINTLN(String(uid2));
       // por ahora todo lo que origina en BLE es tipo MSG
       if (String(uid2)!=""){
          packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e("MSG"), id_node, uid2, msg);
          rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes_tmp2);
       }
    }
  DEBUG_PRINTLN(F("ready , packet sent to message queue"));
}
