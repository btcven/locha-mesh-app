/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

// declaracion de librerias
#include <Arduino.h>
#include "update_older_records.h"
#include "packet.h"
#include "memory_def.h"
#include <string.h>
#include <Time.h>
#include <TimeLib.h>
#include "radio.h"
#include "general_functions.h"
#include "debugging.h"

//declaracion de variables
extern char *id_node;
extern packet_t Buffer_packet;
extern std::string txValue;
extern std::string rxValue;
// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;

extern uint8_t mensaje_waiting_to_send; // id del mensaje_waiting para ser reenviado
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;
extern uint8_t total_mensajes_salientes; // cantidad de mensajes en la cola
extern unsigned long tiempo_desde_ultimo_packet_recibido;

extern not_delivered_type_e why_not_delivered; // causa de no entrega de algun packet
extern bool run_pending_task;

// funciones para el mantenimiento de las colas de mensaje:
// se reenvian packets en espera
//se eliminan y mandan notificaciones de packets que hagan timeout

uint8_t pending_tasks(message_queue_t (&mensajes_waiting)[MAX_MSG_QUEUE], uint8_t &total_mensajes_waiting)
{
  // se hace el reenvio de un mensaje waiting en la cola de mensajes_waiting
  if (total_mensajes_waiting > 0)
  {
    String msg_to_send_now = packet_serialize(mensajes_waiting[1].paquete);
    radioSend(msg_to_send_now.c_str());
    // se coloca el radio nuevamente en modo receives (se hace por segunda vez porque detectamos algunos casos en donde el radio no cambio de modo dentro del radioSend()
    LoRa.receive();
    if ((mensajes_waiting[1].paquete.header.type != MSG) and (mensajes_waiting[1].paquete.header.type != TXN))
    {
      for (uint8_t yy = 1; yy < total_mensajes_waiting; yy++)
      {
        mensajes_waiting[yy] = mensajes_waiting[yy + 1];
      }
      total_mensajes_waiting--;
    }
    else
    {
      // para los MSG y TXN se actualiza los retries
      mensajes_waiting[1].retries = mensajes_waiting[1].retries++;
      mensajes_waiting[1].retry_timestamp = millis();

      // si llega al limite de retries hay que mandar un packet not delivered al originante
      if (mensajes_waiting[1].retries > MSG_QUEUE_WAITING_MAX_RETRIES)
      {

        devolver_como_packet_not_delivered(mensajes_waiting[1].paquete, MAX_RETRIES_REACHED);
        for (uint8_t yy = 1; yy < total_mensajes_waiting; yy++)
        {
          mensajes_waiting[yy] = mensajes_waiting[yy + 1];
        }
        total_mensajes_waiting--;
      }
    }
  }
  run_pending_task = false;
  return 0;
}

// se maneja el mantenimiento de las tablas de mensajes y vecinos por medio de un callback en un xTask FreeRTOS de menor prioridad
void task_update_older_records(void *params)
{

  while (1)
  {
    run_pending_task = true;
    // 1) paquetes en espera: tiempo delayed o si tienen que reintentar
    // 2) chequea rutas viejas: si hay que mandar un paquete route a ver si todavia esta activa la ruta
    // 3) vecinos no reportados desde hace mucho , mandarle un paquete route a ver si responden
    // 4) cada x minutos hacer un HELLO para actualizar cualquier nuevo nodo/ruta
    if (millis() - tiempo_desde_ultimo_packet_recibido > HELLO_RETRY_TIMEOUT)
    {
      ESP_LOGD("PROTO", "trying to discover new routes");

      packet_t packet_temporal;
      packet_temporal = construct_packet_HELLO(id_node);

      String rpta_str = packet_serialize(packet_temporal);

      uint8_t rpta_rad = radioSend(rpta_str);

      // se coloca el radio nuevamente en modo receives (se hace por segunda vez porque detectamos algunos casos en donde el radio no cambio de modo dentro del radioSend()
      LoRa.receive();
      tiempo_desde_ultimo_packet_recibido = millis();
    }
    // se aumenta el delay ya que este callback es de baja prioridad y no debe interferir en la recepcion de otros packets
    delay(HELLO_RETRY_TIMEOUT);
  }
}
