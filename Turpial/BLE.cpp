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
#include "NVS.h"
#include "general_utils.h"
#include "hal/hardware.h"

BLEServer *ble_server = NULL;
BLECharacteristic *tx_uart;
BLECharacteristic *rx_uart;

// uart interface.
std::string txValue_uart;
std::string rxValue_uart;

// server MTU
uint16_t server_mtu = 255;

std::string server_name = "ble.locha.io";

std::string SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
std::string CHARACTERISTIC_UUID_TX = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
std::string CHARACTERISTIC_UUID_RX = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";

bool deviceConnected = false;

class ServerCB : public BLEServerCallbacks
{
    void onConnect(BLEServer *ble_server)
    {
        deviceConnected = true;
        ESP_LOGD("BLE", "[BLE] Client connected %d", ble_server->getConnId());
    }
    void onDisconnect(BLEServer *ble_server)
    {
        deviceConnected = false;
        ESP_LOGD("BLE", "Client disconnected %d", ble_server->getConnId());
        delay(500);
        ble_server->startAdvertising();
    }
};

class characteristicCB : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        ESP_LOGD("BLE", "[BLE] event onWrite");
        rxValue_uart = pCharacteristic->getValue();
        if (rxValue_uart.size() > 0)
        {
            // incoming msg. via BLE (Mobile -> Turpial)
            ESP_LOGD("BLE", "[BLE] MSG: %s", rxValue_uart.c_str());
            rxValue_uart.clear();
        }
    }
};

/**
 * @brief 
 * 
 */
TaskHandle_t bleTaskHandler;

/**
 * @brief 
 * 
 * @param params 
 */
void BLE_task(void *params)
{
    BLEDevice::setMTU(server_mtu);
    BLEDevice::init(server_name);

    ble_server = BLEDevice::createServer();
    ble_server->setCallbacks(new ServerCB());

    BLEService *server_service = ble_server->createService(SERVICE_UUID);

    // tx_uart.
    tx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
    tx_uart->addDescriptor(new BLE2902());

    // rx_uart.
    rx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
    rx_uart->setCallbacks(new characteristicCB());

    // start advertising
    server_service->start();
    ble_server->getAdvertising()->start();

    for (;;)
    {
        if (deviceConnected)
        {
            if (txValue_uart.size() > 0)
            {
                // outcoming msg (Turpial -> mobile)
                ESP_LOGD("BLE", "BLE tx uart: %s", txValue_uart.c_str());
                tx_uart->setValue(txValue_uart);
                tx_uart->notify();
                txValue_uart.clear();
            }
        }
        else
        {
            // dispositivo no conectado a un cliente movil,
            // al ser temporal el uso de mensajeria via ble
            // aqui no hacemos nada.
        }
    }
}

esp_err_t BLE_INIT()
{
    const char *TAG = "BLE";
    // get value from nvs.
    // nvs_clear("BLE");
    bool BLE_enabled = nvs_get_bool("BLE", "enabled", BLE_ENABLED, true);

    if (BLE_enabled)
    {
        ESP_LOGD(TAG, "[BLE] enabled on boot, starting..");
        xTaskCreatePinnedToCore(BLE_task, "BLE_task", 6144, NULL, 4, &bleTaskHandler, 1);
        float temp1 = GetTaskHighWaterMarkPercent(bleTaskHandler, 6144);
  ESP_LOGD(TAG, "[BLE] calculating stack size");
printf(" %04.1f%%\r\n",temp1);
    }
    else
    {
        ESP_LOGD(TAG, "[BLE] Disabled on boot");
    }
    return ESP_OK;
}
