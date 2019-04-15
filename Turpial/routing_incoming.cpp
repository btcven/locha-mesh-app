/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

// declaracion de librerias
#include <Arduino.h>
#include "routing_incoming.h"
#include "hal/hardware.h"
#include "memory_def.h"
#include "general_functions.h"
#include "boards_def.h"
#include "packet.h"
#include "route.h"
#include "radio.h"
#include "debugging.h"

//declaracion de variables
// variables para trasmision BLE
extern std::string rxValue;
extern std::string txValue;
// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;
extern int Lora_RSSI;
extern int Lora_SNR;

extern char* id_node; // id unico del nodo
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting; 
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

// funciones para enrutamiento de paquetes entrantes, dado un tipo de packet_t.type 
// se usa la funcion process_received_packet para determinar como va a ser procesado el paquete recibido
// para cada tipo de paquete existe una funcion uint8_t routing_incoming_PACKET_XXX()
// todas devuelven 0 a menos que consigan algun error

uint8_t routing_incoming_PACKET_MSG(char id_node[16], packet_t packet_received){
 // char* char_vacio= malloc(1);
 // *char_vacio ;
//  char_vacio="";
char *pChar = (char*)"";
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual


      // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
      //DEBUG_PRINT(F("Empezando a procesar el packet"));
      
      if ((compare_char(packet_received.header.to,id_node))or((compare_char(packet_received.header.to,pChar)))){
       
        // se devuelve un packet_ACK por la misma ruta al origen para notificar la recepcion
        
            packet_t new_packet=create_packet(id_node, ACK, packet_received.header.from, id_node, packet_received.body.payload);
            DEBUG_PRINT(F("Se esta procesando routing_incoming_PACKET_MSG:"));
            #ifdef DEBUG
              show_packet(packet_received, true);
            #endif
            
              DEBUG_PRINTLN("");
              DEBUG_PRINT(F("Colocando mensaje al BLE:"));
                // se manda al BLE en formato Json
                // modificado por ahora mientras solventamos lo de la limitacion de 20 char en BLE
             //   String hacia_el_ble=Json_return_msg((String)packet_received.body.payload);
                String hacia_el_ble=(String)packet_received.body.payload;
                txValue=hacia_el_ble.c_str();
              DEBUG_PRINT(hacia_el_ble);
              DEBUG_PRINT(F("-largo del mensaje enviado:"));
              DEBUG_PRINT((String)hacia_el_ble.length());
            
            uint8_t rptas=packet_to_send(new_packet,mensajes_salientes,total_mensajes_salientes);  // se envia a la cola de mensajes salientes
            DEBUG_PRINTLN(F("se actualiza el age de la ruta"));
            // se actualiza el age de la ruta desde el origen al destino y si no existe se crea
            update_route_age(packet_received.header.from, packet_received.header.to);
      } else {
            // el paquete no es para mi, pero tengo que hacerle relay a mis vecinos
            // busco si tengo una ruta entre mi nodo y el destino del paquete (y se actualiza el age de la ruta al conseguirla o se crea si no existe)
            if (existe_ruta(id_node, packet_received.header.to,true)){
           
                packet_t new_packet;
                new_packet=create_packet(id_node, ACK, packet_received.header.from, packet_received.header.to, Buffer_packet.body.payload);
                uint8_t rptas=packet_to_send(new_packet,mensajes_salientes,total_mensajes_salientes);  // se envia a la cola de mensajes salientes
            } else {
              // si no existe ruta, falta determinar si me voy random por cualquiera de los nodos para intentar
            }
      
      }
  
  return 0;
}


