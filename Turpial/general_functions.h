/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

#include <EEPROM.h>  // include for EPPROM management
#include <FS.h>   // Include the SPIFFS library

#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "watchdog.h"

// estructura para la funcion gettimeofday
typedef struct { 
    time_t      tv_sec;     /* seconds since the Epoch*/
    suseconds_t tv_usec;    /* microseconds since the Epoch*/
} timeval_t;

byte read_epprom_variable(int address){
  // Read default variables from EPPROM
  //TODO
  byte value;
  value = EEPROM.read(address);
  return value;
}

int64_t xx_time_get_time() {
  timeval tv;
  gettimeofday(&tv, NULL);
  return (tv.tv_sec * 1000LL + (tv.tv_usec / 1000LL));
}


char* create_unique_id(){
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
   //TODO
   // se arma el unique id
   uint64_t chipid; 
//   uint64_t timestamp;
   char* uniqueid;
// hay que verificar que no exista previamente en EEPROM
   
   chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
 //  timestamp=xx_time_get_time();
   String aleatorio=String(random(1000, 9999),DEC);  // un string de 4 digitos aleatorios
   // el unique id esta conformado por el chipid del ESP concatenado con 4 digitos aleatorios y otros 4 digitos del millis
 //  String uniqueid2="";
 //  uniqueid2.concat(chipid);
   String uniqueid2 = String(chipid);
   uniqueid2.concat(aleatorio);
   uniqueid2.concat(String(millis(), DEC));

  // la primera vez que se invoca se marca en el eeprom como escrito (se colocan 3 letras en la memoria como marca)
  byte value0;
  byte value1;
  byte value2;
  value0=EPPROM.read(0);
  value1=EPPROM.read(1);
  value2=EPPROM.read(2);
  if (!((value0=='C')and(value1=='F')and(value2=='G'))){
    
   char respuesta[16];

snprintf(respuesta, 32, "%08X%04X%04X", (uint32_t)chipid,aleatorio,(uint32_t)millis());
  
      EEPROM.write(0, 'C');
      EEPROM.write(1, 'F');
      EEPROM.write(2, 'G');
      WriteStringToEEPROM(3,uniqueid2);
      //EEPROM.write(3,uniqueid); // ocupa 32 bytes , o sea, queda ocupado el eeprom hasta la posicion 35 inclusive
      return respuesta;
  else {
    uniqueid2=ReadStringFromEEPROM(3);
  }
  uniqueid2.toCharArray(uniqueid2, 32);
  return uniqueid2;
}
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

// esta funcion hace parse de un string a un arreglo dado un separador (por ej. una ip, una mac address o un packet separado por algun caracter
// ej.:
//const char* macStr = "90-A2-AF-DA-14-11";
// byte mac[6];
//parseBytes(macStr, '-', mac, 6, 16);
//////////////////////////////////////////////////////////////////////
void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (int i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}

#endif // GENERAL_FUNCTIONS_H
