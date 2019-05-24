/**
 * @file tables.cpp
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
#include "hal/hardware.h"
#include "memory_def.h"
#include "packet.h"
#include "SQLite.h"
#include "tables.h"

extern char *id_node;

/**
 * @brief check if a node is blacklisted
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
uint8_t is_blacklisted(char id_node_eval[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  query.append("SELECT COUNT(*) from BLACKLISTED_NODES WHERE ID='");
  query.append(id_node_eval);
  query.append("'");
  int rpta = buscar_valor((char *)query.c_str(), db);
  if (rpta > 0)
  {
    return 1;
  }
  return 0;
}

/**
 * @brief check if a neigbour exists
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool is_neighbour(char id_node_eval[SIZE_IDNODE], sqlite3 *db)
{

  std::string query;
  query.append("SELECT COUNT(*) from NODES WHERE ID='");
  query.append(id_node_eval);
  query.append("'");

  int rpta = buscar_valor((char *)query.c_str(), db);
  if (rpta > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * @brief Create a blacklisted node
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool create_blacklisted_node(char id_node_eval[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  char *pChar = (char *)"";

  if (!(compare_char(id_node_eval, pChar)))
  {
    if ((is_blacklisted(id_node_eval, db) == 0))
    {
      query.append("INSERT INTO BLACKLISTED_NODES ( id ) VALUES ('");
      query.append(id_node_eval);
      query.append("')");

      bool rpta = ejecutar((char *)query.c_str(), db);
      return rpta;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

/**
 * @brief create a new neighbour
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool create_neighbour(char id_node_neig[SIZE_IDNODE], sqlite3 *db)
{
  /**
   * se verifica que el id_node no sea vacio
   * se verifica que no exista previamente
   * se verifica que no exista en blacklist
   * se agrega a la b/d
   * 
   */

  std::string query;
  char *pChar = (char *)"";

  if (!(compare_char(id_node_neig, pChar)))
  {
    if ((is_blacklisted(id_node_neig, db) == 0))
    {
      if (!(is_neighbour(id_node_neig, db)))
      {
        query.append("INSERT INTO NODES ( id, date_created ) VALUES ('");
        query.append(id_node_neig);
        query.append("',date('now'))");
        bool rpta = ejecutar((char *)query.c_str(), db);

        return rpta;
      }
      else
      {
        return false;
      }
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

/**
 * @brief delete a neighbour
 * 
 * @param id_node 
 * @param db 
 * @return true 
 * @return false 
 */
bool delete_neighbour(char id_node_neig[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  query.append("DELETE FROM NODES where id='");
  query.append(id_node_neig);
  query.append("'");
  bool rpta = ejecutar((char *)query.c_str(), db);
  return rpta;
}

/**
 * @brief create a new route
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
bool create_route(char id_source[SIZE_IDNODE], char id_next_neighbour[SIZE_IDNODE], char id_destination[SIZE_IDNODE], uint8_t hops, int RSSI_recibido, int SNR_recibido, sqlite3 *db)
{
  std::string query;
  query.append("INSERT INTO ROUTES (id_origen,id_destino,id_next_neighbour,age,hops,RSSI_packet,SNR_packet,date_last_viewed,date_created) VALUES('");
  query.append(id_source);
  query.append("','");
  query.append(id_destination);
  query.append("','");
  query.append(id_next_neighbour);
  query.append("',");
  query.append(ToString(millis()));
  query.append(",");
  query.append(ToString(hops));
  query.append(",");
  query.append(ToString(RSSI_recibido));
  query.append(",");
  query.append(ToString(SNR_recibido));
  query.append(",date('now'),date('now'))");

  bool rpta = ejecutar((char *)query.c_str(), db);
  return rpta;
}

/**
 * @brief delete a route
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool delete_route(char id_source[SIZE_IDNODE], char id_destination[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  query.append("DELETE FROM ROUTES WHERE id_origen='");
  query.append(id_source);
  query.append("' AND id_destino='");
  query.append(id_destination);
  query.append("'");

  bool rpta = ejecutar((char *)query.c_str(), db);
  return rpta;
}

/**
 * @brief  check if a route exists
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool exist_route(char id_source[SIZE_IDNODE], char id_destination[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  query.append("SELECT COUNT(*) from ROUTES WHERE id_origen ='");
  query.append(id_source);
  query.append("' and id_destino='");
  query.append(id_destination);
  query.append("'");
  int rpta = buscar_valor((char *)query.c_str(), db);
  if (rpta > 0)
  {
    return true;
  }
  else
  {
    // se verifica la ruta invertida
    query.clear();
    query.append("SELECT COUNT(*) from ROUTES WHERE id_origen ='");
    query.append(id_destination);
    query.append("' and id_destino='");
    query.append(id_source);
    query.append("'");
    rpta = buscar_valor((char *)query.c_str(), db);
    if (rpta > 0)
    {
      return true;
    }
    return false;
  }
}

/**
 * @brief check if route is blacklisted
 * 
 * @param id_source 
 * @param id_destination 
 * @param db 
 * @return true 
 * @return false 
 */
bool is_blacklisted_route(char id_source[SIZE_IDNODE], char id_destination[SIZE_IDNODE], sqlite3 *db)
{
  std::string query;
  query.append("SELECT COUNT(*) from BLACKLISTED_ROUTES WHERE id_origen='");
  query.append(id_source);
  query.append("' and id_destino='");
  query.append(id_destination);
  query.append("'");

  int rpta = buscar_valor((char *)query.c_str(), db);
  if (rpta > 0)
  {
    return true;
  }
  else
  {
    // se verifica la ruta inversa
    query.clear();
    query.append("SELECT COUNT(*) from BLACKLISTED_ROUTES WHERE id_origen='");
    query.append(id_destination);
    query.append("' and id_destino='");
    query.append(id_source);
    query.append("'");
    rpta = buscar_valor((char *)query.c_str(), db);
    return rpta;
  }
}
