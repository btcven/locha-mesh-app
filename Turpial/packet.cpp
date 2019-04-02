/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include "packet.h"
#include <string.h> 
#include <Time.h>
#include <TimeLib.h>
#include "general_functions.h"
#include "debugging.h"

extern char* id_node;
extern packet_t Buffer_packet;





radioPacket::radioPacket(packet_t packet)
{
  packet = packet;
  header = packet.header;
  body = packet.body;
}

radioPacket::~radioPacket()
{
  // destructor
  delete this;
}



// Funcion en cargada de convertir un packet en una cadana char para ser enviada por Radio
//  se usa +"|" como separador de campo
String packet_serialize(packet_t packet){
  String rpta_str="";
  rpta_str=rpta_str+(String)packet.header.type+"|";
  rpta_str=rpta_str+(String)packet.header.from+"|";
  rpta_str=rpta_str+(String)packet.header.to+"|";
  rpta_str=rpta_str+(String)packet.header.timestamp+"|";
  rpta_str=rpta_str+(String)packet.body.payload+"|";
  return rpta_str;
}

unsigned long convert_str_to_long(char* time_in_char){
    unsigned long uil;  
    uil = strtoul(time_in_char,NULL,10);
    return uil;
}



packet_type_e convertir_str_packet_type_e(String type_recibido){
  packet_type_e rpta=EMPTY;
  type_recibido.trim();
  if (type_recibido==F("EMPTY")) return EMPTY;
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
  if (type_recibido==EMPTY) rpta=F("EMPTY");
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


// Funcion encargada de convertir una cadena char en un packet 
packet_t packet_deserialize(char* received_text){
  packet_t packet_tmp;
  char* str_in_process="";
 // String packet_type_str="";
 // String packet_from="";
 // String packet_to="";
 // String packet_timestamp="";
 // String packet_type_payload="";
  uint8_t i=1;
//  String temp_var;
  // algo asi 
 // temp_var=received_text.substring(1,1);   //packet.header.type
 // packet_tmp.header.type=(int)temp_var;
  while ((str_in_process = strtok_r(received_text, "|", &received_text)) != NULL) {
    switch (i) {
          case 1:
            packet_tmp.header.type=convertir_str_packet_type_e((String)str_in_process);
            break;
          case 2:
             copy_array_locha(str_in_process, packet_tmp.header.from, 16);
            break;
             case 3:
             copy_array_locha(str_in_process, packet_tmp.header.to, 16);
            break;
             case 4:
             packet_tmp.header.timestamp=convert_str_to_long(str_in_process);
            break;
             case 5:
             copy_array_locha(str_in_process, packet_tmp.body.payload, ((String)str_in_process).length());
            break;
         
        }
    i++;
  }
 // packet_from.toCharArray(packet.header.from, 16);
 // packet_to.toCharArray(packet.header.to, 16);
 //packet_type_payload.toCharArray(packet.body.payload;, 240);
// falta el timestamp que es un longint y aqui se deberia convertir
 // rpta_str=rpta_str+(String)packet.header.timestamp;

  return packet_tmp;
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
