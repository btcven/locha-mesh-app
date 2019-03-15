/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

  
*/

#ifndef BOARDS_DEF_H 
#define BOARDS_DEF_H

// detecting MCU compiler
#if defined(__AVR__)
    #define MCU_ARDUINO
#endif 
// definicion para ESP32 generico
#ifdef ARDUINO_ARCH_ESP32
    #define MCU_ESP32
#endif 
// definiciones para HELTEC
#ifdef WIFI_LORA_32_V2
    #define MCU_ESP32
#endif 
#ifdef ARDUINO_WIFI_LORA_32_V2
    #define MCU_ESP32
#endif
#ifdef Heltec_WIFI_LoRa_32
    #define MCU_ESP32
#endif
#ifdef HELTEC_WIFI_LORA_32
    #define MCU_ESP32
#endif
#ifdef WIFI_KIT_32
    #define MCU_ESP32
#endif
#ifdef WIFI_LORA_32
    #define MCU_ESP32
#endif
#ifdef WIRELESS_STICK
    #define MCU_ESP32
#endif
#ifdef ESP32_DEV
    #define MCU_ESP32
#endif
#ifdef TEENSY36
    #define MCU_ARDUINO
#endif


#endif BOARDS_DEF_H
