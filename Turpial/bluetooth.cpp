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
      DEBUG_PRINT("[BLE] Received:");
      DEBUG_PRINTLN(rxValue.c_str());
      // radioSend(rxValue);   // el enviar por radio va en el main loop en packet_processing_outcoming(), enviar por radio lo que deberia hacer es leer la tabla mensajes_salientes y enviar de a un registro a la vez
      // por que el procesamiento de paquetes es FIFO y si hay paquetes esperando por salir por la radio Lora no puede salir primero un paquete BLE hacia la radio Lora
      // se procesa el ble incoming
      String parametro;
      parametro = String(rxValue.c_str()); // se convierte de string c null terminated a System:String
                                           // el siguiente void extrae del String BLE los 3 parametros: uid,msg,time

      json_receive(parametro, uid_temporal, msg_temporal, time_temporal, hash_temporal);
      DEBUG_PRINTLN(F("Json received:"));
      DEBUG_PRINT("uid:");
      DEBUG_PRINTLN(uid_temporal);
      DEBUG_PRINT("msg:");
      DEBUG_PRINTLN(msg_temporal);
      DEBUG_PRINT("time:");
      DEBUG_PRINTLN(time_temporal);
      DEBUG_PRINT("hash:");
      DEBUG_PRINTLN(hash_temporal);
      // se procesa el comando que se haya recibido por BLE

      // se valida el hash del msg para ver si esta integro
      if (is_valid_hash160(msg_temporal, hash_msg))
      {
        //if (timemsg>now()){
        // se sincroniza la hora en caso de que este desfasada, se confia en que el relogj del movil este correcto
        // setTime(timemsg);
        //}

        // se verifica si viene un comando remoto para el turpial por BLE
        String msg_tmp = (String)msg_temporal;
        msg_tmp.toUpperCase();
        int pos_remote = msg_tmp.indexOf("REMOTE:");

        if (pos_remote >= 0)
        {
          DEBUG_PRINT(F("Remote command received:"));
          DEBUG_PRINTLN(msg_tmp);
          remote_debugging = (String)msg_tmp;
          // txValue=("OK").c_str();
        }
        else
        {
          DEBUG_PRINTLN(F("antes de comenzar BLE incoming:"));

          BLE_incoming(uid_temporal, msg_temporal, time_temporal, hash_temporal, mensajes_salientes, total_mensajes_salientes); // este procesamiento coloca los paquetes broadcast en la cola de mensajes salientes, la cola se procesa en el main loop
          DEBUG_PRINTLN(F("BLE process OK"));
        }
      }
      else
      {
        // no es valido el hash del mensaje
        DEBUG_PRINTLN(F("Invalid hash received on BLE message"));
      }
      rxValue.clear();
      // se vacia el buffer BLE
    }
  }
  void onRead(BLECharacteristic *pCharacteristic)
  {
    if (txValue.size() > 0)
    {
      Serial.println(F("enviando al BLE desde onRead"));
      pCharacteristic->setValue(txValue);
      delay(50);
      txValue.clear();
    }
  }
};

void task_bluetooth(void *params)
{

  BLEDevice::setMTU(128);
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
    // codigo parte 2
    if (deviceConnected)
    {

      if (txValue.size() > 0)
      {
        tx_uart->setValue(txValue);
        tx_uart->notify();
        // se coloca un delay antes de borrar la variable para que pueda llegar el ack del BLE
        delay(50);
        txValue.clear();
      }
    }
    else
    {
      // si hay algo para envio pero no esta el dispositivo conectado
      if (txValue.size() > 0)
      {
        // solo si hay algo para mandar al BLE pero no hay cliente conectado
        Serial.println(F("No hay dispositivo BLE conectado"));
        // se devuelve el packet como not delivered
        packet_return_BLE_str = (String)txValue.c_str();
        // se coloca un delay antes de borrarlo para que pueda haber sido enviado por BLE y recibido el ACK correspondiente
        delay(50);
        txValue.clear();
      }
    }
    delay(50);
  } // WHILE
};
