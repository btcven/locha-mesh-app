/**
 * @file hardware.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#if defined(ARDUINO_HELTEC_WIFI_LORA_32_V1)
    #include "heltec_v1.h"
#elif defined(ARDUINO_HELTEC_WIFI_LORA_32_V2)
    #include "heltec_v2.h"
#endif

#endif // HARDWARE_H_