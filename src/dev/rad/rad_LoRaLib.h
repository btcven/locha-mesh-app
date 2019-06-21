/**
 * @file rad_LoRaLib.h
 * @brief 
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef RAD_LORALIB_H_
#define RAD_LORALIB_H_

#include <Arduino.h>

/**
 * @brief send a string throw Radio
 * 
 * @param data_to_send 
 * @return true 
 * @return false 
 */
bool radioSendLora(std::string data_to_send);


bool radioInitLora();

byte *onRadioReceiveLora(int packetSize);

#endif // RAD_LORALIB_H_