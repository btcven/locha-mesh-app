/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef HARDWAREDEFINITIONS_H_
#define HARDWAREDEFINITIONS_H_

/**
  Different boars can have different configurations.
  Comment out your board or use "CUSTOM" definition
  and set up your pinout.
*/

#define HELTEC
//#define CUSTOM

/* HELTEC HAL */
#ifdef HELTEC

  // Display Module
  #define SCR_SCL     15
  #define SCR_SDA     4
  #define SCR_RST     16
  #define SCR_ADD     0x3c
  #define SCR_Vext    21
  
  // Radio Module
  #define RAD_SCK     0 // pin?
  #define RAD_CSS     18
  #define RAD_RST     14
  #define RAD_MISO    0 // pin?
  #define RAD_MOSI    0 // pin?
  #define RAD_DIO0    26
  #define RAD_DIO1    33
  #define RAD_DIO2    32
  #define RAD_BAND    915E6
  #define RAD_PABOOST true
#endif // HELTEC_V1/V2

#ifdef CUSTOM

  // Display Module
  #define SCR_SCL     22
  #define SCR_SDA     23
  #define SCR_RST     5
  #define SCR_ADD     0x3c
  #define SCR_Vext    NULL
  
   // Radio Module
  #define RAD_SCK     0
  #define RAD_CSS     18
  #define RAD_RST     0
  #define RAD_MISO    0
  #define RAD_MOSI    0
  #define RAD_DIO0    26
  #define RAD_DIO1    33
  #define RAD_DIO2    32
  #define RAD_BAND    915E6 // Freq
  #define RAD_PABOOST true
  
#endif // CUSTOM

#endif // HARDWAREDEFINITIONS_H_
