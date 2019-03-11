/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <LoRaLib.h>
#include <Wire.h>
#include <SSD1306.h>

// devices and default settings
#include "hardware.h"
#include "screen.h"

SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);
//OLEDDisplayUi ui(&display);

// includes internos
char* id_node="turpial.0";
rutas_t routeTable[3];
nodo_t vecinos[3];


#include "packet.h"
#include "incoming.h"

  void iniciar_vecinos_y_rutas(char* id_nodo,nodo_t vecinos[3],rutas_t routeTable[3] ){
    if (id_nodo=="turpial.0"){
        nodo_t nodo_actual;
        nodo_t nodo_vecino;
        nodo_actual.id=id_nodo;
        nodo_vecino.id="turpial_1";
        vecinos[1]=nodo_vecino;
        rutas_t ruta_disponible;
        ruta_disponible.origen=nodo_actual;
        ruta_disponible.next_neighbor=nodo_vecino;
        ruta_disponible.destino=nodo_vecino;
        ruta_disponible.age=millis();
        routeTable[1]=ruta_disponible;
    }
     if (id_nodo=="turpial.1"){
        nodo_t nodo_actual;
        nodo_t nodo_vecino;
        nodo_t nodo_vecino2;

        nodo_actual.id=id_nodo;
        nodo_vecino.id="turpial_0";
        nodo_vecino2.id="turpial_2";
        vecinos[1]=nodo_vecino;
        vecinos[2]=nodo_vecino2;
        // ruta T1
        rutas_t ruta_disponible;
        ruta_disponible.origen=nodo_actual;
        ruta_disponible.next_neighbor=nodo_vecino;
        ruta_disponible.destino=nodo_vecino;
        ruta_disponible.age=millis();
        routeTable[1]=ruta_disponible;
          // ruta T2
        rutas_t ruta_disponible2;
        ruta_disponible2.origen=nodo_actual;
        ruta_disponible2.next_neighbor=nodo_vecino2;
        ruta_disponible2.destino=nodo_vecino2;
        ruta_disponible2.age=millis();
        routeTable[2]=ruta_disponible;
        
    }
     if (id_nodo=="turpial.2"){
        nodo_t nodo_actual;
        nodo_t nodo_vecino;
        nodo_actual.id=id_nodo;
        nodo_vecino.id="turpial_1";
        vecinos[1]=nodo_vecino;
        rutas_t ruta_disponible;
        ruta_disponible.origen=nodo_actual;
        ruta_disponible.next_neighbor=nodo_vecino;
        ruta_disponible.destino=nodo_vecino;
        ruta_disponible.age=millis();
        routeTable[1]=ruta_disponible;
    }
  }
  
  

void setup()
{
  Serial.begin(BAUDRATE);
  if (SCR_ENABLED)
  {
    Serial.print("[SRC] Initiating... ");
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
      Serial.println("OK");
      display.flipScreenVertically();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.setFont(ArialMT_Plain_10);
      display.drawString(0, 0, "[SCR] OK");
      display.display();
    }
  }
  if (BLE_ENABLED)
  {
    Serial.print("[BLE] Initiating... ");
    // -- activar módulo ble --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- activar la tarea
    // xTaskCreate(task_ble, "task_ble", ...);
  }
  if (WAP_ENABLED)
  {
    Serial.print("[WAP] Initiating... ");
    // -- activar módulo wap --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
    //
  }
  if (WST_ENABLED)
  {
    Serial.print("[WST] Initiating... ");
    // -- activar módulo wst --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
  }
  if (RAD_ENABLED)
  {
    Serial.print("[RAD] Initiating... ");
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
    // xTaskCreate(task_rad, "task_rad", ...);
  }


  iniciar_vecinos_y_rutas();
 
}

void loop()
{

}
