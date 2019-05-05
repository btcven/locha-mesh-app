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


/**
 * @brief: from packet_t to uint8_t array
 * 
 * @param target: pointer to uint8_t array
 * @param source: source packet
 * @param s_size: size of source packet 
 */
void packet_to_char(uint8_t *target, packet_t source, size_t s_size)
{
  // target ya es un puntero(*target) no necesitamos referenciarlo como tal
  // packet lo pasamos como parametro, necesitamos un puntero para memcpy (&)
  memcpy(target, &source, s_size);
}

/**
 * @brief from uint8_t array to packet_t
 * 
 * @param target; pointer to an empty packet
 * @param source; pointer to uint8_t array 
 * @param s_size: size of uint8_t array
 */
void char_to_packet(packet_t *target, uint8_t *source, size_t s_size)
{
  // packet_t *target: ya es puntero, no necesitamos referenciarlo como tal
  // uint8_t *source: ya es puntero, no necesitamos referenciarlo como tal
  memcpy(target, source, s_size);
}


// funcion para contruir un packet HELLO que identifique inicialmente al nodo



// Funcion para construir un packet dao el origen, destino, tipo y contenido (payload)
packet_t create_packet(char *id_node, packet_type_e tipo_packet, subtype_u subtipo_packet, char *from, char *to, char *next_neighbor, char *checksum_data, char *payload, uint16_t packet_number, uint16_t packet_total)
{

  
  
  packet_header_t header;
  body_data_u body;
 
  
  header.packet_type = tipo_packet;
   header.packet_sub=subtipo_packet;
   
  //switch (tipo_packet)
  //  {
  //  case DATA:
    
    //    header.packet_sub=subtipo_packet;
    //    break;
    //case ROUTING:
   //      header.packet_sub.routing_type=subtipo_packet;
   //     break;
   // case SECURITY:
   //      header.packet_sub.security_type=subtipo_packet;
   //     break;
   // }
  
  copy_array(from, header.from, SIZE_IDNODE);
  copy_array(to, header.to, SIZE_IDNODE);
  copy_array(next_neighbor, header.next_neighbor, SIZE_IDNODE);
  header.timestamp = millis();
  copy_array(checksum_data, header.checksum_data, SIZE_HASH_MSG);

  // el payload depende del tipo de packet_t
  if (tipo_packet==DATA){
    copy_array(payload, body.body_data_splitted.payload, SIZE_PAYLOAD);
    
    body.body_data.payload_length = sizeof(body.body_data_splitted.payload);
    body.body_data_splitted.packet_number=packet_number;
     body.body_data_splitted.packet_total=packet_total;
     body.body_data_splitted.not_delivered_type=EMPTY_NOT_DELIVERED;
  } else {
    
     copy_array(payload, body.body_data.payload, SIZE_PAYLOAD);
     body.body_data.payload_length = sizeof(body.body_data.payload);
  }
  
  packet_t new_packet;
  new_packet.header = header;
  new_packet.body = body;
  return new_packet;
}

// create_packet overload for individual packets
packet_t create_packet(char *id_node, packet_type_e tipo_packet, subtype_u subtipo_packet, char *from, char *to, char *next_neighbor, char *checksum_data, char *payload){
  return create_packet(id_node, tipo_packet, subtipo_packet, from, to, next_neighbor, checksum_data, payload, 1,1);
  
}

char* convertir_packet_type_e_str(packet_type_e type_recibido){
  char* rpta=NULL;
  char *pChar = (char*)"EMPTY";
  if (type_recibido==EMPTY) rpta=pChar;
  pChar = (char*)"DATA";
  if (type_recibido==DATA) rpta=pChar;
  pChar = (char*)"ROUTING";
  if (type_recibido==ROUTING) rpta=pChar;
  pChar = (char*)"SECURITY";
  if (type_recibido==SECURITY) rpta=pChar;
 
  return rpta;
}

