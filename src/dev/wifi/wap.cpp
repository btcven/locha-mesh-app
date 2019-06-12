/**
 * @file wap.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <WiFi.h>
#include "wap.h"
#include "dev/nvs/nvs.h"
#include "hal/hardware.h"

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t wapInit()
{
    const char *tag = "WAP";

    // get values from nvs or set default values from hal/hardware.h
    /* 
    const char *apSSID(nvs_get_string(TAG, "ssid", WAP_SSID, true));
    const char *apPassword(nvs_get_string(TAG, "pass", WAP_PASS, true));
    int32_t apChannel = nvs_get_int(TAG, "channel", WAP_CHANNEL, true);
    int32_t apMaxConn = nvs_get_int(TAG, "maxconn", WAP_MAXCONN, true);
    bool initAP = WiFi.softAP(apSSID, apPassword, apChannel, 0, apMaxConn);
    */

    //if (initAP)
    if (true)
    {
        ESP_LOGD(TAG, "AP init OK");
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "AP init ERROR");
        return ESP_FAIL;
    }
}
