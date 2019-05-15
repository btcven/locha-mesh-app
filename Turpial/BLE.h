/**
 * @file BLE.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
#ifndef BLE_H
#define BLE_H
#include <Arduino.h>

/**
 * @brief 
 * 
 * @param params 
 */
void BLE_task(void *params);

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t BLE_INIT();

#endif // BLE_H