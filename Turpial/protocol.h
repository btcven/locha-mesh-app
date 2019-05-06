/**
 * @file protocol.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef PROTOCOL_H 
#define PROTOCOL_H

#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"

/**
 * @brief 
 * 
 * @param id_node 
 * @param packet_temporal 
 * @param RSSI_recibido 
 * @param SNR_recibido 
 */
void process_received_packet(char id_node[SIZE_IDNODE], packet_t packet_temporal, int RSSI_recibido, int SNR_recibido);

#endif // PROTOCOL_H
