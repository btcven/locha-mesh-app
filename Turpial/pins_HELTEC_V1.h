/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */

#ifndef PINS_HELTEC_V1_H
  #define PINS_HELTEC_V1_H
  
 // Display Module
  #define SCR_SCL     15
  #define SCR_SDA     4
  #define SCR_RST     16
  #define SCR_ADD     0x3c
  #define SCR_Vext    NULL
  
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

#endif // PINS_HELTEC_V1_H