#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include <cJSON.h>
#include <iostream>
#include "general_functions.h"




extern char* uid;
extern char* msg;
extern double timemsg;
extern char* hash_msg;



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
 Serial.print(F("mensaje completo recibido:"));
 Serial.println(mensaje3);
 cJSON* el_arreglo=cJSON_Parse(mensaje3);
 uid_intern = cJSON_GetObjectItem(el_arreglo, "uid")->valuestring;
 msg_intern = cJSON_GetObjectItem(el_arreglo, "msg")->valuestring;
 timemsg_intern_str = cJSON_GetObjectItem(el_arreglo, "time")->valuestring;
 hash_msg_intern = cJSON_GetObjectItem(el_arreglo, "hash")->valuestring;
 Serial.print(F("uid dentro de json_receive:"));
 Serial.println(uid_intern);
 Serial.print(F("msg dentro de json_receive:"));
 Serial.println(msg_intern);
 
   
 
 // deletes cJSON from memory
//  cJSON_Delete(el_arreglo);

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

// esta funcion verifica si el hash del mensaje es valido
bool is_valid_hash160(char* mensaje, char* hash_recibido){
// aqui se debe adicionar las libs para hash160

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
