/**
 * @file packet.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>
#include "memory_def.h"


typedef enum
{
  EMPTY=0,
  JOIN=1,
  BYE=2,
  ROUTE=3,
  ACK=4,
  MSG=5,
  HELLO=6,
  GOSSIP=7,
  NOT_DELIVERED=8,
  TXN=9
  //BIN=10 // binarios para el caso de audios, fotos
} packet_type_e;

// causas de no entrega de packets
typedef enum
{
  EMPTY_NOT_DELIVERED=0,
  BLE_NOT_CONNECTED=1,
  MAX_RETRIES_REACHED=2
} not_delivered_type_e;

typedef struct
{
  packet_type_e type;
  char from[SIZE_IDNODE];
  char to[SIZE_IDNODE];
  // modificacion para Relay
  char next_neighbor[SIZE_IDNODE];
  unsigned long timestamp;
  char hash[SIZE_HASH_MSG];
} packet_header_t;

typedef struct
{
  char payload[SIZE_PAYLOAD];
} packet_body_t;

typedef struct
{
  packet_header_t header;
  packet_body_t body;
} packet_t;



packet_type_e convertir_str_packet_type_e(char* type_recibido);
packet_type_e convertir_int_packet_type_e(uint8_t type_recibido);
char* convertir_packet_type_e_str(packet_type_e type_recibido);
std::string packet_serialize(packet_t packet);
packet_t packet_deserialize(char* received_text);
packet_t create_packet(char* id_node, packet_type_e tipo_packet, char* from, char* to,  char* next_neighbor, char* hash,char* payload);
std::string Json_return_error(std::string mensaje);
std::string Json_return_msg(std::string mensaje);

#endif // PACKET_H
