/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef RADIO_H 
#define RADIO_H
// declaracion de librerias
#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include <LoRa.h>
#include "memory_def.h"

// declaracion de funciones
uint8_t radioSend(String _data);
void task_radio(void *params);
void process_Lora_incoming(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos);

#endif // RADIO_H
