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
#include "dev/scr/scr.h"
#include "dev/rad/rad.h"

system_status_t system_status;

void setup()
{
    esp_err_t sys_init;
    const char *TAG = "SETUP";

    // Battery level
    sys_init = escInit();
    if (sys_init != ESP_OK)
        ESP_LOGE(TAG, "Under voltage detected");

    // Screen Test
    sys_init = scrInit();
    if (sys_init != ESP_OK)
        ESP_LOGE(TAG, "Screen module: ERROR");

    /**
   * @brief RAD iface enabled on boot?
   * 
   */
    ESP_LOGD(TAG, "Starting RADIO");
    sys_init = radInit();
    if (sys_init != ESP_OK)
    {
        esp_restart();
    }
}

void loop() {}