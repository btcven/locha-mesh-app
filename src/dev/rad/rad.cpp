/**
 * @file rad.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Arduino.h>
#include "rad.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "dev/nvs/nvs.h"

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t rad_init()
{
    const char *TAG = "RAD";
    // only in dev stage, clear nvs before start:
    // nvsClear(TAG);

    bool rad_enabled = nvsGetBool(TAG, "enabled", RAD_ENABLED, true);

    if (rad_enabled)
    {
        ESP_LOGD(TAG, "%s %S", MSG_STARTING, MSG_RADIO);
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}