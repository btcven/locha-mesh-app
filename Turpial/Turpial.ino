/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <LoRaLib.h>
#include <Wire.h>
#include <string.h>
using namespace std;
// detecting MCU compiler
#if defined(__AVR__)
    #define MCU_ARDUINO
#endif 
// definicion para ESP32 generico
#ifdef ARDUINO_ARCH_ESP32
    #define MCU_ESP32
#endif 
// definiciones para HELTEC
#ifdef WIFI_LORA_32_V2
    #define MCU_ESP32
#endif 
#ifdef ARDUINO_WIFI_LORA_32_V2
    #define MCU_ESP32
#endif
#ifdef WIFI_KIT_32
    #define MCU_ESP32
#endif
#ifdef WIFI_LORA_32
    #define MCU_ESP32
#endif
#ifdef WIRELESS_STICK
    #define MCU_ESP32
#endif


// devices and default settings
#include "hardware.h"
#ifdef MCU_ESP32
  #include "SSD1306.h"
  #include "bluetooth.h"
  #include "screen.h"
#endif 
#include "packet.h"
#include "route.h"
#include "debugging.h"


#ifdef MCU_ESP32
  SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST, GEOMETRY_128_64); // tambien esta GEOMETRY_128_64 pero depende del screen del HELTEC
#endif 

// variables fijas para este demo
char *id_node = "turpial.0";


// includes internos
uint8_t total_vecinos = 0; // cantidad de vecinos del nodo actual
uint8_t total_rutas = 0;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes = 0;   // cantidad de mensajes en la cola
rutas_t routeTable[255];
nodo_t vecinos[255];
message_queue_t mensajes_salientes[255];

void setup()
{
  
  DEBUG_BEGIN(BAUDRATE);

 
#ifdef MCU_ESP32
      if (SCR_ENABLED)
      {
        DEBUG_PRINT(F("[SRC] Initiating... "));
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
          DEBUG_PRINTLN(F("OK"));
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
      char* node_id_2=create_unique_id();
      DEBUG_PRINT(node_id_2);
      DEBUG_PRINT(F(" >"));
   #endif
}

void loop()
{
  show_debugging_info();
  Serial.print("el vecino:");
                Serial.println((String)vecinos[total_vecinos].id);
                Serial.print("total vecinos:");
                Serial.println(total_vecinos);
                delay(5000);
}
