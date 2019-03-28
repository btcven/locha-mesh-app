/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <SPI.h>
#include "LoRa.h"
#include "radio.h"

extern std::string txValue_Lora;
extern std::string rxValue_Lora;


// recibe un paquete
void onReceive(int packetSize) {
  // txValue << packet
  // pasar a la variable txValue.
  char in_process;
  for (int i = 0; i < packetSize; i++) {
    in_process=(char)LoRa.read();
   // se coloca en el Buffer Lora
   if (in_process!='|'){
      rxValue_Lora=rxValue_Lora+in_process;  
      Serial.print(in_process);
   } else {
      // es un fin de mensaje
      Serial.print("procesar packet recibido por Lora:");
      Serial.println(rxValue_Lora.c_str());
      // se coloca en Buffer_packet
      // se invoca process_received_packet para procesar el buffer_packet recibido
      
      rxValue_Lora.clear();
   }
    
    
  }
  Serial.print("\n");
}

// envía un paquete.
void radioSend(std::string _data) {
  // hay que verificar primero si el canal esta libre Listen before Talk
  
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
    if (txValue_Lora.size() > 0) {
      Serial.println(txValue_Lora.c_str());

    }
    delay(10);
  }
}
