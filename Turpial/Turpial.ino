/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h>
#include <Wire.h>
#include "SSD1306.h"


// devices and default settings
#include "hardware.h"
#include "bluetooth.h"
#include "radio.h"
#include "screen.h"
#include "packet.h"
#include "route.h"

SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST, GEOMETRY_128_64); // tambien esta GEOMETRY_128_64 pero depende del screen del HELTEC

// variables fijas para este demo
char id_node[] = "turpial.00000000";

int total_vecinos = 1; // cantidad de vecinos del nodo actual
int total_rutas = 1;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)

// tabla de rutas
rutas_t routeTable[255];

// tabla de vecinos
nodo_t vecinos[255];

// cola de mensajes salientes.
message_queue_t mensajes_salientes[255];

int total_mensajes_salientes = 0;

// funcion para llenar manualmente los datos del modelo demo en la tabla vecinos y rutas
void iniciar_vecinos_y_rutas(char id_nodo[16], nodo_t vecinos[], rutas_t routeTable[]) {
  if (id_nodo = "turpial.00000000")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    strcpy(nodo_actual.id, id_nodo);
    strcpy(nodo_vecino.id, "turpial_0000001");

    vecinos[1] = nodo_vecino;

    rutas_t ruta_disponible;
    ruta_disponible.origen = nodo_actual;
    ruta_disponible.next_neighbor = nodo_vecino;
    ruta_disponible.destino = nodo_vecino;
    ruta_disponible.age = millis();
    routeTable[1] = ruta_disponible;
  }
  if (id_nodo == "turpial.00000001")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_t nodo_vecino2;
    strcpy(nodo_actual.id, id_nodo);
    strcpy(nodo_vecino.id, "turpial_0000000");
    strcpy(nodo_vecino2.id, "turpial_0000002");

    vecinos[1] = nodo_vecino;
    vecinos[2] = nodo_vecino2;
    // ruta T1
    rutas_t ruta_disponible;
    ruta_disponible.origen = nodo_actual;
    ruta_disponible.next_neighbor = nodo_vecino;
    ruta_disponible.destino = nodo_vecino;
    ruta_disponible.age = millis();
    routeTable[1] = ruta_disponible;
    // ruta T2
    rutas_t ruta_disponible2;
    ruta_disponible2.origen = nodo_actual;
    ruta_disponible2.next_neighbor = nodo_vecino2;
    ruta_disponible2.destino = nodo_vecino2;
    ruta_disponible2.age = millis();
    routeTable[2] = ruta_disponible;
  }
  if (id_nodo == "turpial.0000002")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;

    strcpy(nodo_actual.id, id_nodo);
    strcpy(nodo_vecino.id, "turpial_0000001");

    vecinos[1] = nodo_vecino;
    rutas_t ruta_disponible;
    ruta_disponible.origen = nodo_actual;
    ruta_disponible.next_neighbor = nodo_vecino;
    ruta_disponible.destino = nodo_vecino;
    ruta_disponible.age = millis();
    routeTable[1] = ruta_disponible;
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
      display.drawString(0, 0, "Starting ..");
      display.display();
    }
  }
  if (BLE_ENABLED)
  {
    Serial.print("[BLE] Initiating... \n");
    // -- activar módulo ble --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- activar la tarea
    xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
  }
  if (WAP_ENABLED)
  {
    Serial.print("[WAP] Initiating... ");
    // -- activar módulo wap --
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
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
    xTaskCreate(task_radio, "task_radio", 2048, NULL, 5, NULL);
  }

  iniciar_vecinos_y_rutas(id_node, vecinos, routeTable);
}

void loop()
{
  while (Serial.available()) {

  }
}
