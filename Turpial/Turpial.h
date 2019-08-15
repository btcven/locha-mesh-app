/**
 * @file Turpial.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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
