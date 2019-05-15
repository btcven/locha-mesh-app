/**
 * @file BLE.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
#ifndef BLE_H
#define BLE_H
#include <Arduino.h>
#include <string>

std::string txValue_uart;
std::string rxValue_uart;

uint16_t server_mtu = 255;

std::string server_name = "subid.locha.io";

std::string SERVICE_UUID = "";
std::string CHARACTERISTIC_UUID_TX = "";
std::string CHARACTERISTIC_UUID_RX = "";

/**
 * @brief 
 * 
 * @param params 
 */
void BLE_task(void *params);

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t BLE_INIT();

#endif // BLE_H