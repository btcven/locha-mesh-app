/**
 * @file heltec_v2.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef FEATHERESP32_H_
#define FEATHERESP32_H_

#if DEV_SETUP == 1
    #include "dev-setup.h"
#endif


#define ESC_ENABLED true
#define SCR_ENABLED true
#define RAD_ENABLED true
#define BLE_ENABLED false
#define WST_ENABLED true
#define WAP_ENABLED true
#define LED_ENABLED true
#define BTN_ENABLED true

// Energy Source Control
#if ESC_ENABLED
    #define ESC_PIN     13
    #define ESC_BAT_MAX 4095
    #define ESC_BAT_MIN 2200
    #define ESC_XS      0.000256
    #define ESC_MUL     1000
    #define ESC_MMUL    100
#endif // ESC_ENABLED

#if SCR_ENABLED
    #define SCR_ADD     0x3c
    #define SCR_SDA     4
    #define SCR_SCL     15
    #define SCR_RST     16
    #define Vext        0
#endif // SCR_ENABLED

#if RAD_ENABLED
    #define RAD_TYPE    RFM95
    #define RAD_SCK     5
    #define RAD_CSS     18
    #define RAD_RST     14
    #define RAD_MISO    19
    #define RAD_MOSI    27
    #define RAD_DIO0    26
    #define RAD_DIO1    33
    #define RAD_BAND    915E6
    #define RAD_PABOOST true
    #define RAD_PIN_RECEIVE_DATA    23
#endif // RAD_ENABLED

#if WST_ENABLED
    #if DEV_SETUP == 1
        #define WST_SSID    DEV_WST_SSID
        #define WST_PASS    DEV_WST_PASS
    #else
        #define WST_SSID    ""
        #define WST_PASS    ""
    #endif // DEV_SETUP
#endif // WST_ENABLED

#if WAP_ENABLED
    #define WAP_SSID "io.locha.ap"
    #define WAP_PASS ""
    #define WAP_CHANNEL 6
    #define WAP_MAXCONN 4
#endif // WAP_ENABLED

#if LED_ENABLED
    #define LED_PIN 25
#endif // LED_ENABLED

#if BTN_ENABLED
    #define BTN_GPIO 0
#endif // BTN_ENABLED

#if CONFIG_FREERTOS_UNICORE
    #define ARDUINO_RUNNING_CORE 0
#else
    #define ARDUINO_RUNNING_CORE 1
#endif // CONFIG_FREERTOS_UNICORE

#endif // FEATHERESP32_H_
