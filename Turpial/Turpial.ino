/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h>

// devices and default settings
#include "hardware.h"
#include "language_es.h"
#include "boards_def.h"
#include "memory_def.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "debugging.h"

#include <Wire.h>



#ifdef RAD_ENABLED
  //#include <LoRaLib.h>
  #include "heltec.h"
#endif

#ifdef WAP_ENABLED
  #include <WiFi.h>
#else
  #ifdef WST_ENABLED
    #include <WiFi.h>
  #endif
#endif
  
#ifdef SCR_ENABLED  
  
  #include "oled/OLEDDisplayUi.h"
  #include "scr_images.h"
#endif
#ifdef BLE_ENABLED 
  #include "bluetooth.h"
#endif 




#ifdef SCR_ENABLED   
  extern Heltec_ESP32 Heltec;
  OLEDDisplayUi ui( Heltec.display );
     
#endif 

// variables fijas para este demo
// ID unico del nodo
char id_nodo_demo[]="turpial.0";


char* id_node=id_nodo_demo;



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

// variables para trasmision BLE
  String rxValue;
  String txValue;


#ifdef SCR_ENABLED   

void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) {
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String(millis()));
}



void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Nodes Locha Mesh");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 25, "Total Neigbours:");
  display->drawString(x+10, y + 35, (String)total_vecinos);
  display->drawString(x, y + 45, "Total Blacklisted:");
  display->drawString(x+10, y + 55, (String)total_nodos_blacklist);
}

void drawFrame2(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Routes Locha Mesh");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 25, "Total Routes:");
  display->drawString(x, y + 35, (String)total_rutas);
}

void drawFrame3(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Outcoming Queue");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 25, "Total packets queue:");
  display->drawString(x, y + 35, (String)total_mensajes_salientes);
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Services");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 25, "Wifi:");
  String msg_screen="Inactive";
  #ifdef WAP_ENABLED
  msg_screen="Active";
  #else
    #ifdef WST_ENABLED
       msg_screen="Active";
    #endif
  #endif
  display->drawString(x, y + 35, "Lora:");
  msg_screen="Inactive";
  #ifdef RAD_ENABLED
    msg_screen="Active";
  #endif
  display->drawString(x, y + 45, msg_screen);
display->drawString(x, y + 55, "BLE:");
  msg_screen="Inactive";
  #ifdef BLE_ENABLED
    msg_screen="Active";
  #endif
  display->drawString(x, y + 65, msg_screen);
  
}

FrameCallback frames[] = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 };

int frameCount = 4;

#endif



void setup()
{
  bool display_enabled=false;
  bool lora_enabled=false;
  bool serial_enabled=false;
 #ifdef DEBUG
    serial_enabled=true;
 #endif
 #ifdef SCR_ENABLED
    display_enabled=true;
 #endif
 #ifdef RAD_ENABLED
    lora_enabled=true;
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
       
         #ifdef SCR_ENABLED
      // se inicializa el display
      ui.setTargetFPS(30);
      // Customize the active and inactive symbol
      ui.setActiveSymbol(activeSymbol);
      ui.setInactiveSymbol(inactiveSymbol);
      // You can change this to
      // TOP, LEFT, BOTTOM, RIGHT
      ui.setIndicatorPosition(BOTTOM);
    
      // Defines where the first frame is located in the bar.
      ui.setIndicatorDirection(LEFT_RIGHT);
    
      // You can change the transition that is used
      // SLIDE_LEFT, SLIDE_RIGHT, SLIDE_UP, SLIDE_DOWN
      ui.setFrameAnimation(SLIDE_LEFT);

      // Add frames
      ui.setFrames(frames, frameCount);
    
      // Initialising the UI will init the display too.
      ui.init();
    
      Heltec.display->flipScreenVertically();
  
    #endif
        
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
   
        //    xTaskCreate(task_bluetooth, "task_bluetooth", 1024 * 2, NULL, 5, NULL);
        
        
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
    // xTaskCreate(task_rad, "task_rad", ...);
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

pinMode(LED_PIN, OUTPUT);
#ifdef LED_ENABLED
    digitalWrite(LED_PIN, HIGH);
#else
    digitalWrite(LED_PIN, LOW);
#endif
}

void loop()
{


//  int remainingTimeBudget = ui.update();

//  if (remainingTimeBudget > 0) {
    // You can do some work here
    // Don't do stuff if you are below your
    // time budget.
    
 // se efectua el procesamiento de paquetes entrantes
          packet_processing_incoming();
          // se efectua el procesamiento de paquetes salientes
          packet_processing_outcoming();   
          // solo se agrega la consola de comandos cuando se esta compilando para DEBUG
          #ifdef DEBUG
              uint8_t rpta=show_debugging_info(vecinos,total_vecinos);
          #endif

    
//    delay(remainingTimeBudget);
//  }
         

 
}
