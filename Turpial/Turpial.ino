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
#include "packet.h"
#include "incoming.h"


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
}

void loop()
{

}
