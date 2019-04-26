/**
 * @file NVS.cpp
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
#include <Preferences.h>
#include "NVS.h"

bool nvs_get_bool(const char *name, const char *key, bool defaultValue)
{
    Preferences nvs;
    if (nvs.begin(name, false))
    {
        bool value = nvs.getBool(key, NULL);

        if (value == NULL)
        {
            ESP_LOGD("NVS", "%s %s not found, trying to save it", name, key);
            size_t stored = nvs.putBool(key, defaultValue);
            value = nvs.getBool(key, NULL);
            nvs.end();
            if (value == NULL)
            {
                ESP_LOGE("NVS", "Save Error, returning default value for %s %s", name, key);
                return defaultValue;
            }
            else
            {
                ESP_LOGD("NVS", "Setting found for %s %s", name, key);
                return value;
            }
        }
        else
        {
            nvs.end();
            ESP_LOGD("NVS", "Setting found for %s %s", name, key);
            return value;
        }
    }
    else
    {
        ESP_LOGE("NVS", "Open Error");
        return defaultValue;
    }
}