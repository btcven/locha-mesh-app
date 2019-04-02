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

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// Funcion encargada de convertir una cadena char en un packet 
packet_t packet_deserialize(char* received_text){
  packet_t packet_tmp;
  char* str_in_process="";
 
  uint8_t i=1;

 Serial.print("voy a deserialize con:");
 Serial.println(received_text);
  while ((str_in_process = strtok_r(received_text, "|", &received_text)) != NULL) {
    switch (i) {
          case 1:
            Serial.print("el tipo es:");
            Serial.print((String)str_in_process);
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
 // Serial.print("sali del while con:");
 // Serial.println(packet_tmp.body.payload);
 

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
