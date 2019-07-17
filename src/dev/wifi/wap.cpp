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
    const char *TAG = "WAP";

    // get values from nvs or set default values from hal/hardware.h
    const char *apSSID(nvsGetString(TAG, "ssid", WAP_SSID, true));
    const char *apPassword(nvsGetString(TAG, "pass", WAP_PASS, true));
    int32_t apChannel = nvsGetInt(TAG, "channel", WAP_CHANNEL, true);
    int32_t apMaxConn = nvsGetInt(TAG, "maxconn", WAP_MAXCONN, true);
    bool initAP = WiFi.softAP(apSSID, apPassword, apChannel, 0, apMaxConn);

    if (initAP)
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
