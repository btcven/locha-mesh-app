/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef RADIO_H 
#define RADIO_H

#include <Arduino.h>


void radioSend(std::string _data);
void task_radio(void *params);

#endif // RADIO_H
