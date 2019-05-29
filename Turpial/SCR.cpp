/**
 * @file SCR.cpp
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
#include "SSD1306.h"
#include "SCR.h"
#include "NVS.h"
#include "hal/hardware.h"

/**
 * @brief Create an instance of SSD1306
 * @return SSD1306 
 */
SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);

esp_err_t SCR_INIT()
{
    const char *TAG = "SCR";

    // only in dev stage:
    // clear nvs before start.
    //
    // nvs_clear("SCR");

    bool SCR_enabled = nvs_get_bool(TAG, "enabled", SCR_ENABLED, false);

    if (SCR_enabled)
    {
        if (Vext)
        {
            pinMode(Vext, OUTPUT);
            digitalWrite(Vext, LOW);
            delay(100);
        }

        if (!display.init())
        {
            ESP_LOGE(TAG, "[%s] Error starting", TAG);
            return ESP_FAIL;
        }
        else
        {
            display.setBrightness(5);
            display.flipScreenVertically();
            display.drawString(0, 0, "Starting...");
            display.display();
            ESP_LOGD(TAG, "%s OK", TAG);
            return ESP_OK;
        }
    }
    else
    {
        ESP_LOGD(TAG, "%s disabled on boot", TAG);
        return ESP_OK;
    }
}
