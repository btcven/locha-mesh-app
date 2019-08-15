/**
 * @file BLE.cpp
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
 */
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BLE.h"

BLEServer *ble_server = NULL;
BLECharacteristic *tx_uart;
BLECharacteristic *rx_uart;
bool deviceConnected = false;

class ServerCB : public BLEServerCallbacks
{
    void onConnect(BLEServer *ble_server)
    {
        deviceConnected = true;
        ESP_LOGD("BLE", "Client connected");
    }
    void onDisconnect(BLEServer *ble_server)
    {
        deviceConnected = false;
        ESP_LOGD("BLE", "Client disconnected");
        delay(100); // was 500
        ble_server->startAdvertising();
    }
};

class characteristicCB : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        // determinar *pCharacteristic
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
        // determinar *pCharacteristic
    }
};

void BLE_task(void *params)
{

    BLEDevice::setMTU(server_mtu);
    BLEDevice::init(server_name);

    ble_server = BLEDevice::createServer();
    ble_server->setCallbacks(new ServerCB());

    BLEService *server_service = ble_server->createService(UUID_UART);

    while (1)
    {
        if (deviceConnected)
        {
            if(txValue_uart.size() > 0) {
                // procesar tx_uart
            }
        }
        else
        {
            if(txValue_uart.size() > 0) {
                // aqui no ponemos nada, pues es temporal el uso de ble.
            }
        }
    }
}

esp_err_t BLE_INIT() {
    // iniciar servidor ble.
    

}