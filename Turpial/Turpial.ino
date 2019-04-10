/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include <WiFi.h>
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
#include "button.h"
#include "screen.h"
#include "radio.h"
#include "bluetooth.h"
#include "update_older_records.h"

using namespace std;

// variables fijas para este demo
// ID unico del nodo
char id_nodo_demo[16] = "TURPIAL.0";


char *id_node;

// includes internos
uint8_t total_vecinos;            // cantidad de vecinos del nodo actual
uint8_t total_rutas;              // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes; // cantidad de mensajes en la cola
uint32_t outcoming_msgs_size;     // tamaño de la cola de mensajes salientes en bytes.
uint8_t total_nodos_blacklist;    // cantidad de nodos en blacklist
uint8_t total_mensajes_waiting;   // cantidad de mensajes en la cola de espera por ACK , reintento u otro estado de espera
uint8_t mensaje_waiting_to_send;   // id del mensaje_waiting para ser reenviado

rutas_t routeTable[MAX_ROUTES];
uint32_t route_table_size = 0;      // tamaño de la tabla de rutas en bytes

nodo_t vecinos[MAX_NODES];
uint32_t vecinos_table_size = 0;    // size of neigbours table

nodo_t blacklist[MAX_NODES_BLACKLIST];
uint32_t blacklist_table_size = 0;  // size of blacklisted nodes table

message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
message_queue_t mensajes_waiting[MAX_MSG_QUEUE];

packet_t Buffer_packet;             // packet_t usado como buffer para mensajes incoming y outcoming
String packet_return_BLE_str = "";  // se usa en los callback para devolver valores hacia el main loop
String packet_return_Lora_str = ""; // se usa en los callback para devolver valores hacia el main loop
String remote_debugging = "";       // se usa para recibir comandos de debugging remote de la app movil via BLE al equipo

uint8_t packet_timeout = 30; // expiration time in seconds of packets

unsigned long tiempo;

bool radio_Lora_receiving;

// variables para trasmision BLE
std::string rxValue = "";
std::string txValue = "";
char *uid;
char *msg;
double timemsg;
char *hash_msg;

// variables para trasmision Lora
std::string rxValue_Lora = "";
std::string txValue_Lora = "";
int Lora_RSSI;
int Lora_SNR;

void setup()
{
  uint8_t i;
  radio_Lora_receiving = false;
  bool display_enabled = false;
  bool lora_enabled = false;
  bool serial_enabled = false;
  bool wifi_enabled = false;
  total_mensajes_salientes = 0;
  total_nodos_blacklist = 0;
  total_mensajes_waiting = 0;
  total_rutas = 0;
  total_vecinos = 0;
  // se coloca el id_nodo en mayusculas
  // id_nodo_demo=char_to_uppercase(id_nodo_demo, 16);
  // id_node= node_name_char_to_uppercase(id_nodo_demo);
  id_node = id_nodo_demo;
  copy_array_locha(id_nodo_demo, id_node, 16);
  // fin de colocar id_nodo en mayusculas

#if defined(DEBUG)
  serial_enabled = true;
#endif
  
#ifdef DEBUG
  DEBUG_BEGIN(BAUDRATE);
  while (!Serial)
    ;
  Serial.setDebugOutput(true);
  delay(2000);
#endif

  // On board led (LED) is enabled??
  if (LED_ENABLED)
  {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
  }
  // On board button (BTN) is enabled?
  if (BTN_ENABLED)
  {
    pinMode(BTN_GPIO, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BTN_GPIO), irq_button, CHANGE);
  }
  // Screen aka SCR is enabled?
  if (SCR_ENABLED)
  {
    DEBUG_PRINT(MSG_SCR);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(MSG_START);
    xTaskCreate(task_screen, "task_screen", 2048, NULL, 6, NULL);
  }

  if (BLE_ENABLED)
  {
    // BLE Server is enabled?
    DEBUG_PRINT(MSG_BLE);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(MSG_START);
    xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
  }

  // WiFi AP aka WAP, is enabled?
  if (WAP_ENABLED)
  {
    DEBUG_PRINT(MSG_WAP);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(MSG_START);
  }

  // WiFi Station aka WST, is enabled?
  if (WST_ENABLED)
  {
    DEBUG_PRINT(MSG_WST);
    DEBUG_PRINT(" ");
    DEBUG_PRINTLN(MSG_START);
  }

  // Radio iface is enabled?
  if (RAD_ENABLED)
  {
    DEBUG_PRINT(MSG_RAD);
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

  
  
    // se crea un task para las tareas de baja prioridad tipo garbage collector  (prioridad 2 por debajo de las otras task)
    // que chequea rutas viejas, paquetes en espera,  vecinos no reportados desde hace mucho tiempo
    xTaskCreate(task_update_older_records, "task_update_older_records", 2048, NULL, 2, NULL);

  
  DEBUG_PRINT(id_node);
  DEBUG_PRINT(F(" >"));

  // se inicializa el control del tiempo
  tiempo = millis();

    // se manda un mensaje por Lora tipo HELLO para que los vecinos lo identifiquen y le hagan JOIN
    packet_t packet_HELLO;
    copy_array_locha(id_node, packet_HELLO.header.from, 16);
    radioSend(packet_serialize(packet_HELLO));

} //setup

