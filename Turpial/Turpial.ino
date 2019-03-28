/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include <WiFi.h>
#include <SSD1306.h>
#include <Time.h>
#include <TimeLib.h>
// devices and default settings
#include "hal/hardware.h"
#include "lang/language.h"

#include "memory_def.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "debugging.h"
#include "fonts/DejaVu_Sans_10.h"
#include "fonts/DejaVu_Sans_12.h"
#include "scr_images.h"
#include "screens.h"
#include "radio.h"
#include "bluetooth.h"
using namespace std;

// variables fijas para este demo
// ID unico del nodo
char id_nodo_demo[16] = "TURPIAL.0";   
//char *id_nodo_demo = "TURPIAL.0"; 
char *id_node;// = id_nodo_demo;

#if SCR_ENABLED == true
SSD1306 display(SCR_ADD, SDA_OLED, SCL_OLED, RST_OLED);
#endif

// includes internos
uint8_t total_vecinos = 0;            // cantidad de vecinos del nodo actual
uint8_t total_rutas = 0;              // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes = 0; // cantidad de mensajes en la cola
uint8_t total_nodos_blacklist = 0;    // cantidad de nodos en blacklist

rutas_t routeTable[MAX_ROUTES];
nodo_t vecinos[MAX_NODES];
nodo_t blacklist[MAX_NODES_BLACKLIST];
message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
packet_t Buffer_packet; // packet_t usado como buffer para mensajes incoming y outcoming

uint8_t packet_timeout=30;   // expiration time in seconds of packets

unsigned long tiempo;

// variables para trasmision BLE
std::string rxValue = "";
std::string txValue = "";
char *uid ;
char *msg;
double timemsg;

// variables para trasmision Lora
std::string rxValue_Lora = "";
std::string txValue_Lora = "";

void setup()
{
  uint8_t i;
  
  bool display_enabled = false;
  bool lora_enabled = false;
  bool serial_enabled = false;
  bool wifi_enabled = false;
  
  // se coloca el id_nodo en mayusculas
 // id_nodo_demo=char_to_uppercase(id_nodo_demo, 16);
   //id_node= node_name_char_to_uppercase(id_nodo_demo);
    id_node = id_nodo_demo;
    copy_array_locha(id_nodo_demo, id_node, 16);
    //fin de colocar id_nodo en mayusculas
    
#if defined(DEBUG)
  serial_enabled = true;
#endif
  //
  if (SCR_ENABLED)
  {
    display_enabled = true;
    if (Vext)
    {
      pinMode(Vext, OUTPUT);
    }
    else
    {
      display_enabled = false;
    }
    if (RAD_ENABLED)
    {
      lora_enabled = true;
    }
    else
    {
      lora_enabled = false;
    }
    if (WST_ENABLED)
    {
      wifi_enabled = true;
    }
    else
    {
      wifi_enabled = false;
    }
    if (WAP_ENABLED)
    {
      wifi_enabled = true;
    }
    else
    {
      wifi_enabled = false;
    }

#ifdef DEBUG
    DEBUG_BEGIN(BAUDRATE);
    while (!Serial);
    Serial.setDebugOutput(true);
    delay(2000);
#endif

    rxValue = "";
    txValue = "";

    if (SCR_ENABLED)
    {
      DEBUG_PRINTLN(MSG_SCR_INIT);
      // activar módulo SCR
      // leer NVS,  verificar si existe registro
      // si existe aplicar, si no establecer parametros por defecto.
      // se inicializa el display
      display.init();
      display.flipScreenVertically();
    }

    if (BLE_ENABLED)
    {
      DEBUG_PRINTLN(F("[BLE] Initiating... "));
      xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
    }
    if (WAP_ENABLED)
    {
      DEBUG_PRINT(F("[WAP] Initiating... "));
      // -- activar módulo wap --
    }

    if (WST_ENABLED)
    {
      DEBUG_PRINTLN(F("[WST] Initiating... "));
    }

    if (RAD_ENABLED)
    {
      DEBUG_PRINTLN(F("[RAD] Initiating... "));
      xTaskCreate(task_radio, "task_radio", 2048, NULL, 5, NULL);
    }

    // se coloca el cursor en el terminal serial
    DEBUG_PRINTLN(F("Starting terminal"));
    // se genera el node_id solo si no existe
    if (id_node == "")
    {
      create_unique_id(id_node);
    }
    DEBUG_PRINT(id_node);
    DEBUG_PRINT(F(" >"));

    if (LED_ENABLED)
    {
      pinMode(LED_PIN, OUTPUT);
      digitalWrite(LED_PIN, HIGH);
    }

    // se inicializa el control del tiempo
    tiempo = millis();
  }
} //setup

