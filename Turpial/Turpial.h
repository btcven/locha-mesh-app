/**
 * @file Turpial.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef TURPIAL_H
#define TURPIAL_H

#include <Arduino.h>

/**
 * @brief Handler for network task
 * 
 */
TaskHandle_t peerHandler;

/**
 * @brief Handler for Auto Hello
 * 
 */
TaskHandle_t AUTO_HELLO_Handler;

#endif // TURPIAL_H
