/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#include <Arduino.h>
#include <string>
#include <WiFi.h>
#include <Time.h>
#include <TimeLib.h>
#include "dummy.h"
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

// NID
char id_nodo_demo[SIZE_IDNODE] = "";

char *id_node;

uint8_t total_vecinos;               // cantidad de vecinos del nodo actual
uint8_t total_rutas;                 // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes;    // cantidad de mensajes en la cola
uint8_t outcoming_msgs_size;         // tamaño de la cola de mensajes salientes en bytes.
uint8_t total_nodos_blacklist;       // cantidad de nodos en blacklist
uint8_t total_rutas_blacklist;       // cantidad de nodos en blacklist
uint8_t total_mensajes_waiting;      // cantidad de mensajes en la cola de espera por ACK , reintento u otro estado de espera
uint8_t mensaje_waiting_to_send = 0; // id del mensaje_waiting para ser reenviado

rutas_t routeTable[MAX_ROUTES];
uint8_t route_table_size = 0; // tamaño de la tabla de rutas en bytes

nodo_t vecinos[MAX_NODES];
uint32_t vecinos_table_size = 0; // size of neigbours table

nodo_t blacklist_nodes[MAX_NODES_BLACKLIST];
uint32_t total_blacklist_nodes = 0; // size of blacklisted nodes table

rutas_blacklisted_t blacklist_routes[MAX_NODES_BLACKLIST];
uint32_t total_blacklist_routes = 0; // size of blacklisted routes table

message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
message_queue_t mensajes_waiting[MAX_MSG_QUEUE];

packet_t Buffer_packet;             // packet_t usado como buffer para mensajes incoming y outcoming
String packet_return_BLE_str = "";  // se usa en los callback para devolver valores hacia el main loop
String packet_return_Lora_str = ""; // se usa en los callback para devolver valores hacia el main loop
String remote_debugging = "";       // se usa para recibir comandos de debugging remote de la app movil via BLE al equipo

not_delivered_type_e why_not_delivered = EMPTY_NOT_DELIVERED; // causa de no entrega de algun packet

uint8_t packet_timeout = 30; // expiration time in seconds of packets

unsigned long tiempo;
unsigned long tiempo_desde_ultimo_packet_recibido = 0;
bool run_pending_task = false;

bool radio_Lora_receiving;

// variables para BLE
std::string rxValue;
std::string txValue;
char *uid;
char *msg;
double timemsg;
char *hash_msg;

// Radio variables
std::string rxValue_Lora;
std::string txValue_Lora;
int Lora_RSSI;
int Lora_SNR;

void setup()
{
 
  char *pChar = (char *)"";
  radio_Lora_receiving = false;
 
  total_mensajes_salientes = 0;
  total_nodos_blacklist = 0;
  total_rutas_blacklist = 0;
  total_mensajes_waiting = 0;
  total_rutas = 0;
  total_vecinos = 0;
  // se coloca el id_nodo en mayusculas
  // id_nodo_demo=char_to_uppercase(id_nodo_demo, SIZE_IDNODE);
  // id_node= node_name_char_to_uppercase(id_nodo_demo);
  id_node = id_nodo_demo;
  copy_array_locha(id_nodo_demo, id_node, SIZE_IDNODE);
  // fin de colocar id_nodo en mayusculas

#if defined(DEBUG)
  //serial_enabled = true;
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
    ESP_LOGD("SCR", "Starting SCR..");
    xTaskCreatePinnedToCore(task_screen, "task_screen", 4096, NULL, 3, NULL, 1);
  }

  if (BLE_ENABLED)
  {
    // BLE Server is enabled?
    ESP_LOGD("BLE", "Starting BLE..");
    xTaskCreatePinnedToCore(task_bluetooth, "task_bluetooth", 8192, NULL, 5, NULL, 1);
  }

  // WiFi AP aka WAP, is enabled?
  if (WAP_ENABLED)
  {
    ESP_LOGD("WAP", "Starting WAP..");
  }

  // WiFi Station aka WST, is enabled?
  if (WST_ENABLED)
  {
    ESP_LOGD("WST", "Starting WST..");
  }

  // Radio iface is enabled?
  if (RAD_ENABLED)
  {
    ESP_LOGD("RAD", "Starting RAD..");
    xTaskCreatePinnedToCore(task_radio, "task_radio", 8192, NULL, 5, NULL, 1);
  }

  // se genera el node_id solo si no existe
  if (compare_char(id_node, pChar))
  {
    String id_tmp = get_id_mac();
    strcpy(id_node, id_tmp.c_str());
  }

  xTaskCreatePinnedToCore(task_update_older_records, "task_update_older_records", 4096, NULL, 2, NULL, 1);

  tiempo = millis();
  ESP_LOGD("PROTO", "[PROTOCOL] Sending Hello...");
  uint8_t rptad = packet_to_send(construct_packet_HELLO(id_node), mensajes_salientes, total_mensajes_salientes);
  if (rptad>0){
    // si recibe cualquier cosa distinta de 0 es que sucedio algo dentro del void
    ESP_LOGD("SETUP", "[PROTOCOL] error inside packet_to_send ...");
  }
  LoRa.receive();
} //setup

void loop()
{
 

  if (millis() - tiempo < 0)
  {
    tiempo = millis();
  }
  packet_processing_outcoming(mensajes_salientes, total_mensajes_salientes, mensajes_waiting, total_mensajes_waiting);

// solo se agrega la consola de comandos cuando se esta compilando para DEBUG
#ifdef DEBUG
  uint8_t rpta_tmp = show_debugging_info(vecinos, total_vecinos, remote_debugging);
  if (rpta_tmp==0){
    ESP_LOGD("Setup", "No pudo ser procesado show_debugging_info");
  }
#endif

  if (radio_Lora_receiving)
  {
    process_Lora_incoming(vecinos, total_vecinos);
  }

  // se verifica si hay que devolver via BLE algun packet
  if (packet_return_BLE_str.length() > 0)
  {

    packet_t paquet_in_process2 = packet_deserialize_str(packet_return_BLE_str.c_str());

    if ((paquet_in_process2.header.type == MSG) or (paquet_in_process2.header.type == TXN))
    {
      ESP_LOGD("PROTO", "[PROTOCOL] incoming data..");

      devolver_como_packet_not_delivered(paquet_in_process2, BLE_NOT_CONNECTED);

      String text_to_send_to_ble = packet_into_json(paquet_in_process2, "err");

      txValue.assign(text_to_send_to_ble.c_str());
    }

    packet_return_BLE_str = "";
  }

  if (run_pending_task)
  {
    uint8_t rpta_task = pending_tasks(mensajes_waiting, total_mensajes_waiting);
    if (rpta_task>0){
       ESP_LOGD("Setup", "Fail processing pending_tasks");
    }
  }
}
