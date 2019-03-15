#include <Arduino.h>
#include <string.h> 
#include "route.h"

char *create_unique_id()
{
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
  //TODO
  // se arma el unique id
  uint64_t chipid;
  char *uniqueid;
  
  #ifdef MCU_ARDUINO
    String aleatorio=String(random(1000, 9999),DEC);  // un string de 4 digitos aleatorios
    aleatorio.toCharArray(uniqueid, 32);
  #endif 
  #ifdef MCU_ESP32
    chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
    char ssid[23];
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(ssid, 23, "MCUDEVICE-%04X%08X", chip, (uint32_t)chipid);
    String uniqueid2;
    uniqueid2 = String(ssid);
    uniqueid2.toCharArray(uniqueid, 32);
  #endif 
  
  return uniqueid;
}

char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}
