/**
 * @file RAD.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include <LoRaLib.h>
#include "RAD.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "NVS.h"

/**
 * @brief Start Radio interface
 * 
 * @return esp_err_t 
 */
esp_err_t RAD_INIT()
{
    const char *TAG = "RADIO";
    // only in dev stage:
    // clear nvs before start.
    //
    // nvs_clear("RADIO");

    bool RAD_enabled = nvs_get_bool(TAG, "enabled", RAD_ENABLED, true);

    if (RAD_enabled)
    {
        ESP_LOGD(TAG, "Starting RADIO...");
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}
