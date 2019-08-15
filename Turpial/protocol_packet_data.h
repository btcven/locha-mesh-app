/**
 * @file protocol_packet_data.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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
 * @brief select processing option based on data_type received
 * only for packets with type=DATA
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_DATA(char id_node[SIZE_IDNODE], packet_t packet_temporal);


#endif // PROTOCOL_PACKET_DATA_H
