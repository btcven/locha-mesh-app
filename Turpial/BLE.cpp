/**
 * @file BLE.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
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
BLECharacteristic *rx_batt;
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

/**
 * @brief 
 * 
 */
class characteristicCB : public BLECharacteristicCallbacks
{
    /**
     * @brief 
     * 
     * @param pCharacteristic 
     */
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        // determinar *pCharacteristic
        // mobile app -> Turpial

        BLEUUID descriptor = pCharacteristic->getUUID();
        // if rx uart.
        if (descriptor.equals == rx_uart)
        {
            ESP_LOGD("BLE", "BLE (rx_uart)");
        }
        // if rx batt.
        if (descriptor.equals == rx_batt)
        {
            ESP_LOGD("BLE", "BLE (rx_batt)");
        }
    }
    /**
     * @brief 
     * 
     * @param pCharacteristic 
     */
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

    BLEService *server_service = ble_server->createService(SERVICE_UUID);

    while (1)
    {
        if (deviceConnected)
        {
            if (txValue_uart.size() > 0)
            {
                // procesar tx_uart
            }
        }
        else
        {
            if (txValue_uart.size() > 0)
            {
                // aqui no ponemos nada, pues es temporal el uso de ble.
            }
        }
    }
}

esp_err_t BLE_INIT()
{
    // iniciar servidor ble.
}