/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h>
//using namespace std;



// devices and default settings
#include "hardware.h"
#include "boards_def.h"
#include "memory_def.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "debugging.h"
#include "language_es.h"


#ifdef MCU_ESP32
#include <LoRaLib.h>
#include <Wire.h>
  #include "SSD1306.h"
  #include "screen.h"
  #include "bluetooth.h"
#endif 




#ifdef MCU_ESP32
  SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST, GEOMETRY_128_64); // tambien esta GEOMETRY_128_64 pero depende del screen del HELTEC
#endif 

// variables fijas para este demo
// ID unico del nodo
char* id_node = "turpial.0";


// includes internos
uint8_t total_vecinos = 0; // cantidad de vecinos del nodo actual
uint8_t total_rutas = 0;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes = 0;   // cantidad de mensajes en la cola
rutas_t routeTable[MAX_ROUTES];
nodo_t vecinos[MAX_NODES];
message_queue_t mensajes_salientes[MAX_MSG_QUEUE];


// variables para trasmision BLE
  String rxValue;
  String txValue;

void setup()
{
  
  DEBUG_BEGIN(BAUDRATE);

 
#ifdef MCU_ESP32
      if (SCR_ENABLED)
      {
        DEBUG_PRINT(MSG_SCR_INIT);
        // activar módulo SCR
        // leer NVS,  verificar si existe registro
        // si existe aplicar, si no establecer parametros por defecto.
        if (SCR_Vext)
        {
          pinMode(SCR_Vext, OUTPUT);
          digitalWrite(SCR_Vext, LOW);
          delay(50);
        }
    
        bool SCR_isActive = display.init();
    
        if (SCR_isActive)
        {
          DEBUG_PRINTLN(MSG_OK);
          display.flipScreenVertically();
          display.setTextAlignment(TEXT_ALIGN_LEFT);
          display.setFont(ArialMT_Plain_10);
          display.drawString(0, 0, "Starting ..");
          display.display();
        }
      }
      
      
      if (BLE_ENABLED)
      {
        DEBUG_PRINT(F("[BLE] Initiating... "));
        // -- activar módulo ble --
        // 1.- leer NVS,  verificar si existe registro
        // 2.- si existe aplicar, si no establecer parametros por defecto.
        // 3.- activar la tarea
        #ifdef MCU_ESP32
          xTaskCreate(task_bluetooth, "task_bluetooth", 1024 * 2, NULL, 5, NULL);
        #endif 
      }
      if (WAP_ENABLED)
      {
        DEBUG_PRINT(F("[WAP] Initiating... "));
        // -- activar módulo wap --
      }

  
  if (WST_ENABLED)
  {
    DEBUG_PRINT(F("[WST] Initiating... "));
    // -- activar módulo wst --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
  }
  if (RAD_ENABLED)
  {
    DEBUG_PRINT(F("[RAD] Initiating... "));
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
    // xTaskCreate(task_rad, "task_rad", ...);
  }
#endif // DEL MCU
// se coloca el cursor en el terminal serial
   #ifdef DEBUG
      char node_id_2=create_unique_id();
      DEBUG_PRINT(node_id_2);
      DEBUG_PRINT(F(" >"));
   #endif
}

void loop()
{
 
  uint8_t rpta=show_debugging_info(vecinos,total_vecinos,rxValue,txValue);
  
}