// convierte una cadena de caracteres recibida en formato texto al typedef enum packet_type_e 
packet_type_e convertir_str_packet_type_e(char* type_recibido){
  packet_type_e rpta=EMPTY;
  char *pChar = (char*)"EMPTY";
  if (compare_char(type_recibido,pChar)) return EMPTY;
  pChar = (char*)"DATA";
  if (compare_char(type_recibido,pChar)) return DATA;
  pChar = (char*)"ROUTING";
  if (compare_char(type_recibido,pChar)) return ROUTING;
   pChar = (char*)"SECURITY";
  if (compare_char(type_recibido,pChar)) return SECURITY;
 
  return rpta;
}

void show_packet(packet_t packet_rx,const char *TAG)
  {
    ESP_LOGE(TAG, "Showing packet_t");
    ESP_LOGE(TAG,"----------------------------------------\n");
    ESP_LOGE(TAG,"type: %02X\n", packet_rx.header.packet_type);
    ESP_LOGE(TAG,"subtype: %02X\n", packet_rx.header.packet_sub);
    ESP_LOGE(TAG,"from: %s\n", packet_rx.header.from);
    ESP_LOGE(TAG,"to: %s\n", packet_rx.header.to);
    ESP_LOGE(TAG,"----------------------------------------\n");
    if (packet_rx.header.packet_type==DATA){ 
        ESP_LOGE(TAG,"body length: %d\n", packet_rx.body.body_data_splitted.payload_length);
        ESP_LOGE(TAG,"data: %s\n", packet_rx.body.body_data_splitted.payload);
    } else {
        ESP_LOGE(TAG,"body length: %d\n", packet_rx.body.body_data.payload_length);
        ESP_LOGE(TAG,"data: %s\n", packet_rx.body.body_data.payload);
    }
    
    ESP_LOGE(TAG,"----------------------------------------\n");
   
}

packet_t construct_packet_HELLO(char *id_node,char *from)
{
  packet_t packet_HELLO;
  
  char *pChar = (char *)"";
  subtype_u subtipo_packet;
  subtipo_packet.routing_type=HELLO;
packet_HELLO=create_packet(id_node, ROUTING, subtipo_packet, from, pChar, pChar, pChar, pChar);
  
  //packet_HELLO.header.packet_type = ROUTING;
  //packet_HELLO.header.packet_sub.routing_type = HELLO;
  //copy_array(from, packet_HELLO.header.from, SIZE_IDNODE);
  //copy_array(pChar, packet_HELLO.header.to, SIZE_IDNODE);
  //copy_array(pChar, packet_HELLO.header.next_neighbor, SIZE_IDNODE);
  //copy_array(pChar, packet_HELLO.header.checksum_data, SIZE_HASH_MSG);
  //copy_array(pChar, packet_HELLO.body.body_data.payload, SIZE_PAYLOAD);
  //packet_HELLO.body.body_data.payload_length = sizeof(packet_HELLO.body.body_data.payload);
 
  //packet_HELLO.header.timestamp = millis();

  return packet_HELLO;
}

// funcion que construye un mensaje son para devolver al bluetooth en formato ERR
std::string Json_return_error(std::string mensaje,std::string uid_mensaje)
{
  char *msg_to_return;
  strcpy(msg_to_return, "{\"uid\":\"");
  strcat(msg_to_return, string2char(uid_mensaje));
  strcat(msg_to_return, string2char("\",\"ERR\":\""));
  strcat(msg_to_return, string2char(mensaje));
  strcat(msg_to_return, "\",\"time\":");
  strcat(msg_to_return, string2char(ToString(millis())));
  strcat(msg_to_return, "\",\"status\":NOT_DELIVERED}");

  return ToString(msg_to_return);
}
// funcion que construye un mensaje son para devolver al bluetooth en formato MSG
std::string Json_return_msg(std::string mensaje,std::string uid_mensaje)
{
  char *msg_to_return;
  strcpy(msg_to_return, "{\"uid\":\"");
  strcat(msg_to_return, string2char(uid_mensaje));
  strcpy(msg_to_return, "\",\"msg\":\"");
  strcat(msg_to_return, string2char(mensaje));
  strcat(msg_to_return, "\",\"time\":");
  strcat(msg_to_return, string2char(ToString(millis())));
  strcat(msg_to_return, "\",\"status\":OK}");

  return ToString(msg_to_return);
}
