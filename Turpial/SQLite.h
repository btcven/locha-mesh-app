/**
 * @file SQLite.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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

// exec a select query and return only first record/first field
// if select return more than 10000 records then return empty
/* it uses: int sqlite3_prepare_v2(
  sqlite3 *db,             Database handle 
  const char *zSql,        SQL statement, UTF-8 encoded 
  int nByte,               Maximum length of zSql in bytes. 
  sqlite3_stmt **ppStmt,   OUT: Statement handle 
  const char **pzTail      OUT: Pointer to unused portion of zSql 
);
*/
char *buscar(char *query, sqlite3 *db);

// exec a select query and return only first record/first field
// if select return more than 10000 records then return empty
/* it uses: int sqlite3_prepare_v2(
  sqlite3 *db,             Database handle 
  const char *zSql,        SQL statement, UTF-8 encoded 
  int nByte,               Maximum length of zSql in bytes. 
  sqlite3_stmt **ppStmt,   OUT: Statement handle 
  const char **pzTail      OUT: Pointer to unused portion of zSql 
);
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
