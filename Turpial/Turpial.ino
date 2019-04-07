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


char *id_node;

#if SCR_ENABLED == true
  SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);
#endif

// includes internos
uint8_t total_vecinos;            // cantidad de vecinos del nodo actual
uint8_t total_rutas;              // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes; // cantidad de mensajes en la cola
uint8_t total_nodos_blacklist;    // cantidad de nodos en blacklist
uint8_t total_mensajes_waiting;   // cantidad de mensajes en la cola de espera por ACK , reintento u otro estado de espera

rutas_t routeTable[MAX_ROUTES];
nodo_t vecinos[MAX_NODES];
nodo_t blacklist[MAX_NODES_BLACKLIST];
message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
message_queue_t mensajes_waiting[MAX_MSG_QUEUE];

packet_t Buffer_packet; // packet_t usado como buffer para mensajes incoming y outcoming
String packet_return_BLE_str="";  // se usa en los callback para devolver valores hacia el main loop
String packet_return_Lora_str="";  // se usa en los callback para devolver valores hacia el main loop
String remote_debugging="";      // se usa para recibir comandos de debugging remote de la app movil via BLE al equipo

uint8_t packet_timeout=30;   // expiration time in seconds of packets

unsigned long tiempo;

bool radio_Lora_receiving;

// variables para trasmision BLE
std::string rxValue = "";
std::string txValue = "";
char *uid ;
char *msg;
double timemsg;
char* hash_msg;

// variables para trasmision Lora
std::string rxValue_Lora = "";
std::string txValue_Lora = "";

void setup()
{
  uint8_t i;
  radio_Lora_receiving=false; 
  bool display_enabled = false;
  bool lora_enabled = false;
  bool serial_enabled = false;
  bool wifi_enabled = false;
  total_mensajes_salientes=0;
  total_nodos_blacklist = 0;
    total_mensajes_waiting=0;
  total_rutas = 0;
  total_vecinos = 0;
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
      DEBUG_PRINT(MSG_SCR);
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(MSG_START);
      // activar módulo SCR
      // leer NVS,  verificar si existe registro
      // si existe aplicar, si no establecer parametros por defecto.
      // se inicializa el display
      display.init();
      display.flipScreenVertically();
    }

    if (BLE_ENABLED)
    {
     
      DEBUG_PRINT(MSG_BLE);
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(MSG_START);
      xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
    }
    if (WAP_ENABLED)
    {
      DEBUG_PRINT(MSG_WAP);
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(MSG_START);
      // -- activar módulo wap --
    }

    if (WST_ENABLED)
    {
      
      DEBUG_PRINT(MSG_WST);
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(MSG_START);
    }

    if (RAD_ENABLED)
    {
      DEBUG_PRINT(MSG_SCR);
      DEBUG_PRINT(" ");
      DEBUG_PRINTLN(MSG_START);
      xTaskCreate(task_radio, "task_radio", 2048, NULL, 5, NULL);
    }

    // se coloca el cursor en el terminal serial
              
    DEBUG_PRINT(MSG_SERIAL);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(MSG_START);
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

// con esta variable se lleva el control de cual frame de pantalla se esta mostrando en el momento
int pantalla_activa = 1;   


void loop()
{
  char* packet_str_tmp;
  char remitente[16];
  
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

  // se efectua el procesamiento de paquetes salientes
  packet_processing_outcoming(mensajes_salientes,total_mensajes_salientes,mensajes_waiting,total_mensajes_waiting);

  // solo se agrega la consola de comandos cuando se esta compilando para DEBUG
  #ifdef DEBUG
          uint8_t rpta_tmp = show_debugging_info(vecinos, total_vecinos,remote_debugging);
  #endif

  if (radio_Lora_receiving){
    delay(20);
      process_Lora_incoming();
  }

  // se verifica si hay que devolver via BLE algun packet 
  if (packet_return_BLE_str.length()>0){ 
    delay(500);  // se hace una pausa antes de devolver para liberar el radio o cualquier otro recurso de menos prioridad que necesite ejecutarse
    Serial.println("devolviendo packet ...");
    Serial.print("recibi:");
    Serial.println(packet_return_BLE_str);
    
    
    
  //  packet_return_BLE_str.toCharArray(packet_str_tmp, packet_return_BLE_str.length());
    
    packet_t paquet_in_process2=packet_deserialize_str(packet_return_BLE_str.c_str());
 //   paquet_in_process2.header.type=NOT_DELIVERED;
    // se invierte el remitente con el destinatario
    copy_array_locha(paquet_in_process2.header.from, remitente, 16);
    copy_array_locha(paquet_in_process2.header.to, paquet_in_process2.header.from, 16);
    copy_array_locha(remitente, paquet_in_process2.header.to, 16);

    // se manda por el BLE
    txValue=paquet_in_process2.body.payload;
  
   packet_return_BLE_str="";
  Serial.println("seliendo del envio hacia el BLE");
  }

}
