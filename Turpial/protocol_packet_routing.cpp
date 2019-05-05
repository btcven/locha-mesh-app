  /**
 * @file protocol_packet_routing.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-26
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
  
  #include <Arduino.h>
  #include "protocol_packet_routing.h"
  #include "hal/hardware.h"
  #include "lang/language.h"
  #include "memory_def.h"
  #include "general_utils.h"
  #include "packet.h"

// manejo de packets  HELLO
void protocol_incoming_PACKET_ROUTING_HELLO(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    // se verifica si el remitente esta en blacklist
    // se verifica si la ruta o la ruta inversa al remitente esta en blacklist
    // se verifica si NO es vecino (packets HELLO only between direct neigbour and cant be relay)
    // se crea el vecino
    // se crea la ruta
    // se devuelve un packet ROUTING JOIN
    
}

// manejo de packets  JOIN
void protocol_incoming_PACKET_ROUTING_JOIN(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    
    
}

// manejo del packets  GOSSIP
void protocol_incoming_PACKET_ROUTING_GOSSIP(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    
    
}

// manejo del packets  ROUTE
void protocol_incoming_PACKET_ROUTING_ROUTE(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    
    
}

// manejo del packets  BYE
void protocol_incoming_PACKET_ROUTING_BYE(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    
    
}
  
void protocol_incoming_PACKET_ROUTING(char id_node[SIZE_IDNODE], packet_t packet_temporal){
    // se clasifica se acuerdo al subtipo de packet recibido
      switch (packet_temporal.header.packet_sub.routing_type)
            {
            case EMPTY_ROUTING:
                break;
            case HELLO:
                protocol_incoming_PACKET_ROUTING_HELLO(id_node, packet_temporal);
                break;
            case JOIN:
                protocol_incoming_PACKET_ROUTING_JOIN(id_node, packet_temporal);
                break;
            case GOSSIP:
                protocol_incoming_PACKET_ROUTING_GOSSIP(id_node, packet_temporal);
                break;
            case ROUTE:
                protocol_incoming_PACKET_ROUTING_ROUTE(id_node, packet_temporal);
                break;
            case BYE:
                protocol_incoming_PACKET_ROUTING_BYE(id_node, packet_temporal);
                break;
            }
}
