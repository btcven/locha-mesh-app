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

/**
 * @brief packet type
 * 
 */
typedef enum
{
  EMPTY,
  JOIN,
  BYE,
  ROUTE,
  ACK,
  MSG,
  HELLO,
  GOSSIP,
  NOT_DELIVERED, // esto no seria un tipo de paquete, más bien define un estado.
  TXN,
  BINARY
} packet_type_e;

/**
 * @brief response when a packet can't be delivered.
 * 
 */
typedef enum
{
  EMPTY_NOT_DELIVERED,
  BLE_NOT_CONNECTED, // mas genérico, puede ser union ó enum
  MAX_RETRIES_REACHED
} not_delivered_type_e;

/**
 * @brief 
 * 
 */
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

/**
 * @brief 
 * 
 */
typedef struct
{
  char payload[SIZE_PAYLOAD];
} packet_body_t;

/**
 * @brief 
 * 
 */
typedef struct
{
  packet_header_t header;
  packet_body_t body;
} packet_t;

/**
 * @brief 
 * 
 * @param type_recibido 
 * @return packet_type_e 
 */
packet_type_e convertir_str_packet_type_e(char *type_recibido);

/**
 * @brief 
 * 
 * @param type_recibido 
 * @return packet_type_e 
 */
packet_type_e convertir_int_packet_type_e(uint8_t type_recibido);

/**
 * @brief 
 * 
 * @param type_recibido 
 * @return char* 
 */
char *convertir_packet_type_e_str(packet_type_e type_recibido);

/**
 * @brief 
 * 
 * @param packet 
 * @return std::string 
 */
std::string packet_serialize(packet_t packet);

/**
 * @brief 
 * 
 * @param received_text 
 * @return packet_t 
 */
packet_t packet_deserialize(char *received_text);

/**
 * @brief Create a packet object
 * 
 * @param id_node 
 * @param tipo_packet 
 * @param from 
 * @param to 
 * @param next_neighbor 
 * @param hash 
 * @param payload 
 * @return packet_t 
 */
packet_t create_packet(char *id_node, packet_type_e tipo_packet, char *from, char *to, char *next_neighbor, char *hash, char *payload);

/**
 * @brief 
 * 
 * @param mensaje 
 * @return std::string 
 */
std::string Json_return_error(std::string mensaje);

/**
 * @brief 
 * 
 * @param mensaje 
 * @return std::string 
 */
std::string Json_return_msg(std::string mensaje);

#endif // PACKET_H
