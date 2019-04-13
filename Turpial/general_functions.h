/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef GENERAL_FUNCTIONS_H_
#define GENERAL_FUNCTIONS_H_

// declaracion de librerias
#include <Arduino.h>
#include "packet.h"
using namespace std;

//declaracion de variables
extern char* uid;
extern char* msg;
extern double timemsg;

// declaracion de funciones
void create_unique_id(char* &unique_id_created);
char* string2char_node_name(String command);
void string2char_node_name_v2(String command, char* &respuesta);
char* string2char(String command);
boolean isNumeric(String str);
String packet_into_json(packet_t packet_to_convert, String BLE_type);
String get_id_mac();
String freeRam();
void copy_array_locha(char* src, char* dst, int len);
void json_receive(String message, char* &uid_intern,char* &msg_intern, char* &timemsg_intern_str, char* &hash_msg_intern );
char* node_name_char_to_uppercase(char array_temp[16]);
uint8_t convert_str_to_uint8(String texto);
bool is_valid_hash160(char* mensaje, char* hash_recibido);
long long char2LL(char *str);

#endif // GENERAL_FUNCTIONS_H_
