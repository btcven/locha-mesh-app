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

esp_err_t nvs_clear(const char *name)
{
    const char *TAG = "NVS";

    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGV(TAG, "NVS namespace %s is open", name);

        if (nvs.clear())
        {
            nvs.end();
            ESP_LOGD(TAG, "Purged the NVS namespace %s", name);
            return ESP_OK;
        }
        else
        {
            nvs.end();
            ESP_LOGE(TAG, "Purging.. the NVS namespace %s", name);
            return ESP_FAIL;
        }
    }
    else
    {
        ESP_LOGE(TAG, "Opening.. the NVS namespace %s", name);
        return ESP_FAIL;
    }
}

const char *nvs_get_string(const char *name, const char *key, const char *defaultValue, bool upset)
{
    const char *TAG = "NVS";

    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGD(TAG, "NVS namespace %s is open", name);
        // leer
        // si valor no existe -> guardar valor por defecto. openned
        // leer -> return value
        return defaultValue;
    }
    else
    {
        ESP_LOGE(TAG, "Opening.. the NVS namespace %s, using default value", name);
        return defaultValue;
    }
};

int32_t nvs_get_int(const char *name, const char *key, int32_t defaultValue, bool upset)
{
    const char *TAG = "NVS";

    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGV(TAG, "NVS namespace %s is open", name);
        int32_t value = nvs.getInt(key, NULL);
        if (value == NULL)
        {
            if (upset)
            {
                // upset enabled
                ESP_LOGD(TAG, "%s %s not found, trying to save it", name, key);
                size_t stored = nvs.putInt(key, defaultValue);
                value = nvs.getInt(key, NULL);
                nvs.end();
                if (value == NULL)
                {
                    // error saving
                    ESP_LOGE(TAG, "Saving.. returning default value for %s %s", name, key);
                    return defaultValue;
                }
                else
                {
                    // ok saving
                    ESP_LOGD(TAG, "Upset ok for %s %s", name, key);
                    return value;
                }
            }
            else
            {
                // upset disabled
                ESP_LOGV(TAG, "%s: %s not found and upset is disabled, using default value", name, key);
                nvs.end();
                return defaultValue;
            }
        }
        else
        {
            nvs.end();
            ESP_LOGD(TAG, "Setting found for %s %s", name, key);
            return value;
        }
    }
    else
    {
        ESP_LOGE(TAG, "Opening.. the NVS namespace: %s", name);
        return defaultValue;
    }
}

bool nvs_get_bool(const char *name, const char *key, bool defaultValue, bool upset)
{
    const char *TAG = "NVS";

    Preferences nvs;

    if (nvs.begin(name, false))
    {
        ESP_LOGV(TAG, "NVS namespace %s is open", name);

        bool value = nvs.getBool(key, NULL);

        if (value == NULL)
        {
            if (upset)
            {
                ESP_LOGD(TAG, "%s %s not found, trying to save it", name, key);
                size_t stored = nvs.putBool(key, defaultValue);
                value = nvs.getBool(key, NULL);
                nvs.end();

                if (value == NULL)
                {
                    // error saving
                    ESP_LOGE(TAG, "Saving.. returning default value for %s %s", name, key);
                    return defaultValue;
                }
                else
                {
                    // ok saving
                    ESP_LOGD(TAG, "Upset ok for %s %s", name, key);
                    return value;
                }
            }
            else
            {
                // upset disabled
                ESP_LOGV(TAG, "%s: %s not found and upset is disabled, using default value", name, key);
                nvs.end();
                return defaultValue;
            }
        }
        else
        {
            nvs.end();
            ESP_LOGD(TAG, "Setting found for %s %s", name, key);
            return value;
        }
    }
    else
    {
        ESP_LOGE(TAG, "Opening.. the nvs namespace %s", name);
        return defaultValue;
    }
}