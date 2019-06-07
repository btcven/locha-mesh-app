/**
 * @file NVS.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Arduino.h>
#include <string>
#include <Preferences.h>
#include "nvs.h"
#include "hal/hardware.h"

bool nvsGetBool(const char *name, const char *key, bool defaultValue, bool upset)
{
    const char *tag = "NVS";
    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGD(tag, "namespace %s is open", name);

        bool value = nvs.getBool(key, NULL);

        if (value == NULL)
        {
            if (upset)
            {
                // upset enabled
                ESP_LOGD(tag, "%s %s not found, trying to save it", name, key);
                size_t stored = nvs.putBool(key, defaultValue);
                value = nvs.getBool(key, NULL);
                nvs.end();

                if (value == NULL)
                {
                    // error saving
                    ESP_LOGE(tag, "Saving.. returning default value for %s %s", name, key);
                    return defaultValue;
                }
                else
                {
                    // ok saving
                    ESP_LOGD(tag, "Upset ok for %s %s", name, key);
                    return value;
                }
            }
            else
            {
                // upset disabled
                ESP_LOGD(tag, "%s: %s not found and upset is disabled, using default value", name, key);
                nvs.end();
                return defaultValue;
            }
        }
        else
        {
            nvs.end();
            ESP_LOGD(tag, "Setting found for %s %s", name, key);
            return value;
        }
    }
    else
    {
        ESP_LOGE(tag, "Opening.. the nvs namespace %s", name);
        return defaultValue;
    }
}

esp_err_t nvs_clear(const char *name)
{
    const char *tag = "NVS";

    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGV(tag, "NVS namespace %s is open", name);

        if (nvs.clear())
        {
            nvs.end();
            ESP_LOGD(tag, "Purged the NVS namespace %s", name);
            return ESP_OK;
        }
        else
        {
            nvs.end();
            ESP_LOGE(tag, "Purging.. the NVS namespace %s", name);
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(tag, "Opening.. the NVS namespace %s", name);
        return ESP_FAIL;
    }
}