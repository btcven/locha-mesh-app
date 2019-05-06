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

/**
 * @brief SQLite can format the file system if corrupted 
 */
#define FORMAT_SPIFFS_IF_FAILED true

/**
 * @brief Initialize the file system
 * 
 * @return esp_err_t 
 */
esp_err_t open_fs();

#endif // FS_LOCHA_H