/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h> 
#include "hal/hardware.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "routing_incoming.h"
#include "routing_outcoming.h"

#include <Time.h>
#include <TimeLib.h>

extern char* id_node;
extern packet_t Buffer_packet;


// se incluyen los eventos para externos para cada tipo de packet_t
extern uint8_t routing_incoming_PACKET_HELLO(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_BYE(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_JOIN(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_ROUTE(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_ACK(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_GOSSIP(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_MSG(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_TXN(char id_node[16], packet_t packet_received);
extern uint8_t routing_incoming_PACKET_NOT_DELIVERED(char id_node[16], packet_t packet_received);






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
  uint8_t rpta;
  switch (header.type)
  {
  case EMPTY:
  case JOIN:
      routing_incoming_PACKET_JOIN(id_node, Buffer_packet);
      break;
  case BYE:
      routing_incoming_PACKET_BYE(id_node, Buffer_packet);
      break;
  case ROUTE:
      routing_incoming_PACKET_ROUTE(id_node, Buffer_packet);
      break;
  case ACK:
      routing_incoming_PACKET_ACK(id_node, Buffer_packet);
      break;
  case MSG:
     routing_incoming_PACKET_MSG(id_node, Buffer_packet);
     break;
  case HELLO:
   routing_incoming_PACKET_HELLO(id_node, Buffer_packet);
      break;
  case GOSSIP:
   routing_incoming_PACKET_GOSSIP(id_node, Buffer_packet);
      break;
  case NOT_DELIVERED:
   routing_incoming_PACKET_NOT_DELIVERED(id_node, Buffer_packet);
      break;
  default:
    break;
  }
}


// Funcion en cargada de convertir un packet en una cadana char para ser enviada por Radio
String packet_serialize(packet_t packet){
  String rpta_str="";
  rpta_str=rpta_str+(String)packet.header.type;
  rpta_str=rpta_str+(String)packet.header.from;
  rpta_str=rpta_str+(String)packet.header.to;
  rpta_str=rpta_str+(String)packet.header.timestamp;
  rpta_str=rpta_str+(String)packet.body.payload;
  return rpta_str;
}

// Funcion en cargada de convertir una cadena char en un packet 
packet_t packet_deserialize(String received_text){
  packet_t packet_tmp;
//  String temp_var;
  // algo asi
 // temp_var=received_text.substring(1,1);   //packet.header.type
 // packet_tmp.header.type=(int)temp_var;
  
 // packet.header.from;
 // rpta_str=rpta_str+(String)packet.header.to;
 // rpta_str=rpta_str+(String)packet.header.timestamp;
 // rpta_str=rpta_str+(String)packet.body.payload;
  return packet_tmp;
}


packet_type_e convertir_str_packet_type_e(String type_recibido){
  packet_type_e rpta=EMPTY;
  type_recibido.trim();
  if (type_recibido==F("ACK")) return ACK;
  if (type_recibido==F("JOIN")) return JOIN;
  if (type_recibido==F("BYE")) return BYE;
  if (type_recibido==F("MSG")) return MSG;
  if (type_recibido==F("ROUTE")) return ROUTE;
  if (type_recibido==F("HELLO")) return HELLO;
  if (type_recibido==F("GOSSIP")) return GOSSIP;
  if (type_recibido==F("NOT_DELIVERED")) return NOT_DELIVERED;
  return rpta;
}

String convertir_packet_type_e_str(packet_type_e type_recibido){
  String rpta="";
  if (type_recibido==ACK) rpta=F("ACK");
  if (type_recibido==JOIN) rpta=F("JOIN");
  if (type_recibido==BYE) rpta=F("BYE");
  if (type_recibido==MSG) rpta=F("MSG");
  if (type_recibido==ROUTE) rpta=F("ROUTE");
  if (type_recibido==HELLO) rpta=F("HELLO");
  if (type_recibido==GOSSIP) rpta=F("GOSSIP");
  if (type_recibido==NOT_DELIVERED) rpta=F("NOT_DELIVERED");
  return rpta;
}

packet_t create_packet(char* id_node, packet_type_e tipo_packet, char* from, char* to, char* payload){
   
      packet_header_t header;
      packet_body_t body;

      header.type=tipo_packet;
      copy_array_locha(from, header.from, 16);
      copy_array_locha(to, header.to, 16);
      header.timestamp=millis();
      copy_array_locha(payload, body.payload, 240);
      packet_t new_packet;
      new_packet.header=header;
      new_packet.body=body;
    return new_packet;
}
