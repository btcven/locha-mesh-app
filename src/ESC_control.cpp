/**
 * @file ESC_control.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Arduino.h>
#include "hal/hardware.h"
#include "ESC_control.h"

float getBatteryLevel()
{
    int analog_value = analogRead(13);
    float vs = analog_value * 1.1;
    return vs;
}

float getBatteryLevelPercent()
{
    float bat_lvl = getBatteryLevel();
    float bat_lvl_pct = ((ESC_BAT_MIN - bat_lvl) * 100) / (ESC_BAT_MAX - ESC_BAT_MIN);
    return bat_lvl_pct;
};

esp_err_t ESC_init()
{
    float lvl_pct = getBatteryLevelPercent();
    if (lvl_pct >= 2)
    {
        ESP_LOGD("esc", "Voltage: %d", lvl_pct);
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}