// cuando se recibe un packet HELLO se devuelve un packet JOIN
// TODO: en el payload debe contener la tabla de vecinos,rutas hasta donde alcance el limite de tamaño
uint8_t routing_incoming_PACKET_JOIN(char id_node[16], packet_t packet_received){
  // nuevo vecino de la tabla de vecinos
  uint8_t rpta1=create_neighbor(packet_received.header.from,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
  // nueva ruta en la tabla de rutas
  nodo_t nodo1;
  nodo_t nodo2;
  rutas_t nueva_ruta;
  copy_array_locha(packet_received.header.to, nodo1.id, 16);
  copy_array_locha(packet_received.header.from, nodo2.id, 16);
  create_route(nodo1, nodo2, nodo2,vecinos,total_vecinos, blacklist,total_nodos_blacklist ,routeTable,total_rutas);
  return 0;
}

// si se desconecta, apaga o sale de la red, o se hace un reset se envia un BYE para que los vecinos liberen recursos asociados a este nodo
uint8_t routing_incoming_PACKET_BYE(char id_node[16], packet_t packet_received){
  // borra al vecino de la tabla de vecinos
  uint8_t i;
  uint8_t is_MSG=0;
   for (i = 1; i <= total_vecinos; i++) {
      if (compare_char(vecinos[i].id,packet_received.header.from)){
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
    if (compare_char(routeTable[i].origen.id, packet_received.header.from)){
        is_MSG=i;
        break;
    }
    if (compare_char(routeTable[i].destino.id,packet_received.header.from)){
        is_MSG=i;
        break;
    }
    if (compare_char(routeTable[i].origen.id,packet_received.header.to)){
        is_MSG=i;
        break;
    }
    if (compare_char(routeTable[i].destino.id,packet_received.header.to)){
        is_MSG=i;
        break;
    }
    if (compare_char(routeTable[i].next_neighbor.id,packet_received.header.to)){
        is_MSG=i;
        break;
    }
     if (compare_char(routeTable[i].next_neighbor.id,packet_received.header.from)){
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

uint8_t routing_incoming_PACKET_ROUTE(char id_node[16], packet_t packet_received){
  // este tipo de paquete permite adicionar nuevas rutas a la tabla de rutas
  
  Serial.println(F("se recibio un packet route"));
  return 0;
}

// TODO
uint8_t routing_incoming_PACKET_NOT_DELIVERED(char id_node[16], packet_t packet_received){
  // si no es para mi se reenvia el paquete a los vecinos por la ruta donde origino
  Serial.println(F("se recibio un packet not delivered"));
  return 0;
}

// TODO
uint8_t routing_incoming_PACKET_GOSSIP(char id_node[16], packet_t packet_received){
  // 
  Serial.println(F("se recibio un packet gossip"));
  return 0;
}

// TODO
uint8_t routing_incoming_PACKET_TXN(char id_node[16], packet_t packet_received){
  // 
  Serial.println(F("se recibio un packet txn"));
  return 0;
}
uint8_t routing_incoming_PACKET_HELLO(char id_node[16], packet_t packet_received){
  // 
  nodo_t nodo1;
  nodo_t nodo2;
  rutas_t nueva_ruta;
  std::string payload_join="";
    
  Serial.println(F("se recibio un packet hello"));
  // se crea una ruta al packet que envio el HELLO y se devuelve un PACKET_JOIN
   // nueva ruta en la tabla de rutas
   uint8_t rpta1=create_neighbor(packet_received.header.from,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
  
  // nueva ruta en la tabla de rutas

  copy_array_locha(packet_received.header.to, nodo1.id, 16);
  copy_array_locha(packet_received.header.from, nodo2.id, 16);
  create_route(nodo1, nodo2, nodo2,vecinos,total_vecinos, blacklist,total_nodos_blacklist ,routeTable,total_rutas);

  // ahora se crea la ruta de A->B
  copy_array_locha(packet_received.header.from,packet_received.header.to, 16);
  copy_array_locha(id_node,packet_received.header.from, 16);
  copy_array_locha(packet_received.header.from,  nodo1.id,16);
  copy_array_locha(packet_received.header.to,  nodo2.id,16);
  create_route(nodo1, nodo2, nodo2,vecinos,total_vecinos, blacklist,total_nodos_blacklist ,routeTable,total_rutas);

// se devuelve un packet JOIN
  Serial.println("Se envia un packet Join");
  packet_received.header.type=JOIN;
  copy_array_locha(nodo2.id,packet_received.header.to, 16);
  // copy_array_locha(nodo1.id,packet_received.header.from, 16);

 // se arma como payload el contenido de la tabla vecinos concatenado por el contenido de la tabla rutas
  payload_join=serialize_vecinos(vecinos, total_vecinos,SIZE_PAYLOAD);
  char *cstr = new char[payload_join.length() + 1];
  strcpy(cstr, payload_join.c_str());
  copy_array_locha(cstr,packet_received.body.payload, payload_join.length() + 1);
  //packet_received.body.payload=payload_join.c_str();
  radioSend(packet_serialize(packet_received));
 
  return 0;
}


uint8_t routing_incoming_PACKET_ACK(char id_node[16], packet_t packet_received){
  // se verifica si en los mensajes enviados hay uno que tenga el mismo payload para borrarlo
  uint8_t i;
  uint8_t is_MSG=0;
  Serial.println(F("Packet ACK recibido , se procesa ..."));
   for (i = 1; i <= total_mensajes_waiting; i++) {
    Serial.println("packet received:");
    Serial.print("To:");
    Serial.println((String)packet_received.header.to);
    Serial.print("From:");
    Serial.println((String)packet_received.header.from);
    
      if (((String)mensajes_waiting[i].paquete.header.from==(String)(String)packet_received.header.from)and((String)mensajes_waiting[i].paquete.header.to==(String)packet_received.header.to)){
          // se verifica que sea un ACK de un mensaje tipo MSG
          if (mensajes_waiting[i].paquete.header.type=MSG){
                  // se verifica que tenga el mismo payload (esto deberia ser con el hash pero por ahora a efectos del demo se usa solo el mismo payload)
                if ((String)mensajes_waiting[i].paquete.body.payload==(String)packet_received.body.payload){
                  // se recibio el ACK de que el mensaje MSG fue recibido correctamente
                  is_MSG=i;
                  break;
                }
           }
        }
        }
          if (is_MSG>0){
            Serial.println(F("Procedo a eliminar el packet de la cola waiting..."));
             // se borra el mensaje de la tabla de mensajes_salientes
              for (i = is_MSG; i < total_mensajes_waiting; i++) {
                  mensajes_waiting[i]=mensajes_waiting[i+1];
              }
              total_mensajes_waiting--;
              
          } else {
            Serial.println(F("No se elimino nada de la cola waiting"));
          }
      DEBUG_PRINTLN(F("ACK del packet recibido exitosamente"));
   
}

void update_rssi_snr(char route_from[16], char route_to[16], int RSSI_received, int SNR_received){
  for (uint8_t xx = 1; xx <= total_rutas; xx++) {
      if ((compare_char(routeTable[xx].origen.id,route_from))and(compare_char(routeTable[xx].next_neighbor.id,route_to))){
        routeTable[xx].RSSI_packet=RSSI_received;
        routeTable[xx].SNR_packet=SNR_received;
        break;
      }
      // si existe como ruta inversa tambien hay que actualizarla
    if ((compare_char(routeTable[xx].origen.id,route_to))and(compare_char(routeTable[xx].next_neighbor.id,route_from))){
        routeTable[xx].RSSI_packet=RSSI_received;
        routeTable[xx].SNR_packet=SNR_received;
        break;
      }
  }
}


// esta funcion procesa el paquete recibido 
void process_received_packet(char id_node[16], packet_t packet_temporal,struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, int RSSI_recibido, int SNR_recibido){
  uint8_t rpta;

  DEBUG_PRINT(F("A new packet incoming, type:"));
  DEBUG_PRINTLN(convertir_packet_type_e_str(packet_temporal.header.type));

      // se verifica que el origen y destino no sea el mismo, para evitar ataques 
    if ((String)packet_temporal.header.from!=(String)packet_temporal.header.to){
      
      switch (packet_temporal.header.type)
      {
      case EMPTY:
            DEBUG_PRINT(F("Node:"));
            DEBUG_PRINT((String)packet_temporal.header.from);
            DEBUG_PRINT(F(" is sending "));
            DEBUG_PRINT(convertir_packet_type_e_str(EMPTY));
            DEBUG_PRINTLN(F(" packets, review it."));
        break;
      case JOIN:
          routing_incoming_PACKET_JOIN(id_node, packet_temporal);
          break;
      case BYE:
          routing_incoming_PACKET_BYE(id_node, packet_temporal);
          break;
      case ROUTE:
          routing_incoming_PACKET_ROUTE(id_node, packet_temporal);
          break;
      case ACK:
          routing_incoming_PACKET_ACK(id_node, packet_temporal);
          break;
      case MSG:
         routing_incoming_PACKET_MSG(id_node, packet_temporal);
         break;
      case HELLO:
       routing_incoming_PACKET_HELLO(id_node, packet_temporal);
          break;
      case GOSSIP:
       routing_incoming_PACKET_GOSSIP(id_node, packet_temporal);
          break;
      case NOT_DELIVERED:
       routing_incoming_PACKET_NOT_DELIVERED(id_node, packet_temporal);
          break;
      default:
        break;
      }
    
      // todo packet incoming actualiza el RSSI y el SNR de la ruta que le corresponda
      update_rssi_snr(packet_temporal.header.from, packet_temporal.header.to, RSSI_recibido, SNR_recibido);
    }
}
