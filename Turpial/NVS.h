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
 * @param name 
 * @param key 
 * @param defaultValue 
 * @return true 
 * @return false 
 */
bool nvs_get_bool(const char *name, const char *key, bool defaultValue);

#endif // NVS_H