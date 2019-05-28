/**
 * @file protocol_packet_security.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef PROTOCOL_PACKET_SECURITY_H 
#define PROTOCOL_PACKET_SECURITY_H

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
void protocol_incoming_PACKET_SECURITY_START(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_SECURITY_TEST(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_SECURITY_RESPONSE(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 */
void protocol_incoming_PACKET_SECURITY(const char id_node_from[SIZE_IDNODE], packet_t packet_temporal);

#endif // PROTOCOL_PACKET_SECURITY_H
