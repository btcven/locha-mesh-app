/**
 * @file RAD.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef RAD_H
#define RAD_H
#include <Arduino.h>
#include "memory_def.h"

/**
 * @brief Start Radio interface
 * 
 * @return esp_err_t 
 */
esp_err_t RAD_INIT();

/**
 * @brief send a string throw Radio
 * 
 * @param data_to_send 
 * @return true 
 * @return false 
 */
bool radioSend(uint8_t *data_to_send, size_t size_of_data);

#endif // RAD_H
