/**
 * @file WAPST.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-26
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */

#include "WAPST.h"
#include "WiFi.h"
#include "hal/hardware.h"
#include "NVS.h"

// determinar tipo de arranque WiFi.
// posibles valores son: STA, AP, APSTA
//
// STA:     if WAP disabled, STA enabled
// AP :     if WAP enabled,  STA disabled
// APSTA:   if WAP enabled,  STA enabled

/**
 * @brief 
 * 
 * @param evt 
 */
void WiFiEvent(WiFiEvent_t evt)
{
    switch (evt)
    {
    case SYSTEM_EVENT_WIFI_READY:
        /* code */
        break;

    default:
        break;
    }
}

/**
 * @brief 
 * 
 * @param AP 
 * @param ST 
 * @return wifi_mode_t 
 */
wifi_mode_t select_WiFiMode(bool AP, bool ST)
{
    if (!AP && ST)
        return WIFI_STA;
    if (AP && !ST)
        return WIFI_AP;
    if (AP && ST)
        return WIFI_AP_STA;
    if (!AP && !ST)
        return WIFI_MODE_NULL;
}

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t WiFi_INIT()
{
    const char *TAG = "WiFi";

    bool WAP_enabled = nvs_get_bool("WAP", "enabled", WAP_ENABLED);
    bool WST_enabled = nvs_get_bool("WST", "enabled", WST_ENABLED);

    wifi_mode_t WIFI_MODE = select_WiFiMode(WAP_enabled, WST_enabled);

    switch (WIFI_MODE)
    {
    case WIFI_STA:
        /* code */
        break;
    case WIFI_AP:
        /* code */
        break;
    case WIFI_AP_STA:
        /* code */
        break;
    case WIFI_MODE_NULL:
        /* code */
        break;
    default:
        break;
    }
}