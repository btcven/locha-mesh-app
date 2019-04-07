#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <cJSON.h>
#include <iostream>
#include "esp_system.h"
#include "general_functions.h"
#include "debugging.h"

extern char* uid;
extern char* msg;
extern double timemsg;
extern char* hash_msg;


long long char2LL(char *str)
{
  long long result = 0; // Initialize result
  // Iterate through all characters of input string and update result
  for (int i = 0; str[i] != '\0'; ++i)
    result = result*10 + str[i] - '0';
  return result;
}

uint8_t convert_str_to_uint8(String texto){
 
 unsigned long long y = 0;
for (int i = 0; i < texto.length(); i++) {
    char c = texto.charAt(i);
   if (c < '0' || c > '9') break;
   y *= 10;
   y += (c - '0');
}

  return y;
}


String getMacAddress() {

  union
  {
    uint64_t  llmac;
    uint8_t   mac[6];
  } mac_t;

  ESP_ERROR_CHECK(esp_efuse_mac_get_default(mac_t.mac));
  char szMac[21];
  snprintf(szMac, sizeof(szMac), "%" PRIu64, mac_t.llmac);
  Serial.println("continuo al otro tipo de Mac address:");
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return String(baseMacChr);
}


char *string2char(String command)
{
  if (command.length() != 0)
  {
    char *p = const_cast<char *>(command.c_str());
    return p;
  }
}

void copy_array_locha(char *src, char *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}


String random_name(int numBytes)
{
  char *msg;
  uint8_t i;
  String respuesta;
  byte randomValue;
  memset(msg, 0, numBytes);
  for (i = 0; i < numBytes; i++)
  {
    randomValue = random(0, 37);
    msg[i] = randomValue + 'a';
    if (randomValue > 26)
    {
      msg[i] = (randomValue - 26) + '0';
    }
  }
  respuesta = (String)msg;
  respuesta = "locha_" + respuesta;
  return respuesta;
}

// funcion encargada de colocar los id de nodo en mayusculas
char* node_name_char_to_uppercase(char array_temp[16]){
    String chars_temp=array_temp;
    chars_temp.toUpperCase();
    return string2char(chars_temp);
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
void json_receive(String message, char* &uid2,char* &msg2, double &timemsg2 ){
 // this function receives data message in format: "{'uid':'xxxxx','msg':'yyyy','time':#############}"
  message.replace("'","\"");
  message=message.c_str();
  char mensaje3[message.length()+1];
  message.toCharArray(mensaje3,message.length()+1);
 
  cJSON* el_arreglo=cJSON_Parse(mensaje3);
  uid2 = cJSON_GetObjectItem(el_arreglo, "uid")->valuestring;
 
  msg2 = cJSON_GetObjectItem(el_arreglo, "msg")->valuestring;
 
  timemsg2 = cJSON_GetObjectItemCaseSensitive(el_arreglo, "time")->valuedouble;
   char* timemsg3 = cJSON_GetObjectItemCaseSensitive(el_arreglo, "time")->valuestring;
   Serial.print("recibi time:");
   Serial.print((String)timemsg2);
   Serial.print("-");
   Serial.println((String)timemsg3);
 // deletes cJSON from memory
  cJSON_Delete(el_arreglo);

}


void create_unique_id(char *&unique_id_created) {
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
  //TODO
  // se arma el unique id
  char uniqueid3[16];

  uint32_t uChipId;
  // uint64_t uChipId2;

  uChipId = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
                               ///Serial.printf("ESP32 Chip ID = %04X",(uint16_t)(chipid>>32));//print High 2 bytes
                               //Serial.printf("%08X\n",(uint32_t)chipid);//print Low 4bytes.
                               //char ssid[23];
                               //uint16_t chip = (uint16_t)(chipid >> 32);
                               //snprintf(ssid, 23, "%04X%08X", chip, (uint32_t)chipid);
                               //copy_array_locha(ssid, uniqueid3, 16);


  //  Serial.printf("ESP Chip ID = %04X",(uint16_t)(uChipId>>32));//print High 2 bytes
  //  Serial.printf("%08X\n",(uint32_t)uChipId);//print Low 4bytes.
  //      char clientid[25];
  //      Serial.print("sigo:");
  snprintf(uniqueid3, 25, "%08X", uChipId);
  //    Serial.println(clientid);
  //    copy_array_locha(clientid, uniqueid3, 16);
  copy_array_locha(uniqueid3, unique_id_created, 16);
  //  return uniqueid3;
}

boolean isNumeric(String str)
{
  unsigned int stringLength = str.length();

  if (stringLength == 0)
  {
    return false;
  }

  boolean seenDecimal = false;

  for (unsigned int i = 0; i < stringLength; ++i)
  {
    if (isDigit(str.charAt(i)))
    {
      continue;
    }

    if (str.charAt(i) == '.')
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

// verifica la cantidad de memoria disponible libre
String freeRam()
{
  return (String)ESP.getFreeHeap();
}

// lee el voltaje interno de trabajo del arduino
long readVcc()
{
  // return para todo lo demas que no sea Arduino
  return 0;
}
