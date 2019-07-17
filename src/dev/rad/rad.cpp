/**
 * @file rad.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "rad.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "dev/nvs/nvs.h"
#include "dev/core/general_utils.h"

bool radioreceiving;
bool radioCAD;
std::string radiorxvalue;

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t radInit()
{
    const char *TAG = "RAD";
    // only in dev stage, clear nvs before start:
    // nvsClear(TAG);

    bool rad_enabled = nvsGetBool(TAG, "enabled", RAD_ENABLED, true);

    if (true)
    {
        return ESP_OK;
    }
    else
    {
        return ESP_FAIL;
    }
}
