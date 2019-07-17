/**
 * @file wapst.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
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
