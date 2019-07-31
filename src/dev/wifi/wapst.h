/**
 * @file wapst.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.2.0
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#ifndef WAPST_H_
#define WAPST_H_
#include "WiFi.h"

/**
 * @brief 
 * 
 * @param evt 
 */
void WiFiEvent(WiFiEvent_t evt);

/**
 * @brief 
 * 
 * @param AP 
 * @param ST 
 * @return wifi_mode_t 
 */
wifi_mode_t selectWiFiMode(bool AP, bool ST);

/**
 * @brief Initialize WiFi ifaces
 * 
 * @return esp_err_t 
 */
esp_err_t wifiStart();

#endif // WAPST_H_
