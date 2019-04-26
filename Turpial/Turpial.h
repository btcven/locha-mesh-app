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

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/**
 * @brief 
 * 
 */
TaskHandle_t peerHandler;

#endif // TURPIAL_H
