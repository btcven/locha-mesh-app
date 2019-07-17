/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Arduino.h>
#include <SPI.h>
#include "dev/esc/esc.h"
#include "dev/wifi/wapst.h"
#include "dev/scr/scr.h"

system_status_t system_status;

void setup()
{
    esp_err_t sys_init;

    const char *TAG = "SETUP";

    // Battery
    sys_init = escInit();
    if (sys_init != ESP_OK)
        ESP_LOGE(TAG, "Under voltage detected");

    // Display
    sys_init = scrInit();
    if (sys_init != ESP_OK)
        ESP_LOGE(TAG, "Screen module: ERROR");

    // WiFi
    sys_init = wifiStart();

    // Sub-GHz Radio 


}

void loop() {}