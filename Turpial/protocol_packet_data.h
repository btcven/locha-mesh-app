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
#include "hal/hardware.h"
#include "memory_def.h"
#include "packet.h"
#include "SQLite.h"


/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_MSG(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_TXN(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_BINARY(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_ACK(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_NOT_DELIVERED(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief select processing option based on data_type received
 * only for packets with type=DATA
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_DATA(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);


#endif // PROTOCOL_PACKET_DATA_H