// con esta variable se lleva el control de cual frame de pantalla se esta mostrando en el momento
int pantalla_activa = 1;

void loop()
{
  char *packet_str_tmp;
  char remitente[16];
  /*
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
*/
  if (millis() - tiempo < 0)
  {
    tiempo = millis();
  }

  // se efectua el procesamiento de paquetes salientes
  packet_processing_outcoming(mensajes_salientes, total_mensajes_salientes, mensajes_waiting, total_mensajes_waiting);

// solo se agrega la consola de comandos cuando se esta compilando para DEBUG
#ifdef DEBUG
  uint8_t rpta_tmp = show_debugging_info(vecinos, total_vecinos, remote_debugging);
#endif

  if (radio_Lora_receiving)
  {
    delay(20);
    process_Lora_incoming();
  }



  // se verifica si hay que devolver via BLE algun packet
  if (packet_return_BLE_str.length() > 0)
  {
    delay(500); // se hace una pausa antes de devolver para liberar el radio o cualquier otro recurso de menos prioridad que necesite ejecutarse
    Serial.println("devolviendo packet ...");
    Serial.print("recibi:");
    Serial.println(packet_return_BLE_str);

    //  packet_return_BLE_str.toCharArray(packet_str_tmp, packet_return_BLE_str.length());

    packet_t paquet_in_process2 = packet_deserialize_str(packet_return_BLE_str.c_str());
    //   paquet_in_process2.header.type=NOT_DELIVERED;
    // se invierte el remitente con el destinatario
    copy_array_locha(paquet_in_process2.header.from, remitente, 16);
    copy_array_locha(paquet_in_process2.header.to, paquet_in_process2.header.from, 16);
    copy_array_locha(remitente, paquet_in_process2.header.to, 16);

    // se manda por el BLE
    // los type=MSG se pasa solo el payload al BLE, mientras que los demas type se convierte a Json y se devuelve al app para su procesamiento
   // if (paquet_in_process2.header.type=MSG){
   //   txValue = paquet_in_process2.body.payload;
   // } else {
   String text_to_send_to_ble=packet_into_json(paquet_in_process2);
      txValue = text_to_send_to_ble.c_str();
      Serial.print(F("enviado al BLE:"));
      Serial.println(text_to_send_to_ble);
   // }

if (mensaje_waiting_to_send>0){
  void update_older_record();
}

    packet_return_BLE_str = "";
    Serial.println("seliendo del envio hacia el BLE");
  }
}