int pantalla_activa = 1;





void drawFrame5(int16_t x, int16_t y)
{
  String msg_screen_wifi = "Inactive";
  msg_screen_wifi = "Active WAP";
  if (WAP_ENABLED)
  {
    if (WST_ENABLED)
    {
      msg_screen_wifi = "Active WAP/WST";
    }
    msg_screen_wifi = msg_screen_wifi + "-" + MSG_ENABLED;
  }
  else
  {
    msg_screen_wifi = msg_screen_wifi + "-" + MSG_DISABLED;
  }

  msg_screen_wifi = "Active WST";
  if (WST_ENABLED)
  {
    msg_screen_wifi = msg_screen_wifi + "-" + MSG_ENABLED;
  }
  else
  {
    msg_screen_wifi = msg_screen_wifi + "-" + MSG_DISABLED;
  }

  String msg_screen_radio = "Inactive";

  msg_screen_radio = "Active";

  if (RAD_ENABLED)
  {
    msg_screen_radio = msg_screen_radio + "-" + MSG_ENABLED;
  }
  else
  {
    msg_screen_radio = msg_screen_radio + "-" + MSG_DISABLED;
  }

  String msg_screen_ble = "Inactive";

  msg_screen_ble = "Active";
  if (BLE_ENABLED)
  {
    msg_screen_ble = msg_screen_ble + "-" + MSG_ENABLED;
  }
  else
  {
    msg_screen_ble = msg_screen_ble + "-" + MSG_DISABLED;
  }

  drawframe_rows(0, 0, "Services", "Wifi: " + msg_screen_wifi, "Lora: " + msg_screen_radio, "BLE: " + msg_screen_ble, "", "");
}

uint8_t rpta_tmp ;

void loop()
{

  if (millis() - tiempo > SCR_INTERVAL)
  {
    display.clear();
    switch (pantalla_activa)
    {
    case 1:
      drawframe_title_with_2_fields(0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      break;
    case 2:
      drawframe_table_with_4_fields(0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos) + " bytes", "Size:", (String)sizeof(blacklist) + " bytes");
      break;
    case 3:
      drawframe_title_with_2_fields(0, 0, "Routes Locha Mesh", "Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable) + " bytes");
      break;
    case 4:
      drawframe_title_with_2_fields(0, 0, "Outcoming Queue", "Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes) + " bytes");
      break;
    case 5:
      drawFrame5(0, 0);
      break;
    case 6:
      drawFrame_tech(0, 0);
      break;
    default:
      pantalla_activa = 0; //para que aparezca la primera pantalla
      break;
    }

    pantalla_activa++;
    tiempo = millis();

    if (pantalla_activa > 6)
    {
      pantalla_activa = 1;
    }
    display.display();
  }

  if (millis() - tiempo < 0)
  {
    tiempo = millis();
  }

  // se efectua el procesamiento de paquetes entrantes
  packet_processing_incoming();
  // se efectua el procesamiento de paquetes salientes
  packet_processing_outcoming();

  // solo se agrega la consola de comandos cuando se esta compilando para DEBUG
 // #ifdef DEBUG
     rpta_tmp = show_debugging_info(vecinos, total_vecinos);
//  #endif
  

}
