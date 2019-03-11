#include <Arduino.h>
#include "packet.h"

radioPacket::radioPacket(packet_t packet)
{
  packet = packet;
  header = packet.header;
  body = packet.body;
}

radioPacket::~radioPacket()
{
  // destructor
}

void radioPacket::deserialize()
{
  switch (header.type)
  {
  case EMPTY:
  case JOIN:
  case BYE:
  case ROUTE:
  case ACK:
    Serial.println("ACK\n");
    break;
  case MSG:
    Serial.println("MSG\n");
    break;
  case HELLO:
  case GOSSIP:
  case NOT_DELIVERED:
    break;
  default:
    break;
  }
}

