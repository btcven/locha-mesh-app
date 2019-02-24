#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "boards.h"
#include "language.h"
#include "defaultStartup.h"

// build date and time
#define STRING_VERSION_CONFIG_H __DATE__ "19/02/2019 2047h" __TIME__ 
const int FW_VERSION = 1;  // este es el numero de la version actual

#define BAUDRATE 115200

// The following define selects which electronics board you have.
// Please choose the name from boards.h that matches your setup
#ifndef MOTHERBOARD
  #define MOTHERBOARD HELTEC
#endif

// caracteristicas que se pueden activar/desactivar en el dispositivo
#define SCR_ENABLED true
#define BLE_ENABLED true
#define RAD_ENABLED true
#define WAP_ENABLED true
#define WST_ENABLED true

#define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(es)

#endif //CONFIGURATION_H
