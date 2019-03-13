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
  char *from;
  char *to;
  unsigned long timestamp;
} packet_header_t;

typedef struct
{
  char *payload;
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
  virtual ~radioPacket();
  void serialize();
  void deserialize();

private:
  void msg_handler();
  packet_header_t header;
  packet_body_t body;
  packet_t packet;
};

#endif // PACKET_H
