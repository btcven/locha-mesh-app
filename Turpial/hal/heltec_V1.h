/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef HELTEC_V1_H
#define HELTEC_V1_H

// Los siguientes parametros son tomados por defecto
// algunos de ellos pueden ser sobreescritos en tiempo de ejecución.
#define SCR_ENABLED true
#define RAD_ENABLED true
#define BLE_ENABLED true
#define WST_ENABLED false
#define WAP_ENABLED false
#define LED_ENABLED true
#define BTN_ENABLED true

// Screen Settings
#if SCR_ENABLED
    #define SCR_ADD 0x3c
    #define SCR_SDA 4
    #define SCR_SCL 15
    #define SCR_RST 16
    #define Vext false
#endif // SCR

// Radio Module Settings
#if RAD_ENABLED
    #define RAD_SCK     5
    #define RAD_CSS     18
    #define RAD_RST     14
    #define RAD_MISO    19
    #define RAD_MOSI    27
    #define RAD_DIO0    11
    #define RAD_DIO1    33
    #define RAD_DIO2    32
    #define RAD_BAND    915E6
    #define RAD_PABOOST true
#endif // RAD

#endif // HELTEC_V1_H