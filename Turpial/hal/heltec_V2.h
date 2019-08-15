/**
 * @file heltec_V2.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
 */

#ifndef HELTEC_V2_H
#define HELTEC_V2_H

// Los siguientes parametros son tomados por defecto
// algunos de ellos pueden ser sobreescritos en tiempo de ejecución.
#define SCR_ENABLED     true
#define RAD_ENABLED     true
#define BLE_ENABLED     true
#define WST_ENABLED     true
#define WAP_ENABLED     true
#define LED_ENABLED     true
#define BTN_ENABLED     true

// Screen Settings
#if SCR_ENABLED
    #define SCR_ADD     0x3c
    #define SCR_SDA     4
    #define SCR_SCL     15
    #define SCR_RST     16
    #define Vext        21
#endif // SCR

// Radio Module Settings
#if RAD_ENABLED
    #define RAD_SCK     5
    #define RAD_CSS     18
    #define RAD_RST     14
    #define RAD_MISO    19
    #define RAD_MOSI    27
    #define RAD_DIO0    26
    #define RAD_DIO1    33
    #define RAD_BAND    915E6
    #define RAD_PABOOST true
    #define RAD_CHIP    SX1276
#endif // RAD

// WST default values.
#if WST_ENABLED
    #define WST_SSID    ""
    #define WST_PASS    ""
#endif // WST_ENABLED

// WAP default values.
#if WAP_ENABLED
    #define WAP_SSID    "io.locha.ap"
    #define WAP_PASS    "io.locha.ap"
    #define WAP_CHANNEL 6
    #define WAP_MAXCONN 4
#endif // WAP_ENABLED

// On board LED
#if defined(LED_ENABLED)
    #define LED_PIN 25
#endif

// On Board Button
#if BTN_ENABLED
    #define BTN_GPIO 0
#endif // BTN

#endif // HELTEC_V2_H
