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

/**
 * @brief compare if two chars are equals
 * 
 * @param src 
 * @param dst 
 * @return true: both equal
 * @return false: both not equal
 */
bool compare_char(char *src, char *dst);

/**
 * @brief 
 * 
 * @param mainStr 
 * @param toErase 
 */
void eraseAllSubStr(std::string &mainStr, const std::string &toErase);

/**
 * @brief transforms from std::string to char array
 * 
 * @param cadena 
 * @return char* 
 */
char *std_string_to_char(std::string cadena);

/**
 * @brief 
 * 
 * @param str 
 * @return long long 
 */
long long char2LL(char *str);

/**
 * @brief transforms from std::string to uint8_t array
 * 
 * @param texto 
 * @return uint8_t 
 */
uint8_t convert_str_to_uint8(std::string texto);

/**
 * @brief 
 * 
 * @param command 
 * @return char* 
 */
char *string2char(std::string command);

/**
 * @brief copy between two array pointers
 * 
 * @param src source array
 * @param dst target array
 * @param len length of the source array (*)
 */
void copy_array(char *src, char *dst, int len);

/**
 * @brief Get the Mac Address object
 * 
 * @return std::string 
 */
std::string getMacAddress();

/**
 * @brief 
 * 
 * @tparam T 
 * @param val 
 * @return std::string 
 */
template <typename T>

/**
 * @brief 
 * 
 * @param val 
 * @return std::string 
 */
std::string ToString(T val);

/**
 * @brief Transforms a ULL to std::string
 * 
 * @param i: Number 
 * @return std::string 
 */
std::string number_to_str(unsigned long long i);

/**
 * @brief Determines if a std::string contains a numeric representation.
 * 
 * @param str 
 * @return true: It is a number
 * @return false: It is not a number 
 */
bool isNumeric(std::string str);

/**
 * @brief 
 * 
 * @param message 
 * @param uid_intern 
 * @param msg_intern 
 * @param timemsg_intern_str 
 * @param hash_msg_intern 
 */
void json_receive(std::string message, char *&uid_intern, char *&msg_intern, char *&timemsg_intern_str, char *&hash_msg_intern);

/**
 * @brief 
 * 
 * @param packet_to_convert 
 * @param BLE_type 
 * @return std::string 
 */
std::string packet_into_json(packet_t packet_to_convert, std::string BLE_type);

/**
 * @brief Get the id mac object
 * 
 * @return std::string 
 */
std::string get_id_mac();

/**
 * @brief 
 * 
 * @param data 
 * @param len 
 */
void printHex(byte *data, int len);

/**
 * @brief Create a unique id object
 * 
 * @param unique_id_created 
 */
void create_unique_id(char *&unique_id_created);

/**
 * @brief 
 * 
 * @return std::string 
 */
std::string freeRam();

/**
 * @brief 
 * 
 * @param time_in_char 
 * @return unsigned long 
 */
unsigned long convert_str_to_long(char *time_in_char);

/**
 * @brief 
 * 
 * @param path 
 * @return true 
 * @return false 
 */
bool Fileexists(const char *path);

/*
* Erase First Occurrence of given  substring from main string.
*/
void eraseSubStr(std::string &mainStr, const std::string &toErase);

/*
* For xTask stack sizing
*/
float GetTaskHighWaterMarkPercent(TaskHandle_t task_handle, uint32_t stack_allotment);
#endif // GENERAL_UTILS_H
