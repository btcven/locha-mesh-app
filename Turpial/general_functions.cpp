/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

 // declaracion de librerias
#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <cJSON.h>
#include <iostream>
#include <Hash.h>   // de la libreria uBitcoin https://gitlab.com/btcven/locha/uBitcoin/tree/master
#include "esp_system.h"
#include "general_functions.h"
#include "debugging.h"


// funcion para comparar dos arreglos de char
bool compare_char(char* src ,char* dst){
  if (strcmp(src, dst) == 0){ 
    return true;
  } else { 
    return false;
  }
}

// Funcion de conversion de tipo de datos: Char* a long long
// usada para convertir timestamps que  vienen en cadenas de caracteres a un numero tipo long long 
long long char2LL(char *str)
{
  long long result = 0; // Initialize result
  // Iterate through all characters of input string and update result
  for (int i = 0; str[i] != '\0'; ++i)
    result = result*10 + str[i] - '0';
  return result;
}

// Funcion de conversion de tipo de datos: Texto a uint8_t
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


// Funcion de conversion de tipo de datos: String a char*
char *string2char(String command)
{
  if (command.length() != 0)
  {
    char *p = const_cast<char *>(command.c_str());
    return p;
  }
}

// Copia arreglos de char (src->dst) dado un largo determinado len
void copy_array_locha(char *src, char *dst, int len)
{
  for (int i = 0; i < len; i++)
  {
    *dst++ = *src++;
  }
}

// obtiene la macaddress del ESP32 una vez conectado a Wifi
String getMacAddress() {
  uint8_t baseMac[6];
  // Get MAC address for WiFi station
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
  return String(baseMacChr);
}

// funcion encargada de colocar los id de nodo en mayusculas
char* node_name_char_to_uppercase(char array_temp[16]){
    String chars_temp=array_temp;
    chars_temp.toUpperCase();
    return string2char(chars_temp);
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
void json_receive(String message, char* &uid_intern,char* &msg_intern, char* &timemsg_intern_str, char* &hash_msg_intern ){
 // this function receives data message in format: "{'uid':'xxxxx','msg':'yyyy','time':#############}"
 double time_in_number=0;
  int tipo_dato_time_int;
  int tipo_dato_time_dbl;
  int tipo_dato_time_type;
  char* nombre_del_dato;
  
    message.replace("'","\"");
    message=message.c_str();
    char mensaje3[message.length()+1];
    message.toCharArray(mensaje3,message.length()+1);
    DEBUG_PRINT(F("mensaje completo Json recibido:"));
    DEBUG_PRINTLN(mensaje3);
    cJSON* el_arreglo=cJSON_Parse(mensaje3);
    uid_intern = cJSON_GetObjectItem(el_arreglo, "uid")->valuestring;
    msg_intern = cJSON_GetObjectItem(el_arreglo, "msg")->valuestring;
    timemsg_intern_str = cJSON_GetObjectItem(el_arreglo, "time")->valuestring;
    hash_msg_intern = cJSON_GetObjectItem(el_arreglo, "hash")->valuestring;

    // TODO: verificar si se recibe en lugar de msg un ERR
 
   
 
 // deletes cJSON from memory, ESTA OPCION ESTA PENDIENTE DE PRUEBA
//  cJSON_Delete(el_arreglo);

}

// procedimiento inverso a Json_receive para devolver valores al BLE (toma un packet y lo transforma en un json)
String packet_into_json(packet_t packet_to_convert, String BLE_type){
   // this function convert [acket data in format: "{'uid':'xxxxx','BLE_type':'yyyy','time':#############,'hash':'XXXXXXXXXX'}"
  String rpta;
  char* tipo_packet=convertir_packet_type_e_str(packet_to_convert.header.type);
 // tipo_packet.toLowerCase();
  if (packet_to_convert.header.type!=EMPTY){
      rpta="{";
      rpta=rpta+"'uid':'"+(String)packet_to_convert.header.from+"',";
      rpta=rpta+"'"+BLE_type+"':'"+(String)packet_to_convert.body.payload+"',";
      rpta=rpta+"'type':"+(String)convertir_str_packet_type_e(tipo_packet)+",";
      rpta=rpta+"'time':"+(String)packet_to_convert.header.timestamp+",";
      rpta=rpta+"'hash':'"+(String)packet_to_convert.header.hash+"',";
      rpta=rpta+"}";
  }
  return rpta;
}

String get_id_mac() {
  String result=getMacAddress();
  result.replace(":","");
  return result;
}

// funcion usada para imprimir valores hexadecimales (tipo hash160)
void printHex(byte * data, int len){
    for(int i=0; i<len; i++){
        if(data[i] < 0x10){
            Serial.print("0");
        }
        Serial.print(data[i], HEX);
    }
    Serial.println();
}

void create_unique_id(char *&unique_id_created) {
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
  //TODO: armar el unique id como un compuesto donde el user pueda colocar una parte del uniqueid y el resto sea el chipid (completo o una parte) y algun caracter de validacion
  
  char uniqueid3[16];
  uint32_t uChipId;
  
  uChipId = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
    
  snprintf(uniqueid3, 25, "%08X", uChipId);
  copy_array_locha(uniqueid3, unique_id_created, 16);
  
}

// esta funcion verifica si el hash del mensaje es valido comparando el hash160
bool is_valid_hash160(char* mensaje, char* hash_recibido){

    byte hash[20] = {0};
    
   // hash160(mensaje, strlen(mensaje), hash);
    Serial.print("Hash recibido:");
    Serial.println(hash_recibido);
    Serial.print("Hash del mensaje:");
    Serial.print((String)mensaje);
    Serial.print("-----hash:");
 //   printHex(hash, sizeof(hash));

  return true;
}

// verifica la cantidad de memoria disponible libre
String freeRam()
{
  return (String)ESP.getFreeHeap();
}
