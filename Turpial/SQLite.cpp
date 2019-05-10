/**
 * @file SQLite.cpp
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
#include "SQLite.h"

const char *TAG = "SQLite";

// database declarations
sqlite3 *config_db; // config database for non volatile data
sqlite3 *data_db;   // data database for volatile data

char *zErrMsg = 0;

const char *data = "Callback function";

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    Serial.printf("%s: ", (const char *)data);
    for (i = 0; i < argc; i++)
    {
        Serial.printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    Serial.printf("\n");
    return 0;
}


/**
 * @brief open filename database , return SQLite open status
 * 
 * @param filename 
 * @param pointer to db
 * @return int  SQLite open status
 */
int db_open(const char *filename, sqlite3 **db)
{
const char *TAG = "SQLLite";
    int rc = sqlite3_open(filename, db);

    if (rc)
    {
        ESP_LOGD(TAG, "Can't open database: %s", sqlite3_errmsg(*db));
        return rc;
    }
    else
    {
        ESP_LOGD(TAG, "Opened database successfully %d", rc);
    }
    return rc;
}

/**
 * @brief Exec a sql query against given database
 * 
 * @param db 
 * @param sql 
 * @return int 
 */
int db_exec(sqlite3 *db, const char *sql)
{
    const char *TAG = "SQLLite";
    ESP_LOGE(TAG, "Voy a db_exec");

    int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
  //  int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    
    if (rc != SQLITE_OK)
    {
        ESP_LOGE(TAG, "SQL error: %s", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        ESP_LOGE(TAG, "Operation done successfully");
    }

    return rc;
}


/**
 * @brief exec any action query: insert, update or delete
 * return true if exec OK, or false on error
 * @param query 
 * @param db 
 * @return true 
 * @return false 
 */
bool ejecutar(char *query, sqlite3 *db)
{
  const char *TAG = "SQLLite";
    int rc;
    
    ESP_LOGE(TAG, "Voy a ejecutar");
    rc = db_exec(db, query);
    if (rc != SQLITE_OK)
    {
      ESP_LOGE(TAG, "No se ejecuto");
        return false;
    }
    else
    {
      ESP_LOGE(TAG, "Ejecute correctamente");
        return true;
      
    }
}




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
char *buscar(char *query, sqlite3 *db)
{

    char *resp;
    sqlite3_stmt *res;
    const char *tail;
    int rc;
    int rec_count = 0;
    char *pChar = (char *)"";
    const char *TAG = "SQLLite";

    rc = sqlite3_prepare_v2(db, query, 10000, &res, &tail);
    if (rc != SQLITE_OK)
    {
        ESP_LOGE(TAG, "Failed to fetch data: %s", sqlite3_errmsg(db));
    }
    while (sqlite3_step(res) == SQLITE_ROW)
    {
        resp = (char *)sqlite3_column_text(res, 1); // el 1 es el numero de la columna (campo) en el query
        if (rec_count > 5000)
        {
            ESP_LOGE(TAG, "Please select different range, too many records: %s", rec_count);
            sqlite3_finalize(res);
            return pChar;
        }
    }
    sqlite3_finalize(res);
    return resp;
}

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
int buscar_valor(const char *query, sqlite3 *db)
{

    int resp;
  sqlite3_stmt *res;
    int rec_count = 0;
    const char *tail;
    int rc;
    
    const char *TAG = "SQLLite";
  //  char *pChar = (char *)"";
 //if (db_open("/spiffs/data.db", db)){
 // ESP_LOGE(TAG, "No se pudo abrir la base de datos");
 //   return 0;
 // }
  
 
   rc = sqlite3_prepare_v2(db, query, -1, &res, &tail);
    
   if (rc != SQLITE_OK) {
        ESP_LOGE(TAG, "Failed to fetch data:%s",sqlite3_errmsg(db));
        return 0;   // TODO modificar para cuando se reciba un error devolver NULL y no un campo con cero
    }

while (sqlite3_step(res) == SQLITE_ROW) {
        resp=sqlite3_column_int(res, 0);
        ESP_LOGE(TAG, "dato obtenido: %d", resp);
        rec_count++;
    }
    ESP_LOGE(TAG, "No. of records: %d", rec_count);
    sqlite3_finalize(res);
    return resp;
}



/**
 * @brief check database file & inicialize database
 * check database file & inicialize database
 * @param filename 
 * @param create_on_startup 
 * @return true 
 * @return false 
 */

bool sqlite3_startup(const char *filename, sqlite3 *(&db), bool create_on_startup)
{
std::string only_file((char*)filename);

char* only_file_char;
char *pChar = (char*)"/";
const char *TAG = "SQLLite";
    
      eraseSubStr(only_file,"/spiffs");
      only_file_char=string2char(only_file);
  
    File root = SPIFFS.open(pChar);
    

    ESP_LOGD(TAG, "Inside sqlite3_startup");

    if (!root)
    {
        ESP_LOGE(TAG, "- failed to open directory", TAG);
        return false;
    }
    if (!root.isDirectory())
    {
        ESP_LOGE(TAG, " / is not a directory", TAG);
        return false;
    }
    
    if (create_on_startup)
    {
      ESP_LOGD(TAG, "go to delete older database ");
      ESP_LOGD(TAG, "nombre del archivo %s",only_file_char);
        if (Fileexists(only_file_char))
        
        {
            ESP_LOGD(TAG, "Erasing %s", only_file_char);
            SPIFFS.remove(only_file_char);
        }
    }
  //  ESP_LOGD(TAG, "Goes to sqlite3_initialize()");

    sqlite3_initialize();

    ESP_LOGD(TAG, "Ready sqlite3_initialize()");

    /*
    char *full_path_filename = (char *)malloc(1 + strlen("/spiffs/") + strlen(filename));
    strcpy(full_path_filename, "/spiffs/");
    strcat(full_path_filename, filename);
    */
    

//    ESP_LOGD(TAG, "Copy path ready");

    if (!db_open(filename, &db))
    {
         ESP_LOGE(TAG, "Opened %s", filename);
        return true;
    }
    else
    {
        ESP_LOGE(TAG, "- failed to open database %s", filename);
        return false;
    }
}


/**
 * @brief Start database
 * 
 * @return esp_err_t 
 */
esp_err_t SQLite_INIT()
{
    esp_err_t openFS = open_fs();

    if (openFS == ESP_FAIL)
    {
        return ESP_FAIL;
    }

    bool response = false;

    response = sqlite3_startup("/spiffs/config.db", config_db, false);

    if (!response)
    {
        ESP_LOGE(TAG, "Can't open config database");
        return ESP_FAIL;
    }
    response = sqlite3_startup("/spiffs/data.db", data_db, true);

    if (!response)
    {
        ESP_LOGE(TAG, "Can't create data database");
        return ESP_FAIL;
    }

    // create tables if doesnt exists
 //   ESP_LOGE(TAG, "open again Tables SQLLite");
 // if (db_open("/spiffs/data.db", &data_db)){
 //   return ESP_FAIL;
 // }
    ESP_LOGE(TAG, "SQLite: creating tables");

    // si se usa WITHOUT ROWID oligatoriamente tiene que tener PRIMARY KEY
    int rc=ejecutar("CREATE TABLE IF NOT EXISTS NODES ( id TEXT PRIMARY KEY,DATE_LAST_VIEWED REAL NULL, DATE_CREATED REAL NOT NULL) WITHOUT ROWID;", data_db);
    
    // el siguiente codigo es solo para probar que efectivamente se haga un insert y un select
    // insert dummy para probar que este correctamente creada la tabla
    //rc=ejecutar("INSERT INTO NODES ( id , DATE_LAST_VIEWED , DATE_CREATED) VALUES('8',datetime('now'),datetime('now'));", data_db);
    // select dummy para buscar, en la tabla de NODES se usa el siguiente query
    //  String mysql="SELECT COUNT(*) FROM NODES";
    //  int rpta2=buscar_valor(mysql.c_str(),data_db);
    // ESP_LOGE(TAG, "Respuesta obtenida al buscar:%s",rpta2);
      
      ESP_LOGE(TAG, "Table NODES ready");
      rc=ejecutar("CREATE TABLE IF NOT EXISTS BLACKLISTED_NODES ( id TEXT PRIMARY KEY ) WITHOUT ROWID;", data_db);
      ESP_LOGE(TAG, "Table BLACKLISTED_NODES ready");
      // AUTOINCREMENT only works with INTEGER PRIMARY KEY and doesn't allow  WITHOUT ROWID
      rc=ejecutar("CREATE TABLE IF NOT EXISTS ROUTES (id_ruta INTEGER PRIMARY KEY AUTOINCREMENT,id_origen TEXT NOT NULL,id_destino TEXT NOT NULL,id_next_neighbour TEXT NULL,age INTEGER,hops INTEGER,RSSI_packet INTEGER,SNR_packet INTEGER,date_last_viewed REAL NULL,date_created REAL NOT NULL);", data_db);
      ESP_LOGE(TAG, "Table ROUTES ready");
      rc=ejecutar("CREATE TABLE IF NOT EXISTS BLACKLISTED_ROUTES(id_ruta_blacklisted INTEGER PRIMARY KEY AUTOINCREMENT,id_origen TEXT NOT NULL,id_destino TEXT NOT NULL);", data_db);
      ESP_LOGE(TAG, "Table BLACKLISTED_ROUTES ready");
      rc=ejecutar("CREATE TABLE IF NOT EXISTS PACKET(id_packet INTEGER PRIMARY KEY AUTOINCREMENT, status INTEGER NOT NULL, type_msg INTEGER NOT NULL, subtype_msg INTEGER NOT NULL, id_origen TEXT NOT NULL,id_destino TEXT NULL, id_next_neighbour TEXT NULL, checksum_data  TEXT NULL, timestamp REAL NULL, payload TEXT NULL, payload_length INTEGER NULL, packet_number INTEGER NULL, packet_total INTEGER NULL, not_delivered_type INTEGER NULL);", data_db);
      ESP_LOGE(TAG, "Table PACKET ready");
      rc=ejecutar("CREATE TABLE IF NOT EXISTS PACKET_STATUS(status INTEGER NOT NULL,desc TEXT NOT NULL);", data_db);
      // Fill table PACKET_STATUS
      rc=ejecutar("INSERT INTO PACKET_STATUS(status,desc) VALUES(0,'NOT SENDED');", data_db);
      ESP_LOGE(TAG, "Table PACKET_STATUS ready");
      // create packet_status
      rc=ejecutar("CREATE TABLE IF NOT EXISTS MESSAGE(id_msg INTEGER PRIMARY KEY AUTOINCREMENT,priority INTEGER,retries INTEGER,retry_timestamp REAL NULL);", data_db);
      ESP_LOGE(TAG, "Table MESSAGE ready");  
      rc=ejecutar("CREATE TABLE IF NOT EXISTS MESSAGE_PACKET(id_msg INTEGER NOT NULL,id_packet INTEGER NOT NULL);", data_db);
      ESP_LOGE(TAG, "Table MESSAGE_PACKET ready"); 
      ESP_LOGE(TAG, "Tablas SQLite listas.");
      return ESP_OK;
}


/**
 * 
 * Functions to send / get data from SQLite
 * 
 */


/**
 * @brief Send a packet to database
 * 
 * @param packet_received 
 * @return true 
 * @return false 
 */
bool packet_send(packet_t packet_received){
  
  std::string query_first_part="INSERT INTO PACKET ( status,type_msg , subtype_msg , id_origen,id_destino, id_next_neighbour, checksum_data, timestamp, payload, payload_length ";

  if (packet_received.header.packet_type==DATA){
     if ((packet_received.header.packet_sub.data_type==MSG)or(packet_received.header.packet_sub.data_type==TXN)or(packet_received.header.packet_sub.data_type==BINARY)){
          query_first_part.append(", packet_number, packet_total, not_delivered_type");
     }
  }
  // status=0 NOT SENDED
  query_first_part.append(") VALUES(0,");
  query_first_part.append(ToString(packet_received.header.packet_type));
  query_first_part.append(",");
  query_first_part.append(ToString(packet_received.header.packet_sub));
  query_first_part.append(",'");
  query_first_part.append(ToString(packet_received.header.from));
  query_first_part.append("','");
  query_first_part.append(ToString(packet_received.header.to));
  query_first_part.append("','");
  query_first_part.append(ToString(packet_received.header.next_neighbor));
  query_first_part.append("','");
  query_first_part.append(ToString(packet_received.header.checksum_data));
  query_first_part.append("',");
  query_first_part.append(ToString(packet_received.header.timestamp));
  query_first_part.append(",'");
  
  
  if (packet_received.header.packet_type==DATA){
    if ((packet_received.header.packet_sub.data_type==MSG)or(packet_received.header.packet_sub.data_type==TXN)or(packet_received.header.packet_sub.data_type==BINARY)){
      query_first_part.append(ToString(packet_received.body.body_data_splitted.payload));
      query_first_part.append("',");
      query_first_part.append(ToString(packet_received.body.body_data_splitted.payload_length));
      query_first_part.append(",");
      query_first_part.append(ToString(packet_received.body.body_data_splitted.packet_number));
      query_first_part.append(",");
      query_first_part.append(ToString(packet_received.body.body_data_splitted.packet_total));
      query_first_part.append(",");
      query_first_part.append(ToString(packet_received.body.body_data_splitted.not_delivered_type));
    } else {
      query_first_part.append(ToString(packet_received.body.body_data.payload));
      query_first_part.append("',");
      query_first_part.append(ToString(packet_received.body.body_data.payload_length));
    }
  } else {
      query_first_part.append(ToString(packet_received.body.body_data.payload));
      query_first_part.append("',");
      query_first_part.append(ToString(packet_received.body.body_data.payload_length));
  }
  query_first_part.append(");");
  bool rc=ejecutar((char *)query_first_part.c_str(), data_db);
  return rc;
}
