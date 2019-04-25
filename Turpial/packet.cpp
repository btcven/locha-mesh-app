/**
 * @file packet.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#include <Arduino.h>
#include "packet.h"
#include "general_utils.h"

// convierte una cadena de caracteres recibida en formato texto al typedef enum packet_type_e 
packet_type_e convertir_str_packet_type_e(char* type_recibido){
  packet_type_e rpta=EMPTY;
  char *pChar = (char*)"EMPTY";
  if (compare_char(type_recibido,pChar)) return EMPTY;
  pChar = (char*)"ACK";
  if (compare_char(type_recibido,pChar)) return ACK;
  pChar = (char*)"JOIN";
  if (compare_char(type_recibido,pChar)) return JOIN;
   pChar = (char*)"BYE";
  if (compare_char(type_recibido,pChar)) return BYE;
   pChar = (char*)"MSG";
  if (compare_char(type_recibido,pChar)) return MSG;
   pChar = (char*)"ROUTE";
  if (compare_char(type_recibido,pChar)) return ROUTE;
   pChar = (char*)"HELLO";
  if (compare_char(type_recibido,pChar)) return HELLO;
   pChar = (char*)"GOSSIP";
  if (compare_char(type_recibido,pChar)) return GOSSIP;
   pChar = (char*)"NOT_DELIVERED";
  if (compare_char(type_recibido,pChar)) return NOT_DELIVERED;
         
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
char* convertir_packet_type_e_str(packet_type_e type_recibido){
  char* rpta=NULL;
  char *pChar = (char*)"EMPTY";
  if (type_recibido==EMPTY) rpta=pChar;
  pChar = (char*)"ACK";
  if (type_recibido==ACK) rpta=pChar;
  pChar = (char*)"JOIN";
  if (type_recibido==JOIN) rpta=pChar;
  pChar = (char*)"BYE";
  if (type_recibido==BYE) rpta=pChar;
  pChar = (char*)"MSG";
  if (type_recibido==MSG) rpta=pChar;
  pChar = (char*)"ROUTE";
  if (type_recibido==ROUTE) rpta=pChar;
  pChar = (char*)"HELLO";
  if (type_recibido==HELLO) rpta=pChar;
  pChar = (char*)"GOSSIP";
  if (type_recibido==GOSSIP) rpta=pChar;
  pChar = (char*)"NOT_DELIVERED";
  if (type_recibido==NOT_DELIVERED) rpta=pChar;
  
  return rpta;
}


// funcion para contruir un packet HELLO que identifique inicialmente al nodo
packet_t construct_packet_HELLO(char* from){
    packet_t packet_HELLO;
    char *pChar = (char*)"";
    
    copy_array(from, packet_HELLO.header.from, SIZE_IDNODE);
    copy_array(pChar, packet_HELLO.header.to, SIZE_IDNODE);
    copy_array(pChar, packet_HELLO.header.next_neighbor, SIZE_IDNODE);
    copy_array(pChar, packet_HELLO.header.hash, SIZE_HASH_MSG);
    copy_array(pChar, packet_HELLO.body.payload, SIZE_PAYLOAD);
    packet_HELLO.header.type=HELLO;
    packet_HELLO.header.timestamp=millis();
    
    return packet_HELLO;
}

// Funcion en cargada de convertir un packet en una cadana char para ser enviada por Radio
//  se usa | como separador de campo
std::string packet_serialize(packet_t packet){
   char *rpta_str = (char*)"";
   char *pChar = (char*)"|";
    //std::ostringstream s;
    //s << (int)packet.header.type;
    strcpy(rpta_str, string2char(ToString( packet.header.type)));
    strcat(rpta_str, pChar);
    strcat(rpta_str, packet.header.from);
    strcat(rpta_str, pChar);
    strcat(rpta_str, packet.header.to);
    strcat(rpta_str, pChar);
    strcat(rpta_str, packet.header.next_neighbor);
    strcat(rpta_str, pChar);
    //s << (long long)packet.header.timestamp;
    strcat(rpta_str, string2char(ToString(packet.header.timestamp)));
    strcat(rpta_str, pChar);
    strcat(rpta_str, packet.header.hash);    
    strcat(rpta_str, pChar);
    strcat(rpta_str, packet.body.payload);
    strcat(rpta_str, pChar);
     
return  ToString(rpta_str);
}


// Funcion encargada de convertir una cadena char en un packet 
packet_t packet_deserialize(char* received_text){
  packet_t packet_tmp;
  char* str_in_process=NULL;
 
  uint8_t i=1;

  while ((str_in_process = strtok_r(received_text, "|", &received_text)) != NULL) {
    switch (i) {
          case 1:
            packet_tmp.header.type=convertir_str_packet_type_e(str_in_process);
            break;
          case 2:
          
             copy_array(str_in_process, packet_tmp.header.from, SIZE_IDNODE);
            break;
             case 3:
             
             copy_array(str_in_process, packet_tmp.header.to, SIZE_IDNODE);
            break;
              case 4:
             
             copy_array(str_in_process, packet_tmp.header.next_neighbor, SIZE_IDNODE);
            break;
             
             case 5:
           
             packet_tmp.header.timestamp=convert_str_to_long(str_in_process);
            break;
             case 6:
             
             copy_array(str_in_process, packet_tmp.header.hash, SIZE_HASH_MSG);
            break;
             case 7:
            
             copy_array(str_in_process, packet_tmp.body.payload, (sizeof(str_in_process)));
            break;
         
        }
    i++;
  }

  return packet_tmp;
}


// Funcion para construir un packet dao el origen, destino, tipo y contenido (payload)
packet_t create_packet(char* id_node, packet_type_e tipo_packet, char* from, char* to,  char* next_neighbor, char* hash,char* payload){
   
      packet_header_t header;
      packet_body_t body;

      header.type=tipo_packet;
      copy_array(from, header.from, SIZE_IDNODE);
      copy_array(to, header.to, SIZE_IDNODE);
      copy_array(next_neighbor, header.next_neighbor, SIZE_IDNODE);
      header.timestamp=millis();
      copy_array(hash, header.hash,  SIZE_HASH_MSG);
      copy_array(payload, body.payload, SIZE_PAYLOAD);
      packet_t new_packet;
      new_packet.header=header;
      new_packet.body=body;
    return new_packet;
}

// funcion que construye un mensaje son para devolver al bluetooth en formato ERR
std::string Json_return_error(std::string mensaje){
    char* msg_to_return;
    strcpy(msg_to_return, "{\"uid\":\"broadcast\",\"ERR\":\"");
    strcat(msg_to_return, string2char(mensaje));
    strcat(msg_to_return, "\",\"time\":");
    strcat(msg_to_return, string2char(ToString(millis())));
    strcat(msg_to_return, "\",\"status\":NOT_DELIVERED}");
   
    return ToString(msg_to_return);
}
// funcion que construye un mensaje son para devolver al bluetooth en formato MSG
std::string Json_return_msg(std::string mensaje){
    char* msg_to_return;
    strcpy(msg_to_return, "{\"uid\":\"broadcast\",\"msg\":\"");
    strcat(msg_to_return, string2char(mensaje));
    strcat(msg_to_return, "\",\"time\":");
    strcat(msg_to_return, string2char(ToString(millis())));
    strcat(msg_to_return, "\",\"status\":OK}");
    
    return ToString(msg_to_return);
}
