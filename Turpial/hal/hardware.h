/**
 * @file hardware.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 */
 
#ifndef HARDWARE_H
#define HARDWARE_H

// #define HELTEC_V1
#define HELTEC_V2

// Serial Baudrate
#define BAUDRATE 115200


#if defined(HELTEC_V1)
    #include "heltec_V1.h"
#elif defined(HELTEC_V2)
    #include "heltec_V2.h"
#endif


#endif // HARDWARE_H
