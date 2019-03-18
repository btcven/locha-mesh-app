/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef RADIO_H 
#define RADIO_H

#define RADIO_ENABLED 1

#ifdef RADIO_ENABLED

#define RAD_SCK   5
#define RAD_MISO  19
#define RAD_MOSI  27
#define RAD_CSS   18
#define RAD_RST   14
#define RAD_DIO0  26
#define RAD_BAND  915E6
#define PABOOST   true
#endif

#include "packet.h"

void radioSend(std::string _data);
void task_radio(void *params);

#endif // RADIO_H
