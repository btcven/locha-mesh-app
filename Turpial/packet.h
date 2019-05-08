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
  EMPTY=0B0000,
  DATA=0B0001,
  ROUTING=0B0010,
  SECURITY=0B0011
} packet_type_e;

/**
 * @brief subpacket routing type
 * 
 */
typedef enum
{
  EMPTY_ROUTING=0B0000,
  HELLO=0B0001,
  JOIN=0B0010,
  GOSSIP=0B0011,
  ROUTE=0B0100,
  BYE=0B0101
} subpacket_routing_type_e;

/**
 * @brief subpacket data type
 * 
 */
typedef enum
{
  EMPTY_DATA=0B0000,
  MSG=0B0001,
  TXN=0B0010,
  BINARY=0B0011,
  ACK=0B0100,
  NOT_DELIVERED=0B0101
} subpacket_data_type_e;

/**
 * @brief subpacket security type
 * 
 */
typedef enum
{
  EMPTY_SECURITY=0B0000,
  START=0B0001,
  TEST=0B0010,
  RESPONSE=0B0011
} subpacket_security_type_e;

/**
 * @brief response when a packet can't be delivered.
 * 
 */
typedef enum
{
  EMPTY_NOT_DELIVERED=0B0000,
  BLE_NOT_CONNECTED=0B0001,
  MAX_RETRIES_REACHED=0B0010
} not_delivered_type_e;

/**
 * @brief only 1 data type per packet
 * 
 */
typedef union {
  subpacket_routing_type_e routing_type :4;
  subpacket_data_type_e data_type :4;
  subpacket_security_type_e security_type :4;
} subtype_u;

/**
 * @brief Header of packets
 * 
 */
typedef struct
{
  packet_type_e packet_type:4;     // tipo de packet: corresponde a packet_type_e
  subtype_u packet_sub;  // corresponde al subtipo de packet dependiendo del valor type puede ser subpacket_routing_type_e , subpacket_data_type_e o subpacket_security_type_e
  char *from;   // origen
  char *to;     // destino
  char *next_neighbor;   // siguiente vecino
  char *checksum_data;     // hash160 first 6 digits
  unsigned long timestamp;      // epoch timestamp
} packet_header_t;


/**
 * @brief packet body could be simple: only payload and payload size, or splitted: with packet number/total of packets, not delivery response, and payload w/ length
 * 
 */
typedef struct
{
  uint16_t packet_number;        // packet number or total 
  uint16_t packet_total;         // total of packets compossing full msg or file , max 65535, file size max: 65535 x sizeof(payload) = 11403090 bytes ~~11Mb 
  not_delivered_type_e not_delivered_type;
  uint8_t payload_length; // 1 byte
  char *payload;
} packet_body_data_splitted_t;

/**
 * @brief packet body simple : only payload and length
 * 
 */
typedef struct
{
  uint8_t payload_length; // 1 byte
  char *payload;
} packet_body_data_t;

/**
 * @brief only 1 packet body type per packet
 * 
 */
typedef union {
  packet_body_data_t body_data;
  packet_body_data_splitted_t body_data_splitted;
} body_data_u;

/**
 * @brief packet definition
 * 
 */
typedef struct
{
  packet_header_t header;
  body_data_u body;
} packet_t;


/**
 * @brief: from packet_t to uint8_t array
 * 
 * @param target: pointer to uint8_t array
 * @param source: source packet
 * @param s_size: size of source packet 
 */
void packet_to_char(uint8_t *target, packet_t source, size_t s_size);

/**
 * @brief from uint8_t array to packet_t
 * 
 * @param target; pointer to an empty packet
 * @param source; pointer to uint8_t array 
 * @param s_size: size of uint8_t array
 */
void char_to_packet(packet_t *target, uint8_t *source, size_t s_size);

packet_type_e convertir_str_packet_type_e(char* type_recibido);
char* convertir_packet_type_e_str(packet_type_e type_recibido);

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
packet_t create_packet(char *id_node, packet_type_e tipo_packet, subtype_u subtipo_packet, char *from, char *to, char *next_neighbor, char *checksum_data, char *payload, uint16_t packet_number, uint16_t packet_total);

packet_t construct_packet_HELLO(char *id_node,char *from);
packet_t construct_packet_JOIN(char *id_node,char *to);

void show_packet(packet_t packet_rx,const char *TAG);

/**
 * @brief 
 * 
 * @param mensaje 
 * @return std::string 
 */
std::string Json_return_error(std::string mensaje,std::string uid_mensaje);

/**
 * @brief 
 * 
 * @param mensaje 
 * @return std::string 
 */
std::string Json_return_msg(std::string mensaje,std::string uid_mensaje);

#endif // PACKET_H
