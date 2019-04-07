/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef RADIO_H 
#define RADIO_H
#include <Arduino.h>


uint8_t radioSend(String _data);
void task_radio(void *params);

extern void process_Lora_incoming();

#endif // RADIO_H
