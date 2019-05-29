/**
 * @file heltec_V2.h
 * @author locha.io project developers (dev@locha.io)
 * @brief The following parameters are taken by default and can be changed in execution.
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef HELTEC_V2_H
#define HELTEC_V2_H

/**
 * @brief Start the Screen (SCR) on boot
 * 
 */
#define SCR_ENABLED true

/**
 * @brief Start the Radio (RAD) module on boot
 * 
 */
#define RAD_ENABLED true

/**
 * @brief Start the Bluetooth Low Energy (BLE) iface on boot
 * 
 */
#define BLE_ENABLED true

/**
 * @brief Start WiFi Station (WST) iface on boot
 * 
 */
#define WST_ENABLED true

/**
 * @brief Start the WiFi AP (WAP) iface on boot
 * 
 */
#define WAP_ENABLED true

/**
 * @brief Use the on board LED
 * 
 */
#define LED_ENABLED true

/**
 * @brief Use the on board button
 * 
 */
#define BTN_ENABLED true

// Screen Module Settings
#if SCR_ENABLED
    #define SCR_ADD 0x3c
    #define SCR_SDA 4
    #define SCR_SCL 15
    #define SCR_RST 16
    #define Vext 21
#endif // SCR

// Radio Module Settings
#if RAD_ENABLED
    #define RAD_SCK 5
    #define RAD_CSS 18
    #define RAD_RST 14
    #define RAD_MISO 19
    #define RAD_MOSI 27
    #define RAD_DIO0 26
    #define RAD_DIO1 33
    #define RAD_BAND 915E6
    #define RAD_PABOOST true
    #define RAD_CHIP SX1276
#endif // RAD

// WST default values.
#if WST_ENABLED
    #define WST_SSID ""
    #define WST_PASS ""
#endif // WST_ENABLED

// WAP default values.
#if WAP_ENABLED
    #define WAP_SSID "io.locha.ap"
    #define WAP_PASS "io.locha.ap"
    #define WAP_CHANNEL 6
    #define WAP_MAXCONN 4
#endif // WAP_ENABLED

// On board LED
#if LED_ENABLED
    #define LED_PIN 25
#endif

// On Board Button
#if BTN_ENABLED
    #define BTN_GPIO 0
#endif // BTN

#endif // HELTEC_V2_H
