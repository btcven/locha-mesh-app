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
#ifdef ARDUINO_ARCH_ESP32
    #define MCU_ESP32
#endif 
#ifdef WIFI_LORA_32_V2
    #define MCU_ESP32
#endif 
#ifdef ARDUINO_WIFI_LORA_32_V2
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
int total_vecinos = 0; // cantidad de vecinos del nodo actual
int total_rutas = 0;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
rutas_t routeTable[255];
nodo_t vecinos[255];
message_queue_t mensajes_salientes[255];
int total_mensajes_salientes = 0;

// funcion para llenar manualmente los datos del modelo demo en la tabla vecinos y rutas
void iniciar_vecinos_y_rutas(char *id_nodo, nodo_t vecinos[255], rutas_t routeTable[255])
{
  if (id_nodo == "turpial.0")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_actual.id = id_nodo;
    nodo_vecino.id = "turpial_1";  // agregarle nodo_vecino.id = "turpial_0".c_str();  // para corregir warnings del compilado
    create_neighbor(nodo_vecino.id);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
   
  }
  if (id_nodo == "turpial.1")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_t nodo_vecino2;
    nodo_actual.id = id_nodo;
    nodo_vecino.id = "turpial_0";
    nodo_vecino2.id = "turpial_2";
    create_neighbor(nodo_vecino.id);
    create_neighbor(nodo_vecino2.id);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
    // ruta T2
    create_route(nodo_actual, nodo_vecino2, nodo_vecino2);
  }
  if (id_nodo == "turpial.2")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_actual.id = id_nodo;
    nodo_vecino.id = "turpial_1";
    create_neighbor(nodo_vecino.id);
    // ruta T2
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
  }
}

void setup()
{
  DEBUG_BEGIN(BAUDRATE);
  
#ifdef MCU_ESP32
      if (SCR_ENABLED)
      {
        DEBUG_PRINT("[SRC] Initiating... ");
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
          DEBUG_PRINTLN("OK");
          display.flipScreenVertically();
          display.setTextAlignment(TEXT_ALIGN_LEFT);
          display.setFont(ArialMT_Plain_10);
          display.drawString(0, 0, "Starting ..");
          display.display();
        }
      }
      
      
      if (BLE_ENABLED)
      {
        DEBUG_PRINT("[BLE] Initiating... ");
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
        DEBUG_PRINT("[WAP] Initiating... ");
        // -- activar módulo wap --
      }

  #endif // DEL MCU
  if (WST_ENABLED)
  {
    DEBUG_PRINT("[WST] Initiating... ");
    // -- activar módulo wst --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
  }
  if (RAD_ENABLED)
  {
    DEBUG_PRINT("[RAD] Initiating... ");
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
    // xTaskCreate(task_rad, "task_rad", ...);
  }

  iniciar_vecinos_y_rutas(id_node, vecinos, routeTable);
}

void loop()
{
  mostrar_vecinos(id_node,vecinos);
  mostrar_rutas(id_node,routeTable);
  delay(5000);
}
