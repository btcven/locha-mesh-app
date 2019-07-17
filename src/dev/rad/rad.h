/**
 * @file rad.h
 * @author Roberto De Sanctis for Locha.io
 * @brief Radio Wrapper Class
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef RAD_H_
#define RAD_H_

#include <Arduino.h>
#include <Preferences.h>

#if RAD_ENABLED
#ifdef RAD_TYPE
#if (RAD_TYPE == RFM95)
#include <LoRaLib.h>
#include "rad_LoRaLib.h"
#endif
#if (RAD_TYPE == E32)
#include <RH_Serial.h>
#endif
#endif
#endif



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
 void onReceive(int packetSize) {

/**
 * @brief Send data throw Radio
 * 
 */
bool radioSend(std::string data_to_send);

#endif // RAD_H_