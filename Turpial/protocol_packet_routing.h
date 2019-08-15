/**
 * @file protocol_packet_routing.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
  */
#ifndef PROTOCOL_PACKET_ROUTING_H 
#define PROTOCOL_PACKET_ROUTING_H

#include <Arduino.h>
#include "hal/hardware.h"
#include "memory_def.h"
#include "packet.h"
#include "SQLite.h"


void protocol_incoming_PACKET_ROUTING_HELLO(char id_node[SIZE_IDNODE], packet_t packet_temporal, int RSSI_recibido, int SNR_recibido, sqlite3 *db);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_JOIN(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_GOSSIP(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_ROUTE(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING_BYE(char id_node[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_ROUTING(char id_node[SIZE_IDNODE], packet_t packet_temporal, int RSSI_recibido, int SNR_recibido,sqlite3 *db);

#endif // PROTOCOL_PACKET_ROUTING_H
