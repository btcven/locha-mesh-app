/**
 * @file general_utils.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#ifndef GENERAL_UTILS_H
#define GENERAL_UTILS_H

#include <Arduino.h>
#include <string>
#include <sstream>
#include <cJSON.h>
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "packet.h"


bool compare_char(char* src ,char* dst);
char* std_string_to_char(std::string cadena);
long long char2LL(char *str);
uint8_t convert_str_to_uint8(std::string texto);
char *string2char(std::string command);
void copy_array(char *src, char *dst, int len);
std::string getMacAddress();
template <typename T>
std::string ToString(T val);
std::string number_to_str(unsigned long long i);
boolean isNumeric(std::string str);
void json_receive(std::string message, char* &uid_intern,char* &msg_intern, char* &timemsg_intern_str, char* &hash_msg_intern );
std::string packet_into_json(packet_t packet_to_convert, std::string BLE_type);
std::string get_id_mac();
void printHex(byte * data, int len);
void create_unique_id(char *&unique_id_created);
std::string freeRam();
unsigned long convert_str_to_long(char* time_in_char);
bool Fileexists(const char* path);

#endif // GENERAL_UTILS_H
