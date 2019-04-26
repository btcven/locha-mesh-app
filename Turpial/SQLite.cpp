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

    int rc = sqlite3_open(filename, db);

    if (rc)
    {
        ESP_LOGD(TAG, "Can't open database: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    else
    {
        ESP_LOGD(TAG, "Opened database successfully %d\n", rc);
    }
    return rc;
}

int db_exec(sqlite3 *db, const char *sql)
{
    const char *TAG = "SQLLite";

    // int rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    int rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        ESP_LOGE(TAG, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        ESP_LOGE(TAG, "Operation done successfully\n");
    }

    return rc;
}

// check database file & inicialize database
// database file should not contains special characters neither /  , prefer to use 8+3 syntax name
bool sqlite3_startup(const char *filename, sqlite3 *db, bool create_on_startup)
{
//const char *full_path_filename = "/spiffs/config.db";
std::string only_file((char*)filename);
char* only_file_char;
char *pChar = (char*)"/";
      std::replace( only_file.begin(), only_file.end(), '/spiffs/', NULL);
      only_file_char=string2char(only_file);
  //  strcat(file_plus_dirchr, pChar);
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
        if (Fileexists(only_file_char))
        {
            ESP_LOGD(TAG, "Erasing %s", only_file_char);
            SPIFFS.remove(only_file_char);
        }
    }
    ESP_LOGD(TAG, "Goes to sqlite3_initialize()");

    sqlite3_initialize();

    ESP_LOGD(TAG, "Ready sqlite3_initialize()");

    /*
    char *full_path_filename = (char *)malloc(1 + strlen("/spiffs/") + strlen(filename));
    strcpy(full_path_filename, "/spiffs/");
    strcat(full_path_filename, filename);
    */
    

    ESP_LOGD(TAG, "Copy path ready");

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
 * @brief 
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
    return ESP_OK;
}

// exec any action query: insert, update or delete
// return true if exec OK, or false on error
bool ejecutar(char *query, sqlite3 *db)
{
    int rc;
    rc = db_exec(db, query);
    if (rc != SQLITE_OK)
    {
        return true;
    }
    else
    {
        return false;
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