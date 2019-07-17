/**
 * @file wst.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Arduino.h> 
#include <WiFi.h>
#include "wst.h"
#include "dev/nvs/nvs.h"
#include "hal/hardware.h"

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t wstInit()
{
    const char *TAG = "WST";

    // get values from NVS or set default values from hal/hardware.h

    const char *wst_ssid(nvsGetString(TAG, "ssid", WST_SSID, true));
    const char *wst_pass(nvsGetString(TAG, "pass", WST_PASS, true));
    bool initST = WiFi.begin(wst_ssid, wst_pass);
    
    if(initST)
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
