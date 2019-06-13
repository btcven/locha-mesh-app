/**
 * @file general_utils.cpp
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
#include <sstream>
#include "general_utils.h"


/**
 * @brief converts string into char array
 * 
 */
char *string2char(std::string command)
{
  if (command.length() != 0)
  {
    char *p = const_cast<char *>(command.c_str());
    return p;
  } else {
    return NULL;
  }
}


/**
 * @brief Convert std::string to char array
 * 
 */
 char *std_string_to_char(std::string textreceived)
{
  char *cstr = new char[textreceived.length() + 1];
  strcpy(cstr, textreceived.c_str());
  return cstr;
}

/**
 * @brief Copy char arrays
 * 
 */
 void copy_array(char *src, char *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}

/**
 * @brief Copy char arrays
 * 
 */
 void copy_array_tobyte(char *src, byte *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}


/**
 * @brief  functions for converts to std::string
 * 
 */
 
template <typename T>
std::string ToString(T val)
{
  std::stringstream stream;
  stream << val;
  return stream.str();
}

/**
 * @brief Gets ESP32 unique device ID based on factory mac address
 * 
 */
std::string getMacAddress()
{
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return (baseMacChr);
}


/**
 * @brief removes character ":" from factory mac address
 * 
 */
 std::string getIdMac()
{
  std::string result = getMacAddress();
  result.erase(std::remove(result.begin(), result.end(), ':'), result.end());
  return result;
}
