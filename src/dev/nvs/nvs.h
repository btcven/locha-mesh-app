/**
 * @file NVS.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef NVS_H_
#define NVS_H_

bool nvsGetBool(const char *name, const char *key, bool defaultValue, bool upset);
char *nvsGetString(const char *name, char *key, char *defaultValue, bool upset);
int32_t nvsGetInt(const char *name, const char *key, int32_t defaultValue, bool upset);
esp_err_t nvsClear(const char *name);

#endif // NVS_H_