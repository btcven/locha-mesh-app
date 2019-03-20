
/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef HARDWARE_HELTEC_H
#define HARDWARE_HELTEC_H

#define ESP32_BASED_DEVICE
//#define ESP8266_BASED_DEVICE
//#define NOWIFI_DEVICE

// define display settings.
#define SCR_ENABLED false
#ifdef SCR_ENABLED
  #define SCR_ADD 0x3C
  #define SCR_RST 16
  #define SCR_SDA 4
  #define SCR_SCL 15
  #define SCR_Vext 21
#endif // SCR

// Default BLE  settings
#define BLE_ENABLED true
#ifdef BLE_ENABLED
// -- Default BLE Settings BLOCK --
#endif // BLE

// Radio settings
#define RAD_ENABLED true
#ifdef RAD_ENABLED

// -- Default RAD Settings BLOCK --
#endif // RAD

// Default WAP Settings
#define WAP_ENABLED false
#ifdef WAP_ENABLED
// -- Default WAP Settings BLOCK --
#endif // WAP

#define WST_ENABLED false
#ifdef WST_ENABLED
// -- Default WST Settings BLOCK --
#endif // WST

// Default LED Settings.
#define LED_ENABLED true
#ifdef LED_ENABLED
// -- Default LED Settings BLOCK --
#endif // LED

// Default Button Settings
#define BUTTON_ENABLED true
#ifdef BUTTON_ENABLED
// -- Default BUTTON Settings BLOCK --
#endif // BUTTON

#endif   //HARDWARE_HELTEC_H