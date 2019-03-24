/**
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#include <Arduino.h>
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "bluetooth.h"
#include "radio.h"

BLEServer *ble_server = NULL;
BLECharacteristic *tx_uart;
BLECharacteristic *rx_uart;

std::string server_name = "mesh.locha.io";

std::string txValue;
std::string rxValue;

bool deviceConnected = false;

class ServerCB : public BLEServerCallbacks
{
    void onConnect(BLEServer *ble_server)
    {
      deviceConnected = true;
    }
    void onDisconnect(BLEServer *ble_server)
    {
      deviceConnected = false;
      Serial.println("[BLE] restart advertising...");
      delay(500);
      ble_server->startAdvertising();
    }
};

class characteristicCB : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
      // movil -> ble_server(Turpial)
      rxValue = pCharacteristic->getValue();
      // si tenemos datos podemos enviarlos via radio desde aqui.
      if (rxValue.size() > 0)
      {
        Serial.print("[BLE] Received: ");
        Serial.println(rxValue.c_str());
        radioSend(rxValue);
        rxValue.clear();

      }
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
      // hay que hacer algo cuando el flujo de datos es: ble_server(Turpial) -> movil
    }
};

void task_bluetooth(void *params)
{
  // asignamos el nombre al servidor
  // este será el que se mostrará en la app movil al escanear
  // dispositivos cercanos.
  BLEDevice::init(server_name);
  ble_server = BLEDevice::createServer();
  // asignamos el callback que se disparará cada vez que un cliente
  // conecta o desconecta.
  ble_server->setCallbacks(new ServerCB());
  // el id del servicio que ofrecemos es un UART.
  BLEService *server_service = ble_server->createService(SERVICE_UUID);
  // asignamos la caracteristica TX, la cual enviará al cliente movil nuevos mensajes.
  // desde el punto de vista de la app movil lee datos cuando estén disponibles.
  tx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  // sobre GATT puede ser interesante echar un vistazo a los siguientes enlaces:
  // https://www.bluetooth.com/specifications/gatt
  // sobre el descritor 0x2902
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  tx_uart->addDescriptor(new BLE2902());
  // RX lee nuevos mensajes desde la app movil.o dicho de otra forma
  // escribimos desde la app movil sobre la caracteristica.
  rx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  // asignamos el callback que se disparará cuando hay datos en la característica RX
  rx_uart->setCallbacks(new characteristicCB());

  // iniciamos el servicio y comenzamos a anunciar
  // el nombre de nuestro servidor.
  server_service->start();
  ble_server->getAdvertising()->start();

  // loop
  while (1)
  {
    //
    if (deviceConnected)
    {
      // hay un movil conectado, notificamos y enviamos los datos
      if (txValue.size() > 0)
      {
        Serial.print("[BLE] Sending: ");
        Serial.println(txValue.c_str());
        tx_uart->setValue(txValue);
        tx_uart->notify();
        txValue.clear();
      }
      delay(10);
    }
    else
    {
      // no hay movil conectado pero hay datos pendientes de entregar.
      // aqui podemos enlazar la funcion para el guardado
      // de mensajes pendientes de entrega.
      if (txValue.size() > 0)
      {
        Serial.println("Device not connected to BLE interface");
        // - ToDo: guardamos los mensajes
        // - limpiamos la variable
        txValue.clear();
      }
      delay(10);
    }
  } // WHILE
};
