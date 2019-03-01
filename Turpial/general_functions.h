/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

//#include <EEPROM.h>  // include for EPPROM management
//#include <FS.h>   // Include the SPIFFS library
#include "watchdog.h"


void read_epprom_variables(){
  // Read default variables from EPPROM
  //TODO
}

int64_t xx_time_get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}


String create_unique_id(){
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
   //TODO
   // se arma el unique id
   uint64_t chipid; 
   uint64_t timestamp;
   chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
   timestamp=xx_time_get_time;
   aleatorio=random(1000, 9999);
   
   string uniqueid=String(chipid)+String(aleatorio)+String(timestamp);
// la primera vez que se invoca se marca en el eeprom como escrito (se colocan 3 letras en la memoria como marca)
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');
  EEPROM.write(3,uniqueid); // ocupa 32 bytes , o sea, queda ocupado el eeprom hasta la posicion 35 inclusive
  
  return uniqueid;
}


void split(char* data, char* delim, int index, char* out) {
  char *copy = (char*)malloc(strlen(data) + 1);
  strcpy(copy, data);
  char *str;
  int i = 0;

  while ((str = strtok_r(copy, "|", &copy)) != NULL){ // delimiter is the semicolon
    if (i == index)
      break;
    i++;
  }
  free(copy);
  strcpy(out,str);
}

//  Auxiliar function to handle EEPROM

void EEPROMWritelong(int address, long value){
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
  // commit agregado por roberto
  EEPROM.commit();
}

long EEPROMReadlong(long address){
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void WriteStringToEEPROM(int beginaddress, String string){
  char  charBuf[string.length() + 1];
  string.toCharArray(charBuf, string.length() + 1);
  for (int t =  0; t < sizeof(charBuf); t++)
  {
    EEPROM.write(beginaddress + t, charBuf[t]);
  }
  // commit agregado por roberto
  EEPROM.commit();
}

String  ReadStringFromEEPROM(int beginaddress){
  volatile byte counter = 0;
  char rChar;
  String retString = "";
  while (1)
  {
    rChar = EEPROM.read(beginaddress + counter);
    if (rChar == 0) break;
    if (counter > 31) break;
    counter++;
    retString.concat(rChar);

  }
  return retString;
}

#endif // GENERAL_FUNCTIONS_H
