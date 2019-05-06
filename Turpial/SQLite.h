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
#include "FS_locha.h"
#include <SPIFFS.h>
#include "general_utils.h"

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t SQLite_INIT();
bool ejecutar(char *query, sqlite3 *db);
char *buscar(char *query, sqlite3 *db);
int buscar_valor(char *query, sqlite3 *db);

#endif // SQLITE_H
