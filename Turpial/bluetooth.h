/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// declaracion de librerias
#include <Arduino.h>
#include "general_functions.h"
#include "radio.h"
#include "route.h"
#include "debugging.h"

//declaracion de variables
// Service UUID
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// Characteristic RX
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
// Characteristic TX
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


// declaracion de funciones
void task_bluetooth(void *params);
void BLE_incoming(char* uid2,char* msg_ble, char* timemsg, char* hash_msg, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes, rutas_t routeTable[MAX_ROUTES], uint8_t total_rutas);

#endif // BLUETOOTH_H
