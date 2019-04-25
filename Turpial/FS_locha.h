/**
 * @file FS_locha.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef FS_LOCHA_H 
#define FS_LOCHA_H

#include <Arduino.h>

/* SQLite is on charge of format in case of fail */
#define FORMAT_SPIFFS_IF_FAILED true

esp_err_t open_fs();

#endif // FS_LOCHA_H