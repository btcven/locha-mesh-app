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

system_status_t system_status;

void setup()
{
    esp_err_t sys_init;
    
    // Battery level
    sys_init = ESC_init();
    if (sys_init != ESP_OK)
        ESP_LOGE("SETUP", "Under voltage detected");

    // Screen Test
    sys_init = SCR_test();
    if (sys_init != ESP_OK)
        ESP_LOGE("SETUP", "Screen module: ERROR");

}

void loop() {}