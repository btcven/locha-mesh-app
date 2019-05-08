/**
 * @file tables.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef TABLES_H 
#define TABLES_H

#include <Arduino.h>
#include "hal/hardware.h"
#include "memory_def.h"
#include "packet.h"
#include "SQLite.h"

uint8_t is_blacklisted(char id_node[SIZE_IDNODE], sqlite3 *db);
bool create_blacklisted_node(char id_node[SIZE_IDNODE], sqlite3 *db);
bool is_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
bool create_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
bool delete_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
bool create_route(char id_source[SIZE_IDNODE], char id_next_neighbour[SIZE_IDNODE],char id_destination[SIZE_IDNODE],uint8_t hops, int RSSI_recibido, int SNR_recibido, sqlite3 *db);
bool delete_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);
bool exist_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);
bool is_blacklisted_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);

#endif // TABLES_H
