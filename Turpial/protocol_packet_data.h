/**
 * @file protocol_packet_data.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef PROTOCOL_PACKET_DATA_H 
#define PROTOCOL_PACKET_DATA_H

#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"
  #include "SQLite.h"

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_MSG(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_TXN(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_BINARY(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_ACK(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_NOT_DELIVERED(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_DATA(char id_node[SIZE_IDNODE], packet_t packet_temporal);


#endif // PROTOCOL_PACKET_DATA_H
