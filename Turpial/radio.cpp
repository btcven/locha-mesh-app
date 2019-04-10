/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <SPI.h>
#include <string.h>
#include "radio.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include <LoRa.h>
#include "packet.h"
#include "general_functions.h"
#include "routing_incoming.h"
#include "debugging.h"

using namespace std;

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


// Funcion para validar si un string es numerico
bool is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(), 
        s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

void process_Lora_incoming(){
  // se encarga de procesar la cadena recibida por Lora
  // no puede ser invocado desde Onreceive debido a un bug en la libreria Lora.h
  // https://www.bountysource.com/issues/70601319-calling-any-function-in-the-callback-method

 String mensaje_recibido="";
 
 bool recibido=false;
 char* mensaje_recibido_char;
  // recibo las variables globales que trae el packet
  int RSSI_packet_actual=Lora_RSSI;
  int SNR_packet_actual=Lora_SNR;
  
 mensaje_recibido=rxValue_Lora.c_str();
  Lora_RSSI=LoRa.packetRssi();
  Lora_SNR=LoRa.packetSnr();
 Serial.println("recibiendo mensaje via LoRa");
 rxValue_Lora.clear();  // se libera el buffer Lora
 radio_Lora_receiving=false;  //  se habilita para que se pueda recibir otro packet
 
 // mensaje_recibido.toCharArray(mensaje_recibido_char, mensaje_recibido.length());
   mensaje_recibido_char=string2char(mensaje_recibido);
packet_t packet_received=packet_deserialize_str(mensaje_recibido);
//packet_received.


 // se verifica el header del mensaje recibido a ver si es un packet valido
      Serial.print(F("largo recibido:"));
      Serial.println((String)mensaje_recibido.length());
 
      Serial.print(F("procesar packet recibido por LoRa1:"));
      Serial.println((String)mensaje_recibido);
    // Serial.print(F("procesar packet recibido por LoRa2:"));
    // Serial.println((String)mensaje_recibido_char);
    
      Serial.print("recibi:");
      Serial.print("type:");
      
      Serial.print(convertir_packet_type_e_str(packet_received.header.type));
      Serial.print((String)packet_received.header.type);
       Serial.print("from:");
      Serial.print((String)packet_received.header.from);
      Serial.print("to:");
      Serial.print((String)packet_received.header.to);
       Serial.print("timestamp:");
      Serial.print((String)packet_received.header.timestamp);
      
       Serial.print("payload:");
      Serial.println((String)packet_received.body.payload);

        // si no existe la ruta previamente se agrega la nueva ruta, si existe la ruta se actualiza el age de esa ruta
        
  if (!existe_ruta(id_node, packet_received.header.from,true)){
      nodo_t origen;
      nodo_t vecino;
        Serial.print("se agrega una nueva ruta porque no existe previamente la ruta de lo recibido");
      copy_array_locha(id_node, origen.id, 16);
      copy_array_locha(packet_received.header.from, vecino.id, 16);
      // se agrega el nuevo vecino
      if (!es_vecino(vecino.id)){ 
          create_neighbor(vecino.id,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
      }
      // se crea la nueva ruta
      uint8_t rptass= create_route(origen, vecino, vecino);  
  } 

// solo se envian al BLE los packets Lora incoming MSG o TXN
if ((packet_received.header.type==MSG)or(packet_received.header.type==TXN)){
        // se envia al BLE para efectos del demo, se arma en forma de packet
         Serial.print("se envia al BLE:");
         
         Serial.println(mensaje_recibido);
         Serial.print("---");
         Serial.println(mensaje_recibido.c_str());
   //txValue=msg_will_send;

   // se verifica que tipo de mensaje es para mandarlo al movil
   
   mensaje_recibido= Json_return_msg(packet_received.body.payload);
   txValue=mensaje_recibido.c_str();
        
} 
          Serial.print("se va a enrutar lo recibido:");
        // se hace la parte de enrutamiento del packet
        process_received_packet(id_node,packet_received);
  
}


// recibe un paquete , es invocado via callback 
void onReceive(int packetSize) {
   // modificaciones para evitar el error de call back
   
   if (packetSize == 0) return; 
   
  
// char* mensaje_recibido_temp;
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
  // se hace de esta forma porque la libreria Lora.cpp tiene un bug y no permite invocar voids ni funciones dentro de onReceive
 
  radio_Lora_receiving=true;  
  
 
 
        

    Serial.println("saliendo...");
  }
  
}
}

// envía un paquete.
uint8_t radioSend(String _data) {
  int done =0;
  int rpta;
  int delay_time=500;
  // hay que verificar primero si el canal esta libre Listen before Talk
  Serial.print("voy a enviar el packet:");
  Serial.print(_data.c_str());
   for (int ii = 0; ii<5; ++ii){
      rpta= LoRa.beginPacket();
      if (rpta==1){ 
        break;
      }
      Serial.print("radio busy, reintentando ...");
      delay(delay_time);
      delay_time=500+delay_time;
   }
   LoRa.print(_data.c_str());
  done = LoRa.endPacket();
 if (rpta==1){ 
  if (done){
  Serial.print("enviado OK");
  // se coloca en modo receive para que siga escuchando packets
  
 
  
  // ..::HEADER::..
  // from:
  // to: from_phone
  // time: from_phone
  // ..::BODY::..
  // payload



    LoRa.receive();
    
    return 1;
  }
  }
 // cualquier otro escenario devuelve 0, packet no enviado
 LoRa.receive();
 return 0;
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
      Lora_RSSI = LoRa.packetRssi();
      Lora_SNR=LoRa.packetSnr();
    }
    delay(40);
  }
}
