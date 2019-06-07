/**
 * @file SCR.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <Arduino.h>
#include "SCR.h"
#include "SSD1306.h"
#include "NVS.h"
#include "hal/hardware.h"
#include <Wire.h>
#include "ESC_control.h"
extern system_status_t system_status;

/**
 * @brief Create an instance of SSD1306
 * @return SSD1306 
 */
SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);

esp_err_t SCR_test()
{
    const char *tag = "SCR_test";
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
            ESP_LOGE(tag, "[%s] Error starting", tag);
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
            ESP_LOGD(tag, "%s OK", tag);
            return ESP_OK;
        }
    }
    else
    {
        ESP_LOGD(tag, "%s disabled on boot", tag);
        return ESP_OK;
    }
}

void scrTask(void *params)
{
    display.setBrightness(5);
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_LEFT);

    for (;;)
    {

        display.clear();
        display.drawString(0, 0, String(system_status.bat_lvl, 0));
        display.drawString(40, 0, "mV");
        display.drawString(0, 20, String(system_status.bat_lvl_percent, 1));
        display.drawString(36, 20, "%");
        display.display();
        delay(200);
    }
}
