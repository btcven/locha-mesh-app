/**
 * @file RAD.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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
bool radioSend(std::string data_to_send);

#endif // RAD_H
