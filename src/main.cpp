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
#include "ESC_control.h"
#include "SCR.h"

system_status_t system_status;


void setup()
{


    esp_err_t sys_init;
    
    // Battery level.
    sys_init = ESC_init();
    if (sys_init != ESP_OK)
    {
        ESP_LOGE("SETUP", "Under voltage detected");
    }
    else
    {
        ESP_LOGD("SETUP", "Voltage OK");
    }

    // check Screen & start it if available.
    sys_init = SCR_test();
    if (sys_init != ESP_OK)
    {
        ESP_LOGE("SETUP", "Screen module: ERROR");
    }
    else
    {
        ESP_LOGD("SETUP", "Screen module: OK");
        xTaskCreatePinnedToCore(scrTask, "scrTask", 1024 * 2, NULL, 4, NULL, CONFIG_ARDUINO_RUNNING_CORE);
    }
}

void loop()
{

    float bat_lvl = getBatteryLevel();
    float bat_lvl_pct = getBatteryLevelPercent();
    system_status.bat_lvl = bat_lvl;
    system_status.bat_lvl_percent = bat_lvl_pct;

}