/**
 * @file main.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.2
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 */
#include <Arduino.h>
#include <SPI.h>
#include "dev/esc/esc.h"
#include "dev/wifi/wapst.h"
#include "dev/scr/scr.h"
#include "wssecure/certify.h"

system_status_t system_status;

void setup(){
    esp_err_t sys_init;

    const char *TAG = "SETUP";

    // Battery
    sys_init = escInit();
    if (sys_init != ESP_OK) {
        ESP_LOGE(TAG, "ESC Under voltage detected");
    } else {
        ESP_LOGV(TAG, "ESC OK");
    }

    // Display
    sys_init = scrInit();
    if (sys_init != ESP_OK){
        ESP_LOGE(TAG, "SCR module: ERROR");
    } else {
        ESP_LOGV(TAG, "SCR module OK");
    }
        
    // WiFi
    sys_init = wifiStart();

    // Sub-GHz Radio

    // self signed Cert. test
    certify_t cert = getCertificate();

}

void loop() {}