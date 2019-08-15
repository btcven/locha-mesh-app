/**
 * @file NVS.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
 */
#ifndef NVS_H
#define NVS_H

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t nvs_remove(void);

/**
 * @brief 
 * 
 * @param name 
 * @return esp_err_t 
 */
esp_err_t nvs_clear(const char *name);

/**
 * @brief 
 * 
 * @param name 
 * @param key 
 * @param defaultValue 
 * @param upset 
 * @return int32_t 
 */
int32_t nvs_get_int(const char *name, const char *key, int32_t defaultValue, bool upset);

/**
 * @brief get a string from NVS storage space
 * 
 * @param name 
 * @param key 
 * @param defaultValue 
 * @param upset 
 * @return char* 
 */
char *nvs_get_string(const char *name, char *key, char *defaultValue, bool upset);

/**
 * @brief 
 * 
 * @param name 
 * @param key 
 * @param defaultValue 
 * @param upset 
 * @return true 
 * @return false 
 */
bool nvs_get_bool(const char *name, const char *key, bool defaultValue, bool upset);

#endif // NVS_H
