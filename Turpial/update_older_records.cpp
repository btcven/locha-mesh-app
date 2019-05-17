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
extern char* id_node;
extern packet_t Buffer_packet;
extern std::string txValue;
extern std::string rxValue;
// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;

extern uint8_t mensaje_waiting_to_send;   // id del mensaje_waiting para ser reenviado
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;
extern uint8_t total_mensajes_salientes; // cantidad de mensajes en la cola
extern unsigned long tiempo_desde_ultimo_packet_recibido;

extern not_delivered_type_e why_not_delivered;   // causa de no entrega de algun packet
extern bool run_pending_task;

// funciones para el mantenimiento de las colas de mensaje:
// se reenvian packets en espera
//se eliminan y mandan notificaciones de packets que hagan timeout

uint8_t pending_tasks(message_queue_t (&mensajes_waiting)[MAX_MSG_QUEUE],uint8_t &total_mensajes_waiting,message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE],uint8_t &total_mensajes_salientes){

 bool enviar=true;

 // 1) se verifica si los mensajes en espera hay alguno que deba reenviarse
    if (total_mensajes_waiting>0){ 
         Serial.println(F("Voy a enviar un packet waiting"));
        if ((mensajes_waiting[1].paquete.header.type==MSG)or(mensajes_waiting[1].paquete.header.type==TXN)){
                    // para los MSG y TXN se actualiza los retries
                         mensajes_waiting[1].retries=mensajes_waiting[1].retries++;
                         mensajes_waiting[1].retry_timestamp=millis();
                         Serial.print("cantidad de reintentos:");
                         Serial.print((String)mensajes_waiting[1].retries);
                         // si llega al limite de retries hay que mandar un packet not delivered al originante
                         if (mensajes_waiting[1].retries>MSG_QUEUE_WAITING_MAX_RETRIES){ 
                              Serial.println(F("Se alcanzo el limite de retintentos de envio"));
                              devolver_como_packet_not_delivered(mensajes_waiting[1].paquete,MAX_RETRIES_REACHED);
                              
                              enviar=false;
                          }
        }
        total_mensajes_salientes++;
        mensajes_salientes[total_mensajes_salientes]=mensajes_waiting[1];
         if ((mensajes_waiting[1].paquete.header.type!=MSG)and(mensajes_waiting[1].paquete.header.type!=TXN)){
                for ( uint8_t yy = 1; yy < total_mensajes_waiting; yy++) {
                    mensajes_waiting[yy]=mensajes_waiting[yy+1];
                }
                total_mensajes_waiting--;
          }
        
    }

// 2) TODO: chequea rutas viejas: si hay que mandar un paquete route a ver si todavia esta activa la ruta
    // 3) TODO: vecinos no reportados desde hace mucho , mandarle un paquete route a ver si responden

    // 4) cada x minutos hacer un HELLO para actualizar cualquier nuevo nodo/ruta
    if (millis()-tiempo_desde_ultimo_packet_recibido>HELLO_RETRY_TIMEOUT){
        DEBUG_PRINT(F("Sending AUTOHELLO..."));
     
        packet_t packet_temporal=construct_packet_HELLO(id_node);
    
        String rpta_str=packet_serialize(packet_temporal);
       // DEBUG_PRINT(F("total de mensajes saliente:"));
         
       // DEBUG_PRINTLN((String)total_mensajes_salientes);
        uint8_t rptad=packet_to_send(construct_packet_HELLO(id_node),mensajes_salientes,total_mensajes_salientes,total_vecinos, total_rutas);  // se envia a la cola de mensajes salientes
      //  DEBUG_PRINT(F("total de mensajes saliente despues de packet_to_send:"));
         
      //  DEBUG_PRINTLN((String)total_mensajes_salientes);
        tiempo_desde_ultimo_packet_recibido=millis();
    }
    run_pending_task=false;
    return 0;
}


// se maneja el mantenimiento de las tablas de mensajes y vecinos por medio de un callback en un xTask FreeRTOS de menor prioridad
void task_update_older_records(void *params) {
uint8_t xx;
uint8_t jj;

 while (1) {
  run_pending_task=true;
    delay(HELLO_RETRY_TIMEOUT);
  }
  
}
