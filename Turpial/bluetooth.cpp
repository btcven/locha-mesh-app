/**
 * Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
 * Ported to Arduino ESP32 by Evandro Copercini
 */
/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
//#include <cstdio>
//#include <cstdlib>
//#include <string>
#include <Arduino.h>
#include <lib/ESP32_BLE_Arduino/src/BLEDevice.h>
#include <lib/ESP32_BLE_Arduino/src/BLEServer.h>
#include <lib/ESP32_BLE_Arduino/src/BLEUtils.h>
#include <lib/ESP32_BLE_Arduino/src/BLE2902.h>
#include "defaultStartup.h"

extern BLE_status_t BLEStatus;

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;

String txValue;
String rxValue;

bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"           // Service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // Characteristic RX
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // Characteristic TX

class ServerCB : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
    BLEStatus.isConnected = true;
  }

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
    BLEStatus.isConnected = false;
  }
};

class characteristicCB : public BLECharacteristicCallbacks
{

  void onWrite(BLECharacteristic *pCharacteristic)
  {

    rxValue = pCharacteristic->getValue();

    //  if (rxValue.size() > 0) {
    if (rxValue.length() > 0)
    {

      Serial.println(rxValue.c_str());
      // incoming msg via ble iface:
      // 1.- send via rad iface handler
      // 2.- clear the var.
      //rxValue.clear();
      rxValue = "";
    }
  }
};

void bluetooth_task(void *params)
{

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
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new characteristicCB());
  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.printf("Start advertising\n");

  while (1)
  {
    if (deviceConnected)
    {
      //    if (txValue.size() > 0) {
      if (txValue.length() > 0)
      {
        pTxCharacteristic->setValue(txValue);
        pTxCharacteristic->notify();
        //  txValue.clear();
        txValue = "";
      }
      delay(10);
    }
    else
    {
      // if (txValue.size() > 0) {
      if (txValue.length() > 0)
      {
        Serial.printf("Device not connected to BLE interface.\n");
        Serial.printf("Received: ");
        Serial.println(txValue);
        //txValue.clear();
        txValue = "";
      }
      // disconnecting
      if (!deviceConnected && oldDeviceConnected)
      {
        // Give the Bluetooth interface a chance to get ready
        delay(500);
        // And restart advertising
        pServer->startAdvertising();
        Serial.printf("Restart advertising .. \n");
        oldDeviceConnected = deviceConnected;
      }
      // connecting
      if (deviceConnected && !oldDeviceConnected)
      {
        //
        oldDeviceConnected = deviceConnected;
      }
    }
  }
}
