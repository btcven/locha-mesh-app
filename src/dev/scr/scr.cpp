/**
 * @file SCR.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.2.0
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#include <Arduino.h>
#include "scr.h"
#include "SSD1306.h"
#include "dev/nvs/nvs.h"
#include "hal/hardware.h"
#include <Wire.h>
#include "dev/esc/esc.h"

/**
 * @brief Create an instance of SSD1306
 * @return SSD1306 
 */
SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);

esp_err_t scrInit()
{
    const char *tag = "SCR";
    // only in dev stage:
    // clear nvs before start.
    //
    // nvs_clear("SCR");

    bool SCR_enabled = nvsGetBool(tag, "enabled", SCR_ENABLED, true);

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
            return ESP_FAIL;
        }
        else
        {
            display.setBrightness(5);
            display.flipScreenVertically();
            display.setFont(ArialMT_Plain_10);
            display.setTextAlignment(TEXT_ALIGN_LEFT);
            display.drawString(0, 0, "Starting...");
            display.display();
            return ESP_OK;
        }
    }
    else
    {
        return ESP_OK;
    }
}
