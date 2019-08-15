/**
 * @file BLE.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 *
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
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

std::string UUID_UART = "";
std::string TX_UART = "";
std::string RX_UART = "";

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