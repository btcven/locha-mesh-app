/**
 * @file WST.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-05-09
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include <WiFi.h>
#include "WST.h"
#include "NVS.h"
#include "hal/hardware.h"

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t WST_INIT()
{
    const char *TAG = "WAP";

    // get values from NVS or set default values from hal/hardware.h
    const char *wst_ssid(nvs_get_string(TAG, "ssid", WST_SSID, true));
    const char *wst_pass(nvs_get_string(TAG, "pass", WST_PASS, true));
    bool initST = WiFi.begin(wst_ssid, wst_pass);

    if (initST)
    {
        ESP_LOGD(TAG, "Connected to %s", wst_ssid);
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "Can't connect to %s", wst_ssid);
        return ESP_FAIL;
    }
}