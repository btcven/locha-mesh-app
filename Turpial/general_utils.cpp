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

// funcion para convertir un std::string en un char*
char *std_string_to_char(std::string cadena)
{
  char *cstr = new char[cadena.length() + 1];
  strcpy(cstr, cadena.c_str());
  return cstr;
}

// Funcion de conversion de tipo de datos: Char* a long long
// usada para convertir timestamps que  vienen en cadenas de caracteres a un numero tipo long long
long long char2LL(char *str)
{
  long long result = 0; // Initialize result
  // Iterate through all characters of input string and update result
  for (int i = 0; str[i] != '\0'; ++i)
    result = result * 10 + str[i] - '0';
  return result;
}

// Funcion de conversion de tipo de datos: Texto a uint8_t
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

// Funcion de conversion de tipo de datos: std::string a char*
char *string2char(std::string command)
{
  if (command.length() != 0)
  {
    char *p = const_cast<char *>(command.c_str());
    return p;
  }
}

// Copia arreglos de char (src->dst) dado un largo determinado len
void copy_array(char *src, char *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}

// obtiene la macaddress del ESP32 una vez conectado a Wifi
std::string getMacAddress()
{
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return (baseMacChr);
}

// funcion para convertir un valor en un tipo std::string
template <typename T>
std::string ToString(T val)
{
  std::stringstream stream;
  stream << val;
  return stream.str();
}

// funcion para convertir un numero en un std::string
std::string number_to_str(unsigned long long i)
{
  std::string str = ToString(i);
  return str;
}

// convierte un string en un numero tipo long
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
  // message.replace("'","\"");
  // message=message.c_str();
  char *mensaje3;
  //message.toCharArray(mensaje3,message.length()+1);
  mensaje3 = string2char(message);
  ESP_LOGD(TAG, "%s mensaje completo Json recibido:", mensaje3);
  cJSON *el_arreglo = cJSON_Parse(mensaje3);
  uid_intern = cJSON_GetObjectItem(el_arreglo, "uid")->valuestring;
  msg_intern = cJSON_GetObjectItem(el_arreglo, "msg")->valuestring;
  timemsg_intern_str = cJSON_GetObjectItem(el_arreglo, "time")->valuestring;
  hash_msg_intern = cJSON_GetObjectItem(el_arreglo, "hash")->valuestring;

  // TODO: verificar si se recibe en lugar de msg un ERR

  // deletes cJSON from memory, ESTA OPCION ESTA PENDIENTE DE PRUEBA
  //  cJSON_Delete(el_arreglo);
}

// procedimiento inverso a Json_receive para devolver valores al BLE (toma un packet y lo transforma en un json)
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

// devuelve la mac address del ESP32
std::string get_id_mac()
{
  std::string result = getMacAddress();
  result.erase(std::remove(result.begin(), result.end(), ':'), result.end());
  return result;
}

// funcion usada para imprimir valores hexadecimales (tipo hash160)
void printHex(byte *data, int len)
{
  const char *TAG = "printHex";
  ESP_LOGD(TAG, "%04X", data);
}

// se genera un unique id basado en la mac address original de fabrica del equipo
void create_unique_id(char *&unique_id_created)
{
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
  //TODO: armar el unique id como un compuesto donde el user pueda colocar una parte del uniqueid y el resto sea el chipid (completo o una parte) y algun caracter de validacion

  char uniqueid3[SIZE_IDNODE];
  uint32_t uChipId;

  uChipId = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  snprintf(uniqueid3, 25, "%08X", uChipId);
  copy_array(uniqueid3, unique_id_created, SIZE_IDNODE);
}

// verifica si existe un archivo en el FS
bool Fileexists(const char *path)
{
  File f = SPIFFS.open(path, "r");
  return (f == true) && !f.isDirectory();
}

// verifica la cantidad de memoria disponible libre
std::string freeRam()
{
  return number_to_str(ESP.getFreeHeap());
}
