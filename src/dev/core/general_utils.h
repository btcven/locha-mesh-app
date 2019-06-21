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

/**
 * @brief converts string into char array
 * 
 */
char *string2char(std::string command);

/**
 * @brief Convert std::string to char array
 * 
 */
char *std_string_to_char(std::string textreceived);


void copy_array_tobyte(char *src, byte *dst, int len);


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
 * @brief removes character ":" from factory mac address
 * 
 */
 std::string getIdMac();
 

#endif // GENERAL_UTILS_H