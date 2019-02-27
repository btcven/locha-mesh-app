/**
 * Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
 * Ported to Arduino ESP32 by Evandro Copercini
 */
/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
 
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;

std::string txValue;
std::string rxValue;

bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // Service UUID
#define CHARACTERISTIC_UUID_RX  "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // Characteristic RX
#define CHARACTERISTIC_UUID_TX  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // Characteristic TX

class ServerCB : public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      deviceConnected = true;
    }

    void onDisconnect(BLEServer *pServer) {
      deviceConnected = false;
    }
};

class characteristicCB : public BLECharacteristicCallbacks {
  
    void onWrite(BLECharacteristic *pCharacteristic) {
      
      rxValue = pCharacteristic->getValue();

      if (rxValue.size() > 0) {
        Serial.println(rxValue.c_str());
        // incoming msg via ble iface:
        // 1.- send via rad iface handler
        // 2.- clear the var.
        rxValue.clear();
      }
    }
};

void bluetooth_task(void *params) {

  // device name must be: mac_address + ".locha.io"
  // not yet implemented.
  BLEDevice::init("mesh.locha.io");
  // Create the BLE Server and assign it a callback
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCB());
  // Can be included in the ServerCallback)?
  BLEStatus.isActive = true;
  
  // Set up the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);
  // And assign it BLE Characteristics
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new characteristicCB());
  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.printf("Start advertising\n");

  while (1) {
    if (deviceConnected) {
      if (txValue.size() > 0) {
        pTxCharacteristic->setValue(txValue);
        pTxCharacteristic->notify();
        txValue.clear();
      }
      delay(10);
    }
    else {
      if (txValue.size() > 0) {
        Serial.printf("Device not connected to BLE interface.\n");
        Serial.printf("Received: ");
        Serial.println(txValue.c_str());
        txValue.clear();
      }
      // disconnecting
      if (!deviceConnected && oldDeviceConnected) {
        // Give the Bluetooth interface a chance to get ready
        delay(500);
        // And restart advertising
        pServer->startAdvertising();
        Serial.printf("Restart advertising .. \n");
        oldDeviceConnected = deviceConnected;
      }
      // connecting
      if (deviceConnected && !oldDeviceConnected) {
        //
        oldDeviceConnected = deviceConnected;
      }
    }
  }
}
