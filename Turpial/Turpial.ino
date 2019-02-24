/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include "Configuration.h"


#include <WiFiUdp.h>  // para uso de websockets
#include <EEPROM.h>  // para el manejo de configuraciones en EPPROM
#include <FS.h>   // Include the SPIFFS library


SCR_status_t  SCRStatus;
BLE_status_t  BLEStatus;
WST_status_t  WSTStatus;
WAP_status_t  WAPStatus;
RAD_status_t  RADStatus;

void setup() {
  Serial.begin(BAUDRATE);
  // Screen (aka SCR) active on boot?
  if (SCR_ENABLED) {
    SCRStatus.isEnabled = true;
    Serial.printf(MSG_SCR_ALLOCATING);
    if (screen_init()) {
      SCRStatus.isActive = true;
      Serial.printf(MSG_OK);
    } else {
      SCRStatus.isActive = false;
      Serial.printf(MSG_ERROR);
    }
  }

  // BLE iface active on boot?
  if (BLE_ENABLED) {
    BLEStatus.isEnabled = true;
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

}
