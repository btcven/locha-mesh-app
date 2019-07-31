/**
 * @file ESC_control.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#include <Arduino.h>
#include "hal/hardware.h"
#include "esc.h"

float getBatteryLevel(int analog_pin)
{
    int analog_value = analogRead(analog_pin);
    float vs = analog_value * 1.1;
    return vs;
}

float getBatteryLevelPercent(int analog_pin)
{
    float bat_lvl = getBatteryLevel(analog_pin);
    float bat_lvl_pct = ((ESC_BAT_MIN - bat_lvl) * 100) / (ESC_BAT_MAX - ESC_BAT_MIN);
    return bat_lvl_pct;
};

esp_err_t escInit()
{
    float lvl_pct = getBatteryLevelPercent(ESC_PIN);
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
