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

#include "WiFi.h"
#include "WAPST.h"
#include "WAP.h"
#include "hal/hardware.h"
#include "NVS.h"

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
    case SYSTEM_EVENT_SCAN_DONE:
        /* code */
        break;
    case SYSTEM_EVENT_STA_START:
        /* code */
        break;
    case SYSTEM_EVENT_STA_STOP:
        /* code */
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        /* code */
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        /* code */
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        /* code */
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        /* code */
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        /* code */
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        /* code */
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        /* code */
        break;
    case SYSTEM_EVENT_AP_START:
        /* code */
        break;
    case SYSTEM_EVENT_AP_STOP:
        /* code */
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        /* code */
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        /* code */
        break;
    case SYSTEM_EVENT_GOT_IP6:
        /* code */
        break;
    case SYSTEM_EVENT_ETH_START:
        /* code */
        break;
    case SYSTEM_EVENT_ETH_STOP:
        /* code */
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        /* code */
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
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
    //
    // only in dev stage:
    // clear nvs before start.
    //
    // nvs_clear("WAP");
    // nvs_clear("WST");

    bool WAP_enabled = nvs_get_bool("WAP", "enabled", WAP_ENABLED, false);
    ESP_LOGD(TAG, "WAP iface enabled: %d", WAP_enabled);

    bool WST_enabled = nvs_get_bool("WST", "enabled", WST_ENABLED, false);
    ESP_LOGD(TAG, "WST iface enabled: %d", WST_enabled);

    wifi_mode_t WIFI_MODE = select_WiFiMode(WAP_enabled, WST_enabled);
    ESP_LOGD(TAG, "Starting WiFi mode: %d", WIFI_MODE);


    WiFi.onEvent(WiFiEvent);

    switch (WIFI_MODE)
    {
    case WIFI_STA:
        ESP_LOGD(TAG, "Trying to start WST iface only");
        return ESP_OK;
        break;
    case WIFI_AP:
        ESP_LOGD(TAG, "Trying to start WAP iface only");
        return WAP_INIT();
        break;
    case WIFI_AP_STA:
        ESP_LOGD(TAG, "Trying to start WAP and WST ifaces");
        return ESP_OK;
        break;
    case WIFI_MODE_NULL:
        ESP_LOGD(TAG, "WAP and WST ifaces are disabled");
        return ESP_OK;
        break;
    default:
        ESP_LOGE(TAG, "Unknow error starting WiFi");
        return ESP_FAIL;
        break;
    }
}