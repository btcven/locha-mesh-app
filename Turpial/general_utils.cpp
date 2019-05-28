/**
   @file general_utils.cpp
   @author locha.io project developers (dev@locha.io)
   @brief
   @version 0.1
   @date 2019-04-24

   @copyright Copyright (c) 2019 locha.io project developers
   @license MIT license, see LICENSE file for details

*/

#include <Arduino.h>

#include "general_utils.h"

// compare char
bool compare_char(char *src, char *dst)
{
  if (strcmp(src, dst) == 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void eraseAllSubStr(std::string &mainStr, const std::string &toErase)
{
  size_t pos = std::string::npos;

  // Search for the substring in string in a loop untill nothing is found
  while ((pos = mainStr.find(toErase)) != std::string::npos)
  {
    // If found then erase it from string
    mainStr.erase(pos, toErase.length());
  }
}

// function for convert std::string into char*
char *std_string_to_char(std::string cadena)
{
  char *cstr = new char[cadena.length() + 1];
  strcpy(cstr, cadena.c_str());
  return cstr;
}

// function for convert  Char* to long long
// used to convert timestamps coming from char* into long long variable
long long char2LL(char *str)
{
  long long result = 0; // Initialize result
  // Iterate through all characters of input string and update result
  for (int i = 0; str[i] != '\0'; ++i)
    result = result * 10 + str[i] - '0';
  return result;
}

/**
   @brief  Function to convert std::string to uint8_t array

   @param texto
   @return uint8_t
*/
uint8_t convert_str_to_uint8(std::string texto)
{
  unsigned long long y = 0;
  for (int i = 0; i < texto.length(); i++)
  {
    char c = texto[i];
    if (c < '0' || c > '9')
      break;
    y *= 10;
    y += (c - '0');
  }

  return y;
}

// Function to convert std::string to char*
char *string2char(std::string command)
{
  if (command.length() != 0)
  {
    char *p = const_cast<char *>(command.c_str());
    return p;
  }
}

// Copy a char* into another char*
void copy_array(char *src, char *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}

// get ESP32 macaddress once connected to Wifi
std::string getMacAddress()
{
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return (baseMacChr);
}

// same as getMacAddress() without character :
std::string getMacAddress_onlychars()
{
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X%02X%02X%02X%02X%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return (baseMacChr);
}



// function to convert any type to std::string
template <typename T>
std::string ToString(T val)
{
  std::stringstream stream;
  stream << val;
  return stream.str();
}

// function to convert a number into std::string
std::string number_to_str(unsigned long long i)
{
  std::string str = ToString(i);
  return str;
}

// function to convert a char* into long
unsigned long convert_str_to_long(char *time_in_char)
{
  unsigned long uil;
  uil = strtoul(time_in_char, NULL, 10);
  return uil;
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
std::string getValue(std::string data, char separator, int index)
{
  uint8_t found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data[i] == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substr(strIndex[0], strIndex[1]) : "";
}


/*
  Erase First Occurrence of given  substring from main string.
*/
void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
  // Search for the substring in string
  size_t pos = mainStr.find(toErase);
  if (pos != std::string::npos)
  {
    // If found then erase it from string
    mainStr.erase(pos, toErase.length());
  }
}


/**
   @brief verify if a nstd::string is a number

   @param str
   @return true
   @return false
*/
bool isNumeric(std::string str)
{
  unsigned int stringLength = str.length();
  if (stringLength == 0)
  {
    return false;
  }
  bool seenDecimal = false;

  for (unsigned int i = 0; i < stringLength; ++i)
  {
    if (isDigit(str[i]))
    {
      continue;
    }
    if (str[i] == '.')
    {
      if (seenDecimal)
      {
        return false;
      }
      seenDecimal = true;
      continue;
    }
    return false;
  }
  return true;
}

// use cJson integrated into espressif esp32 sdk
/* The cJSON structure: */
//typedef struct cJSON {
//    struct cJSON *next,*prev;   /* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
//    struct cJSON *child;        /* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

//int type;                   /* The type of the item, as above. */

//char *valuestring;          /* The item's string, if type==cJSON_String */
//int valueint;               /* The item's number, if type==cJSON_Number */
//double valuedouble;         /* The item's number, if type==cJSON_Number */

//char *string;               /* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
//} cJSON;
void json_receive(std::string message, char *&uid_intern, char *&msg_intern, char *&timemsg_intern_str, char *&hash_msg_intern)
{
  // this function receives data message in format: "{'uid':'xxxxx','msg':'yyyy','time':#############}"
  double time_in_number = 0;
  int tipo_dato_time_int;
  int tipo_dato_time_dbl;
  int tipo_dato_time_type;
  char *nombre_del_dato;
  const char *TAG = "JSON_RECEIVE";

  std::replace(message.begin(), message.end(), '\'', '\"');
  char *mensaje3;
  mensaje3 = string2char(message);
  ESP_LOGD(TAG, "%s message Json received:", mensaje3);
  cJSON *el_arreglo = cJSON_Parse(mensaje3);
  uid_intern = cJSON_GetObjectItem(el_arreglo, "uid")->valuestring;
  msg_intern = cJSON_GetObjectItem(el_arreglo, "msg")->valuestring;
  timemsg_intern_str = cJSON_GetObjectItem(el_arreglo, "time")->valuestring;
  hash_msg_intern = cJSON_GetObjectItem(el_arreglo, "hash")->valuestring;

  // TODO: check if received message is a ERR

  // deletes cJSON from memory, this option is pending to test
  //  cJSON_Delete(el_arreglo);
}

// reverse function of Json_receive , used to send values to BLE (take a packet y converts into json)
std::string packet_into_json(packet_t packet_to_convert, std::string BLE_type)
{
  // this function convert [acket data in format: "{'uid':'xxxxx','BLE_type':'yyyy','time':#############,'hash':'XXXXXXXXXX'}"
  char *rpta;
  char *tipo_packet = convertir_packet_type_e_str(packet_to_convert.header.packet_type);
  char *subtipo_packet = convertir_packet_type_e_str(packet_to_convert.header.packet_type);

  if (packet_to_convert.header.packet_type != EMPTY)
  {
    strcpy(rpta, "{'uid':'");
    strcat(rpta, packet_to_convert.header.from);
    strcat(rpta, "','");
    strcat(rpta, string2char(BLE_type));
    strcat(rpta, "':'");
    if (packet_to_convert.header.packet_type == DATA)
    {
      strcat(rpta, packet_to_convert.body.body_data_splitted.payload);
    }
    else
    {
      strcat(rpta, packet_to_convert.body.body_data.payload);
    }
    strcat(rpta, "','type':");
    strcat(rpta, string2char(tipo_packet));
    strcat(rpta, "','subtype':");
    strcat(rpta, string2char(tipo_packet));
    strcat(rpta, "','time':");
    strcat(rpta, string2char(number_to_str(packet_to_convert.header.timestamp)));
    strcat(rpta, "','hash':");
    strcat(rpta, packet_to_convert.header.checksum_data);
    strcat(rpta, "'}");
  }
  return ToString(rpta);
}


/**
   @brief Get gets ESP32 mac address without character :

   @return std::string
*/
std::string get_id_mac()
{
  std::string result = getMacAddress();
  result.erase(std::remove(result.begin(), result.end(), ':'), result.end());
  return result;
}

// print hexadecimal values (ex.: hash160)
void printHex(byte *data, int len)
{
  const char *TAG = "printHex";
  ESP_LOGD(TAG, "%04X", data);
}

// create un unique id based on original factory mac address
void create_unique_id(char *&unique_id_created)
{
  //TODO: create a composed unique id

  char uniqueid3[SIZE_IDNODE];
  uint32_t uChipId;

  uChipId = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  snprintf(uniqueid3, 25, "%08X", uChipId);
  copy_array(uniqueid3, unique_id_created, SIZE_IDNODE);
}

// verify if a given file exists in FS
bool Fileexists(const char *path)
{
  File f = SPIFFS.open(path, "r");
  return (f == true) && !f.isDirectory();
}

// check avaliable memory
std::string freeRam()
{
  return number_to_str(ESP.getFreeHeap());
}

UBaseType_t GetTaskHighWaterMark( TaskHandle_t task_handle )
{
  UBaseType_t uxHighWaterMark;
  uxHighWaterMark = uxTaskGetStackHighWaterMark( task_handle );
  return uxHighWaterMark;
}
/* -----------------------------------------------------------------------------
  GetTaskHighWaterMarkPercent( TaskHandle_t task_handle, uint32_t stack_allotment )

   Input Params:
    - task_handle: The task name you want to examine
    - stack_allotment:  How much stack space did you allocate to it when you created it

  Returns: float with the % of stacke used
  Example:   printf("Stack Used %04.1f%%\r\n", GetTaskHighWaterMarkPercent(xTask1, 2048) );
  Notes:
  -----------------------------------------------------------------------------*/
float GetTaskHighWaterMarkPercent( TaskHandle_t task_handle, uint32_t stack_allotment )
{
  UBaseType_t uxHighWaterMark;
  uint32_t diff;
  float result;

  uxHighWaterMark = uxTaskGetStackHighWaterMark( task_handle );

  diff = stack_allotment - uxHighWaterMark;

  result = ( (float)diff / (float)stack_allotment ) * 100.0;

  return result;
}
