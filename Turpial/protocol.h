/**
 * @file protocol.h
 * @author Locha Mesh project developers (locha.io)
 * @brief  protocolo para intercambio de packet en Locha Mesh
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 */

#ifndef PROTOCOL_H 
#define PROTOCOL_H

#include <Arduino.h>
#include "memory_def.h"
#include "packet.h"

/**
 * @brief Process a received packet , is processed based on packet.header.type , each type had their own void to process
 * 
 * @param id_node 
 * @param packet_temporal 
 * @param RSSI_recibido 
 * @param SNR_recibido 
 * @param db 
 */
void process_received_packet(char id_node[SIZE_IDNODE], packet_t packet_temporal, int RSSI_recibido, int SNR_recibido);

#endif // PROTOCOL_H
