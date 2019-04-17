/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
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

typedef struct
{
  packet_type_e type;
  char from[16];
  char to[16];
  // modificacion para Relay
  char next_neighbor[16];
  unsigned long timestamp;
  char hash[20];
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

class radioPacket
{

public:
  radioPacket(packet_t packet);
  ~radioPacket();
  void serialize();
  void deserialize();

private:
  void msg_handler();
  packet_header_t header;
  packet_body_t body;
  packet_t packet;
};

packet_t create_packet(char* id_node, packet_type_e type, char* from, char* to, char* payload);
packet_type_e convertir_str_packet_type_e(char* type_recibido);
char* convertir_packet_type_e_str(packet_type_e type_recibido);
packet_type_e convertir_int_packet_type_e(uint8_t type_recibido);
String packet_serialize(packet_t packet);
packet_t packet_deserialize_str(String received_text);
packet_t packet_deserialize(char* received_text);
packet_t construct_packet_HELLO(char* from);
String Json_return_error(String mensaje);
String Json_return_msg(String mensaje);

#endif // PACKET_H
