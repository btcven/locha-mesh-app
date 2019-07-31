/**
 * @file rad.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef RAD_H_
#define RAD_H_

#include <Arduino.h>
#include <Preferences.h>




/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t radInit();

/**
 * @brief Processing incoming packets
 * 
 */
 void onReceive(int packetSize);

/**
 * @brief Send data throw Radio
 * 
 */
bool radioSend(std::string data_to_send);

#endif // RAD_H_