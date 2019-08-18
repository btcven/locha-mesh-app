/**
 * @file WAPST.cpp
* @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.2.0
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include "WiFi.h"
#include "wapst.h"
#include "wap.h"
#include "wst.h"
#include "hal/hardware.h"
#include "dev/nvs/nvs.h"

/**
 * @brief 
 * 
 * @param evt 
 */
void WiFiEvent(WiFiEvent_t evt)
{
    const char *TAG = "WiFi";

    switch (evt)
    {
    case SYSTEM_EVENT_WIFI_READY:
        ESP_LOGD(TAG, "Event -> WiFi ready #%d", evt);
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        ESP_LOGD(TAG, "Event -> Scann done #%d", evt);
        break;
    case SYSTEM_EVENT_STA_START:
        ESP_LOGD(TAG, "Event -> WST start #%d", evt);
        break;
    case SYSTEM_EVENT_STA_STOP:
        ESP_LOGD(TAG, "Event -> WST stop #%d", evt);
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        ESP_LOGD(TAG, "Event -> WST connected #%d", evt);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        // for example, this event will fired when the wifi iface station
        // is disconected, (*) if in the config file reconnect is true, then
        // call to WiFi.begin(), **no aditional action required**
        // any event must be defined before the main loop or task
        ESP_LOGD(TAG, "Event -> WST disconnected #%d", evt);
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        ESP_LOGD(TAG, "Event -> WST auth mode change #%d", evt);
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        ESP_LOGD(TAG, "Event -> WST got IP #%d", evt);
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        ESP_LOGD(TAG, "Event -> WST lost IP #%d", evt);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        ESP_LOGD(TAG, "Event -> WST WPS er success #%d", evt);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        ESP_LOGD(TAG, "Event -> WST WPS er failed #%d", evt);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        ESP_LOGD(TAG, "Event -> WST er timeout #%d", evt);
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        ESP_LOGD(TAG, "Event -> WST er pin #%d", evt);
        break;
    case SYSTEM_EVENT_AP_START:
        ESP_LOGD(TAG, "Event -> AP start #%d", evt);
        break;
    case SYSTEM_EVENT_AP_STOP:
        ESP_LOGD(TAG, "Event -> AP stop #%d", evt);
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGD(TAG, "Event -> Client connected #%d", evt);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGD(TAG, "Event -> Client disconnected #%d", evt);
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        ESP_LOGD(TAG, "Event -> IP assigned to client #%d", evt);
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        ESP_LOGD(TAG, "Event -> PROBEREQRECVED #%d", evt);
        break;
    case SYSTEM_EVENT_GOT_IP6:
        ESP_LOGD(TAG, "Event -> got ipv6 #%d", evt);
        break;
    case SYSTEM_EVENT_ETH_START:
        ESP_LOGD(TAG, "Event -> ETH start #%d", evt);
        break;
    case SYSTEM_EVENT_ETH_STOP:
        ESP_LOGD(TAG, "Event -> ETH stop #%d", evt);
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        ESP_LOGD(TAG, "Event -> ETH connected #%d", evt);
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        ESP_LOGD(TAG, "Event -> ETH disconnected #%d", evt);
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        ESP_LOGD(TAG, "Event -> ETH got ip #%d", evt);
        break;
    default:
        ESP_LOGD(TAG, "Event -> Unknow event  #%d", evt);
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
wifi_mode_t selectWiFiMode(bool AP, bool ST)
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
esp_err_t wifiStart()
{
    const char *TAG = "WiFi";

    // only in dev stage:
    // clear nvs before start.

    // nvsClear("WAP");
    // nvsClear("WST");

    bool WAP_enabled = nvsGetBool("WAP", "enabled", WAP_ENABLED, true);
    ESP_LOGD(TAG, "WAP iface enabled: %d", WAP_enabled);

    bool WST_enabled = nvsGetBool("WST", "enabled", WST_ENABLED, true);
    ESP_LOGD(TAG, "WST iface enabled: %d", WST_enabled);

    wifi_mode_t WIFI_MODE = selectWiFiMode(WAP_enabled, WST_enabled);
    ESP_LOGD(TAG, "Starting WiFi mode: %d", WIFI_MODE);

    WiFi.onEvent(WiFiEvent);

    switch (WIFI_MODE)
    {
    case WIFI_STA:
        ESP_LOGD(TAG, "Starting WST iface only");
        return wstInit();
        break;
    case WIFI_AP:
        ESP_LOGD(TAG, "Starting WAP iface only");
        return wapInit();
        break;
    case WIFI_AP_STA:
        ESP_LOGD(TAG, "Starting WAP and WST ifaces");
        esp_err_t WAP_isInit, WST_isInit;
        WAP_isInit = wapInit();
        WST_isInit = wstInit();
        if (WAP_isInit && WST_isInit)
        {
            return ESP_OK;
        }
        else
        {
            return ESP_FAIL;
        }
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