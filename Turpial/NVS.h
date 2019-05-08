/**
 * @file NVS.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
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
char *nvs_get_string(char *name, char *key, char *defaultValue, bool upset);

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
