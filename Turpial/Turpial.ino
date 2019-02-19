/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#include "defaultStartup.h"

void setup() {
  Serial.begin(115200);
  // Screen (aka SCR) active on boot?
  if (SCR_ENABLED) {
    // call to function start or task
  }

  // BLE iface active on boot?
  if (BLE_ENABLED) {
    xTaskCreate(bluetooth_task, "bluetooth_task", 4096, NULL, 5, NULL);
  }

  // RAD iface active on boot?
  if (RAD_ENABLED) {
    // call to function start or task
  }

  // WAP iface active on boot?
  if (WAP_ENABLED) {
    // call to function start or task
  }

  // WST iface active on boot?
  if (WST_ENABLED) {
    // call to function start or task
  }

}

void loop() {
  // put your main code here, to run repeatedly:

}
