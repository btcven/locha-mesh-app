/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
// declaracion de librerias
#include <Arduino.h>
#include <string.h> 
#include <Time.h>
#include <TimeLib.h>
#include "packet.h"
#include "general_functions.h"
#include "debugging.h"


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



// convierte un string en un numero tipo long
unsigned long convert_str_to_long(char* time_in_char){
    unsigned long uil;  
    uil = strtoul(time_in_char,NULL,10);
    return uil;
}


// convierte una cadena de caracteres recibida en formato texto al typedef enum packet_type_e 
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

// convierte un numero al typedef enum packet_type_e 
packet_type_e convertir_int_packet_type_e(uint8_t type_recibido){
  packet_type_e rpta=EMPTY;
  
  if (type_recibido==0) return EMPTY;
  if (type_recibido==1) return JOIN;
  if (type_recibido==2) return BYE;
  if (type_recibido==3) return ROUTE;
  if (type_recibido==4) return ACK;
  if (type_recibido==5) return MSG;
  if (type_recibido==6) return HELLO;
  if (type_recibido==7) return GOSSIP;
  if (type_recibido==8) return NOT_DELIVERED;
  return rpta;
}

// convierte un  typedef enum packet_type_e  en una cadena de caracteres ( inverso de convertir_str_packet_type_e() )
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


// funcion para contruir un packet HELLO que identifique inicialmente al nodo
packet_t construct_packet_HELLO(char* from){
   packet_t packet_HELLO;
   
    copy_array_locha(from, packet_HELLO.header.from, 16);
    copy_array_locha("", packet_HELLO.header.to, 16);
    copy_array_locha("", packet_HELLO.body.payload, 240);
    packet_HELLO.header.type=HELLO;
    packet_HELLO.header.timestamp=millis();
    
    return packet_HELLO;
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

// Funcion encargada de convertir un string en un packet ( inverso de packet_serialize() )
packet_t packet_deserialize_str(String received_text){
   uint8_t ind1;
   uint8_t ind2;
   uint8_t ind3;
   uint8_t ind4;
   uint8_t ind5;
   String str_in_process;
   packet_t packet_tmp;
  
   if (received_text.length()>0){
       ind1 = received_text.indexOf('|');  //finds location of first ,
       str_in_process = received_text.substring(0, ind1);  
            if (isNumeric(str_in_process)){
               
                packet_tmp.header.type=convertir_int_packet_type_e(convert_str_to_uint8(str_in_process));
                
            }
      ind2 = received_text.indexOf('|', ind1+1 );   //finds location of second ,
      str_in_process = received_text.substring(ind1+1, ind2);   //captures second data String
      str_in_process.toCharArray(packet_tmp.header.from,16);
      ind3 = received_text.indexOf('|', ind2+1 );
      str_in_process = received_text.substring(ind2+1, ind3);
      str_in_process.toCharArray(packet_tmp.header.to,16);
      ind4 = received_text.indexOf('|', ind3+1 );
      str_in_process = received_text.substring(ind3+1, ind4);
            if (isNumeric(str_in_process)){
                    packet_tmp.header.timestamp=char2LL(string2char(str_in_process));
            } else { 
              packet_tmp.header.timestamp=0;
            }
       
      ind5 = received_text.indexOf('|', ind4+1);
      if (ind5>0){   // si venia al final con |
          str_in_process = received_text.substring( ind4+1,ind5 );
         
      } else {
        // no trae | al final
        str_in_process = received_text.substring( ind4+1,received_text.length()-ind4-1 );
        
      }
      
      str_in_process.toCharArray(packet_tmp.body.payload,str_in_process.length()+1);
    
   } 
   
   return packet_tmp;
}

// Funcion encargada de convertir una cadena char en un packet 
packet_t packet_deserialize(char* received_text){
  packet_t packet_tmp;
  char* str_in_process=NULL;
 
  uint8_t i=1;

 DEBUG_PRINT(F("voy a deserialize con:"));
 DEBUG_PRINTLN((String)received_text);
  while ((str_in_process = strtok_r(received_text, "|", &received_text)) != NULL) {
    switch (i) {
          case 1:
            DEBUG_PRINT(F("el tipo es:"));
            DEBUG_PRINT((String)str_in_process);
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

  return packet_tmp;
}


// Funcion para construir un packet dao el origen, destino, tipo y contenido (payload)
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

// funcion que construye un mensaje son para devolver al bluetooth en formato ERR
String Json_return_error(String mensaje){
    String msg_to_return;
    msg_to_return="{\"uid\":\"broadcast\",\"ERR\":\""+mensaje+"\",\"time\":"+(String)millis()+"\",\"status\":NOT_DELIVERED}";  
    return msg_to_return;
}
// funcion que construye un mensaje son para devolver al bluetooth en formato MSG
String Json_return_msg(String mensaje){
    String msg_to_return;
    msg_to_return="{\"uid\":\"broadcast\",\"msg\":\""+mensaje+"\",\"time\":"+(String)millis()+"\",\"status\":OK}";  
    return msg_to_return;
}
