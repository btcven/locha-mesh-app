/**
 * @file rad.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1.2
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
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
