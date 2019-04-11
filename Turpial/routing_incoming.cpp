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


uint8_t routing_incoming_PACKET_MSG(char id_node[16], packet_t packet_received){
  
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual


 // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
 Serial.println("empezando procesando el packet");
if (((String)packet_received.header.to==(String)id_node)or(((String)packet_received.header.to==""))){
  // es un paquete para mi nodo
 // process_received_packet(Buffer_packet);
  // se devuelve un packet_ACK por la misma ruta
  // se arma un packet_ACK
   // packet_header_t header;
     // packet_body_t body;
Serial.println("t, se devuelve un ACK");
     // header.type=ACK;
      //header.from=id_node;
   //   copy_array(id_node, header.from, 16);
   //   copy_array(packet_received.header.from, header.to, 16);
     // header.to=packet_received.header.from;
   //   header.timestamp=millis();
    //  copy_array(Buffer_packet.body.payload, body.payload, 240);
    //  body.payload=Buffer_packet.body.payload;   // aqui deberia devolver el hash y en base al hash validar que efectivamente cuando se reciba el ACK elimine al que corresponda
    
     // new_packet.header=header;
    //  new_packet.body=body;
      //packet_t new_packet=create_packet(id_node, ACK, packet_received.header.from, id_node, Buffer_packet.body.payload);
      
      packet_t new_packet=create_packet(id_node, ACK, packet_received.header.from, id_node, packet_received.body.payload);
      Serial.println("tengo el packet en routing_incoming_PACKET_MSG:");
      show_packet(packet_received, true);
      if ((packet_received.header.type==MSG)or(packet_received.header.type==TXN)){
        Serial.println("");
        Serial.print("colocando mensaje al BLE:");
          // se manda al BLE en formato Json
          // modificado por ahora mientras solventamos lo de la limitacion de 20 char en BLE
          //String hacia_el_ble=Json_return_msg((String)packet_received.body.payload);
          String hacia_el_ble=(String)packet_received.body.payload;
          txValue=hacia_el_ble.c_str();
        Serial.print(hacia_el_ble);
        Serial.print("-largo del mensaje enviado:");
        Serial.println((String)hacia_el_ble.length());
      }
      uint8_t rptas=packet_to_send(new_packet,mensajes_salientes,total_mensajes_salientes);  // se envia a la cola de mensajes salientes
Serial.println("se actualiza el age de la ruta");
      // se actualiza el age de la ruta desde el origen al destino y si no existe se crea
       update_route_age(packet_received.header.from, packet_received.header.to);
} else {
  // el paquete no es para mi, pero tengo que hacerle relay a mis vecinos
  // busco si tengo una ruta entre mi nodo y el destino del paquete (y se actualiza el age de la ruta al conseguirla o se crea si no existe)
  if (existe_ruta(id_node, packet_received.header.to,true)){
   //    packet_header_t header;
  //    packet_body_t body;

 //     header.type=ACK;
 //     copy_array(id_node, header.from, 16);
    //  header.from=id_node;
 //   copy_array(packet_received.header.from, header.to, 16);
     // header.to=packet_received.header.from;
  //    header.timestamp=millis();
  //    header.last_node=id_node;   // este parametro se encarga de determinar que no se devuelva el mismo paquete hacia el origen
      
    //  body=Buffer_packet.body;  
      packet_t new_packet;
     // new_packet.header=header;
     // new_packet.body=body;
      new_packet=create_packet(id_node, ACK, packet_received.header.from, packet_received.header.to, Buffer_packet.body.payload);
      uint8_t rptas=packet_to_send(new_packet,mensajes_salientes,total_mensajes_salientes);  // se envia a la cola de mensajes salientes
  } else {
    // si no existe ruta, falta determinar si me voy random por cualquiera de los nodos para intentar
  }

}
  
  return 0;
}

