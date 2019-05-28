  /**
 * @file protocol_packet_data.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-26
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
  
  #include <Arduino.h>

  #include "hal/hardware.h"
  #include "lang/language.h"
  #include "memory_def.h"
  #include "general_utils.h"
  #include "packet.h"
  #include "SQLite.h"
  #include "tables.h"
 // #include "protocol_packet_data.h"

 extern char *id_node;

// manejo del packets  MSG
void protocol_incoming_PACKET_ROUTING_MSG( packet_t packet_temporal){
    // se verifica si el remitente esta en blacklist
    
}

// manejo del packets  TXN
void protocol_incoming_PACKET_ROUTING_TXN( packet_t packet_temporal){
    
    
}

// manejo del packets  BINARY
void protocol_incoming_PACKET_ROUTING_BINARY( packet_t packet_temporal){
    
    
}

// manejo del packets  ACK
void protocol_incoming_PACKET_ROUTING_ACK( packet_t packet_temporal){
    
    
}

// manejo del packets  NOT_DELIVERED
void protocol_incoming_PACKET_ROUTING_NOT_DELIVERED( packet_t packet_temporal){
    
    
}
  

/**
 * @brief select processing option based on data_type received
 * only for packets with type=DATA
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_DATA(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal){
    // se clasifica se acuerdo al subtipo de packet recibido
      switch (packet_temporal.header.packet_sub.data_type)
            {
            case EMPTY_DATA:
                break;
            case MSG:
                protocol_incoming_PACKET_ROUTING_MSG( packet_temporal);
                break;
            case TXN:
                protocol_incoming_PACKET_ROUTING_TXN( packet_temporal);
                break;
            case BINARY:
                protocol_incoming_PACKET_ROUTING_BINARY( packet_temporal);
                break;
            case ACK:
                protocol_incoming_PACKET_ROUTING_ACK( packet_temporal);
                break;
            case NOT_DELIVERED:
                protocol_incoming_PACKET_ROUTING_NOT_DELIVERED( packet_temporal);
                break;
            }
}
