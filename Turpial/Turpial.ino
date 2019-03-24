/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h>
#include <Wire.h>

// devices and default settings
#include "hardware.h"
#include "language_es.h"
#include "boards_def.h"
#include "memory_def.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "debugging.h"
#include "fonts/DejaVu_Sans_12.h"
#include "screens.h"


#ifdef RAD_ENABLED
  #include "heltec.h"
  #include "radio.h"
#endif

#ifdef WAP_ENABLED
  #include <WiFi.h>
#else
  #ifdef WST_ENABLED
    #include <WiFi.h>
  #endif
#endif
  
#ifdef SCR_ENABLED  

 // extern Heltec_ESP32 Heltec;
//  OLEDDisplay *display = Heltec.display;
  #include "scr_images.h"
#endif
#ifdef BLE_ENABLED 
  #include "bluetooth.h"
#endif 



// variables fijas para este demo
// ID unico del nodo
char id_nodo_demo[]="turpial.0";
char* id_node=id_nodo_demo;


#ifdef SCR_ENABLED  
  #define OLED_SCREEN_INTERVAL 5000 
#endif

// includes internos
uint8_t total_vecinos = 0; // cantidad de vecinos del nodo actual
uint8_t total_rutas = 0;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes = 0;   // cantidad de mensajes en la cola
uint8_t total_nodos_blacklist = 0;   // cantidad de nodos en blacklist

rutas_t routeTable[MAX_ROUTES];
nodo_t vecinos[MAX_NODES];
nodo_t blacklist[MAX_NODES_BLACKLIST];
message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
packet_t Buffer_packet;   // packet_t usado como buffer para mensajes incoming y outcoming

unsigned long tiempo;

// variables para trasmision BLE
  String rxValue="";
  String txValue="";


void setup()
{
  bool display_enabled=false;
  bool lora_enabled=false;
  bool serial_enabled=false;
  bool wifi_enabled=false;
  
 #ifdef DEBUG
    serial_enabled=true;
 #endif
 #ifdef SCR_ENABLED
 if (SCR_ENABLED) {
    display_enabled=true;
 } else {
      display_enabled=false;
    }
 #endif
 #ifdef RAD_ENABLED
 if (RAD_ENABLED) {
    lora_enabled=true;
 } else {
      lora_enabled=false;
    }
 #endif
 #ifdef WST_ENABLED
 if (WST_ENABLED) {
    wifi_enabled=true;
 } else {
      wifi_enabled=false;
    }
 #endif
  #ifdef WAP_ENABLED
 if (WAP_ENABLED) {
    wifi_enabled=true;
 } else {
      wifi_enabled=false;
    }
 #endif
 #ifdef DEBUG
    DEBUG_BEGIN(BAUDRATE);
    while(!Serial);
    Serial.setDebugOutput(true);
    delay(2000);
 #endif
  
    // se inicializa la libreria para Heltec
    Heltec.begin(display_enabled /*DisplayEnable Enable*/, !lora_enabled /*LoRa Disable*/, serial_enabled /*Serial Enable*/);
    
  
 
  rxValue="";
  txValue="";

      #ifdef SCR_ENABLED  
      if (SCR_ENABLED)
      {
        DEBUG_PRINTLN(MSG_SCR_INIT);
        // activar módulo SCR
        // leer NVS,  verificar si existe registro
        // si existe aplicar, si no establecer parametros por defecto.
       
        
      // se inicializa el display
      //Heltec.display->init();
      //Heltec.display->flipScreenVertically();
      display->init();
      display->flipScreenVertically();
  
        
      }
      #endif

      #ifdef BLE_ENABLED
      if (BLE_ENABLED)
      {
        DEBUG_PRINTLN(F("[BLE] Initiating... "));
        // -- activar módulo ble --
        // 1.- leer NVS,  verificar si existe registro
        // 2.- si existe aplicar, si no establecer parametros por defecto.
        // 3.- activar la tarea
   
         xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
        
      }
      #endif

      #ifdef WAP_ENABLED
          if (WAP_ENABLED)
          {
            DEBUG_PRINT(F("[WAP] Initiating... "));
            // -- activar módulo wap --
          }
      #endif
  #ifdef WST_ENABLED
  if (WST_ENABLED)
  {
    DEBUG_PRINTLN(F("[WST] Initiating... "));
    // -- activar módulo wst --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
  }
   #endif
  #ifdef RAD_ENABLED
  if (RAD_ENABLED)
  {
    DEBUG_PRINTLN(F("[RAD] Initiating... "));
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
     xTaskCreate(task_radio, "task_radio", 2048, NULL, 5, NULL);
  }
 #endif
 
// se coloca el cursor en el terminal serial
 
      DEBUG_PRINTLN(F("Starting terminal"));
      // se genera el node_id solo si no existe
      if (id_node==""){
         create_unique_id(id_node);
      }
      DEBUG_PRINT(id_node);
      DEBUG_PRINT(F(" >"));


#ifdef LED_ENABLED
    pinMode(LED_PIN, OUTPUT);
    if (LED_ENABLED){
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);  
    }
#endif

// se inicializa el control del tiempo
tiempo=millis();
}


int pantalla_activa=1;

void loop()
{

    
    if (millis()-tiempo>OLED_SCREEN_INTERVAL){
      //Heltec.display->clear();
      display->clear();
      switch (pantalla_activa) {
    case 1:
     // drawframe_title_with_2_fields(Heltec.display, 0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      drawframe_title_with_2_fields(display, 0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      break;
    case 2:
      //drawframe_table_with_4_fields(Heltec.display, 0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos)+" bytes", "Size:", (String)sizeof(blacklist)+" bytes");
      drawframe_table_with_4_fields(display, 0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos)+" bytes", "Size:", (String)sizeof(blacklist)+" bytes");
      break;
    case 3:
     // drawframe_title_with_2_fields(Heltec.display, 0, 0, "Routes Locha Mesh","Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable)+" bytes");
      drawframe_title_with_2_fields(display, 0, 0, "Routes Locha Mesh","Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable)+" bytes");
      break;
    case 4:
      //drawframe_title_with_2_fields(Heltec.display, 0, 0, "Outcoming Queue","Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes)+" bytes");
      drawframe_title_with_2_fields(display, 0, 0, "Outcoming Queue","Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes)+" bytes");
      break;
    case 5:
      //drawFrame5(Heltec.display, 0, 0);
      drawFrame5(display, 0, 0);
      break;
    case 6:
     // drawFrame_tech(Heltec.display, 0, 0);
      drawFrame_tech(display, 0, 0);
      break;
    default:
     pantalla_activa=0;  //para que aparezca la primera pantalla
      break;
  }
  
  pantalla_activa++;
  tiempo=millis();
  if (pantalla_activa>6){
    pantalla_activa=1;
  }
  
     // Heltec.display->display();
      display->display();
     
    }
    
if (millis()-tiempo<0){
  tiempo=millis();
}
   
    
    
    
 // se efectua el procesamiento de paquetes entrantes
          packet_processing_incoming();
          // se efectua el procesamiento de paquetes salientes
          packet_processing_outcoming();   
          // solo se agrega la consola de comandos cuando se esta compilando para DEBUG
          #ifdef DEBUG
         
              uint8_t rpta=show_debugging_info(vecinos,total_vecinos);
          #endif

  
}