uint8_t routing_incoming_PACKET_JOIN(char id_node[16], packet_t packet_received){
  // nuevo vecino de la tabla de vecinos
 // si no existe previamente
 //if (!es_vecino(packet_received.header.from)){
 // copy_array_locha(packet_received.header.from, vecinos[total_vecinos+1].id, 16);
  //vecinos[total_vecinos+1]=packet_received.header.from;
//  total_vecinos++;
 //}
  uint8_t rpta1=create_neighbor(packet_received.header.from,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
  // nueva ruta en la tabla de rutas
  nodo_t nodo1;
  nodo_t nodo2;
  rutas_t nueva_ruta;
 // nodo1.id=packet_received.header.to;
  copy_array_locha(packet_received.header.to, nodo1.id, 16);
 // nodo2.id=packet_received.header.from;
   copy_array_locha(packet_received.header.from, nodo2.id, 16);
  create_route(nodo1, nodo2, nodo2);
  return 0;
}

uint8_t routing_incoming_PACKET_BYE(char id_node[16], packet_t packet_received){
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

uint8_t routing_incoming_PACKET_ROUTE(char id_node[16], packet_t packet_received){
  // este tipo de paquete permite adicionar nuevas rutas a la tabla de rutas
  
  Serial.println(F("se recibio un packet route"));
  return 0;
}

uint8_t routing_incoming_PACKET_NOT_DELIVERED(char id_node[16], packet_t packet_received){
  // si no es para mi se reenvia el paquete a los vecinos por la ruta donde origino
  Serial.println(F("se recibio un packet not delivered"));
  return 0;
}
uint8_t routing_incoming_PACKET_GOSSIP(char id_node[16], packet_t packet_received){
  // 
  Serial.println(F("se recibio un packet gossip"));
  return 0;
}
uint8_t routing_incoming_PACKET_TXN(char id_node[16], packet_t packet_received){
  // 
  Serial.println(F("se recibio un packet txn"));
  return 0;
}
uint8_t routing_incoming_PACKET_HELLO(char id_node[16], packet_t packet_received){
  // 
  nodo_t nodo1;
  nodo_t nodo2;
  Serial.println(F("se recibio un packet hello"));
  // se crea una ruta al packet que envio el HELLO y se devuelve un PACKET_JOIN
   // nueva ruta en la tabla de rutas
   uint8_t rpta1=create_neighbor(packet_received.header.from,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
  
  rutas_t nueva_ruta;
  
 // nodo1.id=packet_received.header.to;
  copy_array_locha(packet_received.header.from,packet_received.header.to, 16);
 // nodo2.id=packet_received.header.from;
   copy_array_locha(id_node,packet_received.header.from, 16);
  
  packet_received.header.type=JOIN;
  copy_array_locha(nodo2.id,packet_received.header.to, 16);
 // copy_array_locha(nodo1.id,packet_received.header.from, 16);
  radioSend(packet_serialize(packet_received));


  // ahra se crea la ruta de A->B
  copy_array_locha(packet_received.header.from,  nodo1.id,16);
  copy_array_locha(packet_received.header.to,  nodo2.id,16);
  create_route(nodo1, nodo2, nodo2);
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
  uint8_t xx;
  for (xx = 1; xx <= total_rutas; xx++) {
      if (((String)routeTable[xx].origen.id==(String)route_from)and((String)routeTable[xx].next_neighbor.id==(String)route_to)){
        routeTable[xx].RSSI_packet=RSSI_received;
        routeTable[xx].SNR_packet=SNR_received;
        break;
      }
    
  }
}


// esta funcion procesa el paquete recibido 
void process_received_packet(char id_node[16], packet_t packet_temporal){
  uint8_t rpta;

  DEBUG_PRINT(F("A new packet incoming, type:"));
  DEBUG_PRINTLN(convertir_packet_type_e_str(packet_temporal.header.type));

      // se verifica que el origen y destino no sea el mismo, para evitar ataques 
    if ((String)packet_temporal.header.from!=(String)packet_temporal.header.to){
      
      switch (packet_temporal.header.type)
      {
      case EMPTY:
            DEBUG_PRINT("Node:");
            DEBUG_PRINT((String)packet_temporal.header.from);
            DEBUG_PRINT(" is sending ");
            DEBUG_PRINT(convertir_packet_type_e_str(EMPTY));
            DEBUG_PRINTLN(" packets, review it.");
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
      update_rssi_snr(packet_temporal.header.from, packet_temporal.header.to, Lora_RSSI, Lora_SNR);
    }
}
