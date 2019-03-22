/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
   * Boards definition 
   * Hardware status definitions
*/

#ifndef HARDWARE_H
#define HARDWARE_H

#define DEBUG
// define serial settings
#define BAUDRATE 115200
#if defined(__AVR__)
  #include "hardware_ARDUINO.h"
#endif
#ifdef TEENSY36
  #include "hardware_ARDUINO.h"
#endif
#ifdef ARDUINO_ARCH_ESP32
  #include "hardware_HELTEC.h"
#endif
#ifdef ESP8266
  #include "hardware_ESP8266.h"
#endif
#ifdef WIFI_LORA_32_V2
  #include "hardware_HELTEC.h"
#endif
#ifdef ARDUINO_WIFI_LORA_32_V2
  #include "hardware_HELTEC.h"
#endif
#ifdef Heltec_WIFI_LoRa_32
  #include "hardware_HELTEC.h"
#endif
#ifdef HELTEC_WIFI_LORA_32
  #include "hardware_HELTEC.h"
#endif
#ifdef WIFI_LORA_32
  #include "hardware_HELTEC.h"
#endif


typedef struct
{
  bool isEnabled;
  bool isActive;
} SCR_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
  bool isConnected;
} BLE_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
  bool isConnected;
} WST_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
  // this may need a better approach:
  // client_t[max_clients]
} WAP_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
} RAD_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
  uint8_t vBat;
} BAT_status_t;

typedef struct
{
  bool isEnabled;
  bool isActive;
  int32_t lat;
  int32_t lon;
} GPS_status_t;


#endif // HARDWARE_H
