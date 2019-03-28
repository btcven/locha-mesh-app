/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <SPI.h>
#include "LoRa.h"
#include "radio.h"

extern std::string txValue;
extern std::string rxValue;


// recibe un paquete
void onReceive(int packetSize) {
  // txValue << packet
  // pasar a la variable txValue.
  for (int i = 0; i < packetSize; i++) {
    Serial.print((char)LoRa.read());
  }
  Serial.print("\n");
}

// envía un paquete.
void radioSend(std::string _data) {
  // hay que verificar primero si el canal esta libre
  
  LoRa.beginPacket();
  LoRa.print(_data.c_str());
  int done = LoRa.endPacket();
  if (done) {
    
    //LoRa.onReceive(onReceive);
    LoRa.receive();
  }
  // ..::HEADER::..
  // from:
  // to: from_phone
  // time: from_phone
  // ..::BODY::..
  // payload
}

void task_radio(void *params) {

  Serial.print(F("[RAD] Initializing ... "));

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, RAD_CSS);
  LoRa.setPins(RAD_CSS, RAD_RST, RAD_DIO0);

  int rad_isInit = LoRa.begin(RAD_BAND, PABOOST);

  if (rad_isInit) {
    Serial.println("OK");
  } else {
    Serial.println("ERROR");
    // kill task by task handler
  }

  // al recibir ejecutar el callback onReceive()
  LoRa.onReceive(onReceive);
  // ponemos en modo recepcion.
  LoRa.receive();

  while (1) {
    if (txValue.size() > 0) {
      Serial.println(txValue.c_str());

    }
    delay(10);
  }



};
