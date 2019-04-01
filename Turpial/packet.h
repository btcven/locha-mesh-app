/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef PACKET_H 
#define PACKET_H

#include <Arduino.h>


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
  NOT_DELIVERED
} packet_type_e;

typedef struct
{
  packet_type_e type;
  char from[16];
  char to[16];
  unsigned long timestamp;
} packet_header_t;

typedef struct
{
  char payload[240];
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
packet_type_e convertir_str_packet_type_e(String type_recibido);
String convertir_packet_type_e_str(packet_type_e type_recibido);
String packet_serialize(packet_t packet);



#endif // PACKET_H
