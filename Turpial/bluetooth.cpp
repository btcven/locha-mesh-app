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
#include <Time.h>
#include <TimeLib.h>
#include "general_functions.h"
#include "bluetooth.h"
#include "radio.h"
#include "route.h"
#include "debugging.h"

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes;
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;

extern String packet_return_BLE_str;
extern String packet_return_Lora_str;
extern String remote_debugging;

extern not_delivered_type_e why_not_delivered; // causa de no entrega de algun packet

BLEServer *ble_server = NULL;
BLECharacteristic *tx_uart;
BLECharacteristic *rx_uart;

std::string server_name = "mesh.locha.io";

extern std::string txValue;
extern std::string rxValue;

// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;

extern char *uid;
extern char *msg;
extern double timemsg;
extern char *hash_msg;

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
    char *uid_temporal = NULL;
    char *msg_temporal = NULL;
    char *hash_temporal = NULL;
    char *time_temporal = NULL;

    // movil -> ble_server(Turpial)
    rxValue = pCharacteristic->getValue();
    // si tenemos datos podemos enviarlos via radio desde aqui.
    if (rxValue.size() > 0)
    {
      String parametro;
      parametro = String(rxValue.c_str()); 
      // se convierte de string c null terminated a System:String
      // se procesa el comando que se haya recibido por BLE

      // se valida el hash del msg para ver si esta integro
      if (is_valid_hash160(msg_temporal, hash_msg))
      {
        // se verifica si viene un comando remoto para el turpial por BLE
        String msg_tmp = (String)msg_temporal;
        msg_tmp.toUpperCase();
        int pos_remote = msg_tmp.indexOf("REMOTE:");

        if (pos_remote >= 0)
        {
          remote_debugging = (String)msg_tmp;
        }
        else
        {
          BLE_incoming(uid_temporal, msg_temporal, time_temporal, hash_temporal, mensajes_salientes, total_mensajes_salientes);
        }
      }
      else
      {
        // Invalid msg checksum.
        ESP_LOGD("BLE", "Invalid msg. checksum");
      }
      rxValue.clear();
    }
  }
};

void task_bluetooth(void *params)
{
  String packet_in_process_str;
  String text_to_send;
 
  uint16_t mtu = 254;
  BLEDevice::setMTU(mtu);
  BLEDevice::init(server_name);

  ble_server = BLEDevice::createServer();
  ble_server->setCallbacks(new ServerCB());
  BLEService *server_service = ble_server->createService(SERVICE_UUID);
  // asignamos la caracteristica TX, la cual enviará al cliente movil nuevos mensajes.
  // desde el punto de vista de la app movil lee datos cuando estén disponibles.
  tx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  // sobre GATT puede ser interesante echar un vistazo a los siguientes enlaces:
  // https://www.bluetooth.com/specifications/gatt
  // sobre el descritor 0x2902
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  tx_uart->addDescriptor(new BLE2902());
  rx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  rx_uart->setCallbacks(new characteristicCB());

  server_service->start();
  ble_server->getAdvertising()->start();

  // loop
  while (1)
  {
    // codigo parte 2
    if (deviceConnected)
    {

      if (txValue.size() > 0)
      {
        packet_in_process_str = txValue.c_str();

        if (packet_in_process_str.indexOf("|") > 0)
        {
          packet_return_BLE_str = packet_in_process_str;
        }
        else
        {
          tx_uart->setValue(txValue);
          tx_uart->notify();
        }
        delay(50);
        txValue.clear();
      }
    }
    else
    {
      // si hay algo para envio pero no esta el dispositivo conectado
      if (txValue.size() > 0)
      {
        ESP_LOGD("BLE", "Mobile not connected..");
        delay(50);
        txValue.clear();
      }
    }

    delay(50);

  } // WHILE
};
