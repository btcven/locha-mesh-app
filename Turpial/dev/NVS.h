/**
 * @file NVS.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
#ifndef NVS_H
#define NVS_H

bool nvs_get_bool(const char *name, const char *key, bool defaultValue);

#endif // NVS_H