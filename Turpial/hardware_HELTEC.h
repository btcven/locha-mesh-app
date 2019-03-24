
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
#define SCR_ENABLED true
#ifdef SCR_ENABLED
  #define SCR_ADD 0x3C
  #define RST_OLED 16
  #define SDA_OLED 4
  #define SCL_OLED 15
  
  #define OLED_UPDATE_INTERVAL 500 
#endif // SCR
// se marca si es un heltec V2 o se coloca comentado si es un heltec v1
#define V2  1

#ifdef V2 //WIFI Kit series V1 not support Vext control
  #define Vext  21
#endif

// Default BLE  settings
#define BLE_ENABLED true
#ifdef BLE_ENABLED
// -- Default BLE Settings BLOCK --
#endif // BLE

// Radio settings
#define RAD_ENABLED true
#ifdef RAD_ENABLED

// -- Default RAD Settings 
// Radio Module
 // #define RAD_SCK     5 // pin?
//  #define RAD_CSS     18
//  #define RST_LoRa     14
//  #define RAD_MISO    19 // pin?/
//  #define RAD_MOSI    27 // pin?
//  #define DIO0    26
//  #define DIO1    33
//  #define DIO2    32
//  #define RAD_BAND    915E6
//  #define RAD_PABOOST true
 

#endif // RAD

// Default WAP Settings
#define WAP_ENABLED false
#ifdef WAP_ENABLED
// -- Default WAP Settings BLOCK --
#endif // WAP

#define WST_ENABLED true
#ifdef WST_ENABLED
// -- Default WST Settings BLOCK --
#endif // WST

// Default LED Settings.
#define LED_ENABLED true
#ifdef LED_ENABLED
// -- Default LED Settings BLOCK --
  #define LED_PIN 25
#endif // LED

// Default Button Settings
#define BUTTON_ENABLED true
#ifdef BUTTON_ENABLED
// -- Default BUTTON Settings BLOCK --
#endif // BUTTON

#endif   //HARDWARE_HELTEC_H
