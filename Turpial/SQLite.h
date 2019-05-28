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
 * @brief Start database
 * 
 * @return esp_err_t 
 */
esp_err_t SQLite_INIT();

/**
 * @brief exec any action query: insert, update or delete
 * return true if exec OK, or false on error
 * @param query 
 * @param db 
 * @return true 
 * @return false 
 */
bool ejecutar(char *query, sqlite3 *db);

/**
 * @brief 
 * 
 * @param query 
 * @param db 
 * @return char* 
 */
char *buscar(char *query, sqlite3 *db);

/**
 * @brief 
 * 
 * @param query 
 * @param db 
 * @return int 
 */
int buscar_valor(char *query, sqlite3 *db);

/**
 * @brief Send a packet to database
 * 
 * @param packet_received 
 * @return true 
 * @return false 
 */
bool packet_send(packet_t packet_received);

#endif // SQLITE_H
