/**
 * @file NVS.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef NVS_H_
#define NVS_H_

bool nvsGetBool(const char *name, const char *key, bool defaultValue, bool upset);
char *nvsGetString(const char *name, char *key, char *defaultValue, bool upset);
int32_t nvsGetInt(const char *name, const char *key, int32_t defaultValue, bool upset);
esp_err_t nvsClear(const char *name);

#endif // NVS_H_