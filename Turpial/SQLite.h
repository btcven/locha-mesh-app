/**
 * @file SQLite.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 */

#ifndef SQLITE_H
#define SQLITE_H

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include "general_utils.h"

/* SQLite is on charge of format in case of fail */
#define FORMAT_SPIFFS_IF_FAILED true

esp_err_t SQLite_INIT();


#endif // SQLITE_H
