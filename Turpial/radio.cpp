/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <SPI.h>
#include "radio.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "LoRa.h"
#include "packet.h"
#include "routing_incoming.h"
#include "debugging.h"

extern std::string txValue_Lora;
extern std::string rxValue_Lora;
extern char *id_node;


// Funcion para validar si un string es numerico
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

// recibe un paquete, es invocado via callback 
void onReceive(int packetSize) {
  // txValue << packet
  // pasar a la variable txValue.
  char in_process;
  const uint8_t tamano_header = 41;  // tama;o en bytes del packet_t header
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

 // se verifica el header del mensaje recibido a ver si es un packet valido
    if (rxValue_Lora.length()>tamano_header){
    // se toma solo el header
      std::string header_str=rxValue_Lora.substr(0, tamano_header); 
      // se verifica si el type es valido
      std::string header_type=header_str.substr(0, 1);
      std::string header_from=header_str.substr(1, 16);
      std::string header_to=header_str.substr(17, 16);
      std::string header_timestamp=header_str.substr(33, 8);
      Serial.print("recibi:");
      Serial.print("type:");
      Serial.print(header_type.c_str());
       Serial.print("from:");
      Serial.print(header_from.c_str());
      Serial.print("to:");
      Serial.print(header_to.c_str());
       Serial.print("timestamp:");
      Serial.println(header_timestamp.c_str());
        
      if (is_number(header_type)){   
        // se coloca en Buffer_packet
        char uid_temp[16];
        
        packet_t packet_tmp;
        packet_tmp.header.type=convertir_str_packet_type_e(header_type.c_str());
        strncpy(packet_tmp.header.from, header_from.c_str(), sizeof(packet_tmp.header.from));
        strncpy(packet_tmp.header.to, header_to.c_str(), sizeof(packet_tmp.header.to));
        packet_tmp.header.timestamp=std::atoi (header_timestamp.c_str());
       
        process_received_packet(id_node,packet_tmp);
        
      }
    }
      rxValue_Lora.clear();
   }
    
    
  }
  Serial.print("\n");
}

// envía un paquete.
void radioSend(std::string _data) {
  // hay que verificar primero si el canal esta libre Listen before Talk
  Serial.print("voy a enviar el packet:");
  Serial.println(_data.c_str());
  LoRa.beginPacket();
  LoRa.print(_data.c_str());
  int done = LoRa.endPacket();
  Serial.print("enviado OK");
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


// declaracion del radio Lora y su vinculacion via SPI
void task_radio(void *params) {

  DEBUG_PRINT(MSG_SCR);
  DEBUG_PRINT(" ");
  DEBUG_PRINT(MSG_INIT);
  DEBUG_PRINTLN(" ");

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, RAD_CSS);
  LoRa.setPins(RAD_CSS, RAD_RST, RAD_DIO0);

  int rad_isInit = LoRa.begin(RAD_BAND, RAD_PABOOST);

  if (rad_isInit) {
    DEBUG_PRINT(MSG_OK);
  } else {
    DEBUG_PRINT(MSG_ERROR);
    // kill task by task handler
  }

  // al recibir ejecutar el callback onReceive()
  LoRa.onReceive(onReceive);
  // ponemos en modo recepcion.
  LoRa.receive();
// se deja en modo recibiendo el radio Lora
  while (1) {
    if (txValue_Lora.size() > 0) {
      DEBUG_PRINT("LoRa:");
      DEBUG_PRINTLN(txValue_Lora.c_str());

    }
    delay(10);
  }
}
