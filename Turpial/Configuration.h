/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "boards.h"
#include "language.h"
#include "defaultStartup.h"

// build date and time
#define STRING_VERSION_CONFIG_H __DATE__ "19/02/2019 2047h" __TIME__ 
const int FW_VERSION = 1;  // este es el numero de la version actual

#define BAUDRATE 115200

// for enable debugging 
#define DEBUG true

// The following define selects which electronics board you have.
// Please choose the name from boards.h that matches your setup
#ifndef MOTHERBOARD
  #define MOTHERBOARD HELTEC_V2
#endif
#ifndef RADIOTYPE
  #define RADIOTYPE 1; //1=LORA  // aqui puede ser cualquier variante soportada por LoraLib
#endif
#ifndef RADIOCHIP
  #define RADIOCHIP SX1276  // aqui puede ser cualquier variante soportada por LoraLib
#endif

// caracteristicas que se pueden activar/desactivar en el dispositivo
#define SCR_ENABLED true
#define BLE_ENABLED true
#define RAD_ENABLED true
#define WAP_ENABLED true
#define WST_ENABLED true
#define GPS_ENABLED false

#define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(es)

#endif //CONFIGURATION_H
