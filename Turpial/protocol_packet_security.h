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
#include "memory_def.h"
#include "packet.h"

void protocol_incoming_PACKET_ROUTING_START(char id_node[SIZE_IDNODE], packet_t packet_temporal);
void protocol_incoming_PACKET_ROUTING_TEST(char id_node[SIZE_IDNODE], packet_t packet_temporal);
void protocol_incoming_PACKET_ROUTING_RESPONSE(char id_node[SIZE_IDNODE], packet_t packet_temporal);
void protocol_incoming_PACKET_SECURITY(char id_node[SIZE_IDNODE], packet_t packet_temporal);

#endif // PROTOCOL_PACKET_SECURITY_H
