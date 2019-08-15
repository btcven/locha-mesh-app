/**
 * @file tables.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha  Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
  */
#ifndef TABLES_H 
#define TABLES_H

#include <Arduino.h>
#include "SQLite.h"

/**
 * @brief 
 * 
 * @param id_node 
 * @param db 
 * @return uint8_t 
 */
uint8_t is_blacklisted(char id_node[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief Create a blacklisted node object
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool create_blacklisted_node(char id_node[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief 
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool is_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief Create a neighbour object
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool create_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief 
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool delete_neighbour(char id_node[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief Create a route object
 * 
 * @param id_source 
 * @param id_next_neighbour 
 * @param id_destination 
 * @param hops 
 * @param RSSI_recibido 
 * @param SNR_recibido 
 * @param db 
 * @return true 
 * @return false 
 */
bool create_route(char id_source[SIZE_IDNODE], char id_next_neighbour[SIZE_IDNODE],char id_destination[SIZE_IDNODE],uint8_t hops, int RSSI_recibido, int SNR_recibido, sqlite3 *db);
/**
 * @brief 
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool delete_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief 
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool exist_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);
/**
 * @brief 
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool is_blacklisted_route(char id_source[SIZE_IDNODE],char id_destination[SIZE_IDNODE], sqlite3 *db);

#endif // TABLES_H
