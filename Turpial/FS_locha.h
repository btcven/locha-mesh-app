/**
 * @file FS_locha.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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