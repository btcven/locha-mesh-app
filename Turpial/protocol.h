/**
 * @file protocol.h
 * @author Locha.io
 * @brief  protocolo para intercambio de packet en LochaMesh
 * @version 0.1
 * @date 2019-04-27
 * 
 * @copyright Copyright (c) 2019
 * 
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
