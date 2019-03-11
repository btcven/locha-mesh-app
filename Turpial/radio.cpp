/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <LoRaLib.h>
#include "radio.h"

extern std::string txValue;
extern std::string rxValue;

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

void setFlag(void)
{
  if (!enableInterrupt)
  {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

void task_radio(void *pvParams) {
  SX1276 radio = new LoRa(RAD_CSS, RAD_DIO0, RAD_RST);
  Serial.print("[RAD] Initializing ... ");
  int state = radio.begin(RAD_BAND);
  if (state == ERR_NONE) {
    Serial.println("OK");
  } else {
    Serial.println("FAILED");
  }

};
