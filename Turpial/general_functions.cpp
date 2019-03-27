#include <Arduino.h>
#include <string.h>
#include <WiFi.h>
#include "hal/hardware.h"
#include "route.h"

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