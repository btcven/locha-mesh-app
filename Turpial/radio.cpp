/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
// declaracion de librerias
#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include <LoRa.h>
#include "radio.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "packet.h"
#include "general_functions.h"
#include "routing_incoming.h"
#include "debugging.h"
using namespace std;

//declaracion de variables
// para BLE
extern std::string txValue;
extern std::string rxValue;
// para LoRa
extern bool radio_Lora_receiving;
extern std::string txValue_Lora;
extern std::string rxValue_Lora;
extern int Lora_RSSI;
extern int Lora_SNR;
extern char *id_node;
extern unsigned long tiempo_desde_ultimo_packet_recibido;
extern nodo_t vecinos[MAX_NODES];
extern uint8_t total_vecinos;  



// Funcion para validar si un string es numerico
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

// funcion encargada de procesar un mensaje recibido por el radio Lora, se recibe una cadena de caracteres en la variable rxValue_Lora
// esta funcion verifica que sea un paquete valido y lo manda a procesar segun el tipo de paquete que corresponda por medio de la funcion process_received_packet()
// esta funcion no puede ser invocada directo desde un callback de un task FreeRTOS y por eso se usa una variable global en el archivo main (Turpial.ino) para poderla  invocar desde el callback

void process_Lora_incoming(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos){
      // se encarga de procesar la cadena recibida por Lora
      // no puede ser invocado desde Onreceive debido a un bug en la libreria Lora.h
      // https://www.bountysource.com/issues/70601319-calling-any-function-in-the-callback-method
    
     String mensaje_recibido="";
     
     bool recibido=false;
     char* mensaje_recibido_char;
      // recibo las variables globales que trae el packet
     Lora_RSSI=LoRa.packetRssi();
     Lora_SNR=LoRa.packetSnr();
  
     int RSSI_packet_actual=Lora_RSSI;
     int SNR_packet_actual=Lora_SNR;
      
     mensaje_recibido=rxValue_Lora.c_str();
     DEBUG_PRINTLN(F("recibiendo mensaje via LoRa"));
     rxValue_Lora.clear();  // se libera el buffer Lora
     radio_Lora_receiving=false;  //  se habilita para que se pueda recibir otro packet
     

      mensaje_recibido_char=string2char(mensaje_recibido);
      packet_t packet_received=packet_deserialize_str(mensaje_recibido);



 // se verifica el header del mensaje recibido a ver si es un packet valido
      DEBUG_PRINT(F("Procesar packet recibido por LoRa1:"));
      DEBUG_PRINTLN((String)mensaje_recibido);
      DEBUG_PRINT(F("Detalle del packet:"));
      DEBUG_PRINT(F("type:"));
      DEBUG_PRINT(convertir_packet_type_e_str(packet_received.header.type));
      DEBUG_PRINT((String)packet_received.header.type);
      DEBUG_PRINT(F("from:"));
      DEBUG_PRINT(packet_received.header.from);
      DEBUG_PRINT(F("to:"));
      DEBUG_PRINT(packet_received.header.to);
      DEBUG_PRINT(F("timestamp:"));
      DEBUG_PRINT((String)packet_received.header.timestamp);
      DEBUG_PRINT(F("payload:"));
      DEBUG_PRINTLN(packet_received.body.payload);

        // si no existe la ruta previamente se agrega la nueva ruta, si existe la ruta se actualiza el age de esa ruta
        
        
              
              // se hace la parte de enrutamiento del packet
              process_received_packet(id_node,packet_received,vecinos,total_vecinos,routeTable,total_rutas,RSSI_packet_actual,SNR_packet_actual);
        
      }


// recibe un paquete , es invocado via callback desde el xTask task_radio en el main (turpial.ino)
void onReceive(int packetSize) {
   // modificaciones para evitar el error de call back
   Serial.println("Recibiendo un packet ...");
   if (packetSize == 0) return; 
     
    char in_process;
    uint8_t i;
    if (!radio_Lora_receiving){
    if (packetSize) {
    
      // estaba packesize-1 y se cambio packetsize 
      for (i = 0; i < packetSize; i++) {
        in_process=(char)LoRa.read();
       // se coloca en el Buffer Lora
          rxValue_Lora=rxValue_Lora+in_process;  
         
      }
    // se usa la variable boolean radio_Lora_receiving para indicar en el loop main que se puede procesar el contenido de rxValue_Lora
    // se hace de esta forma porque la libreria Lora.cpp tiene un bug y no permite invocar voids ni funciones dentro de onReceive, y tambien para facilitar la comunicacion entre los callbacks y el main
 tiempo_desde_ultimo_packet_recibido=millis();

  radio_Lora_receiving=true;  
  
  }
  
}
}

// envía un paquete String recibido
uint8_t radioSend(String _data) {
  uint8_t done =0;
  uint8_t rpta;
  uint16_t delay_time=250;  // millisegundos entre reintento de envio
  uint8_t ii;
  // hay que verificar primero si el canal esta libre Listen before Talk
  DEBUG_PRINT(F("se envia el packet..."));
  DEBUG_PRINTLN(_data.c_str());
  LoRa.setTxPower(20,false);
  // se hacen 5 intentos de delibery a busy variables en caso de que el canal este ocupado
   for (ii = 0; ii<5; ++ii){
      rpta= LoRa.beginPacket();
      if (rpta==1){ 
        break;  // si logra enviarse se sale de los reintentos
      }
      DEBUG_PRINT(F("radio busy, reintentando ..."));
      delay(delay_time);
      delay_time=500+delay_time; //
   }
   LoRa.print(_data.c_str());
  done = LoRa.endPacket();
  LoRa.setTxPower(2,false);
  // se coloca en modo receive para que siga escuchando packets
    // ponemos en modo recepcion.
    delay(20);
  LoRa.receive();
  
 if (rpta==1){ 
  if (done){
  DEBUG_PRINT("Cantidad de reintentos efectuados:");
  DEBUG_PRINT((String)ii);
  DEBUG_PRINTLN(F("enviado OK"));
   // se coloca en modo receive para que siga escuchando packets
    // ponemos en modo recepcion.
  LoRa.receive();
  
 
  
  // ..::HEADER::..
  // from:
  // to: from_phone
  // time: from_phone
  // ..::BODY::..
  // payload

    
    return 1;
  }
  }
 Serial.println("Lora continues...");

 // cualquier otro escenario devuelve 0, packet no enviado
 return 0;
}


// declaracion del radio Lora y su vinculacion via SPI
void task_radio(void *params) {

 // DEBUG_PRINT(MSG_SCR);
 // DEBUG_PRINT(" ");
 // DEBUG_PRINT(MSG_INIT);
 // DEBUG_PRINTLN(" ");

  SPI.begin(RAD_SCK, RAD_MISO, RAD_MOSI, RAD_CSS);
  LoRa.setPins(RAD_CSS, RAD_RST, RAD_DIO0);
  LoRa.setSPIFrequency(1e6);
  int rad_isInit = LoRa.begin(RAD_BAND, RAD_PABOOST);
LoRa.setTxPower(2,false);
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
      DEBUG_PRINT(F("LoRa:"));
      DEBUG_PRINTLN(txValue_Lora.c_str());
      Lora_RSSI = LoRa.packetRssi();
      Lora_SNR=LoRa.packetSnr();
    }
    delay(40);
  }
}
