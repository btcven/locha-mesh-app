/**
 * @file WAP.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include <Arduino.h>
#include "WAP.h"


extern WiFiServer server;

bool load_config(){

    return true;
}

esp_err_t WAP_INIT()
{
    const char *TAG = "WAP";
    bool WAP_enabled = nvs_get_bool(TAG, "enabled", WAP_ENABLED);
    if (WAP_enabled){
            // WAP init
            if (load_config())
            {
                ESP_LOGD(TAG, "Loading config WAP OK");
                return ESP_OK;
            } else {
                ESP_LOGE(TAG, "Error Loading config WAP");
                return ESP_FAIL;
            }
            // starting WIFI SoftAP
            ESP_LOGD(TAG, "Starting WIFI SoftAP");
      
   } else {
       ESP_LOGD(TAG, "WAP disabled"); 
   }
}

/**
 * @brief function for wifi response management
 * 
 */
void responses_WAP(WiFiClient client){
    if (client) { 
    while (client.connected()) {
        if (client.available()) {
           
        }
    }
  // close the connection:
    client.stop();
    }
}
