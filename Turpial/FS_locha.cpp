/**
 * @file FS_locha.cpp
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
#include "FS_locha.h"
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>

esp_err_t open_fs()
{
    const char *TAG = "FileSystem";
    // FS init
    if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED))
    {

        ESP_LOGE(TAG, "Error Mounting file system");
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGE(TAG, "Mounting file system");
        return ESP_OK;
    }
}