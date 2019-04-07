/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include "update_older_records.h"
#include "packet.h"
#include <string.h> 
#include <Time.h>
#include <TimeLib.h>
#include "general_functions.h"
#include "debugging.h"

extern char* id_node;
extern packet_t Buffer_packet;

extern std::string txValue;
extern std::string rxValue;

// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;

extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;
extern uint8_t total_mensajes_salientes; // cantidad de mensajes en la cola


void task_update_older_records(void *params) {
uint8_t xx;
uint8_t jj;

 while (1) {
    // 1) paquetes en espera: tiempo delayed o si tienen que reintentar
    // se recorre la tabla de mensajes_waiting
    if (total_mensajes_waiting>0){
     //  DEBUG_PRINT(F("Waiting MSG present"));
      for ( xx = 0; xx <= total_mensajes_waiting; xx++) {
          if ((mensajes_waiting[xx].retry_timestamp+MSG_QUEUE_WAITING_MAX_AGE)>millis()){
            // el paquete vencio se vuelve a hacer un retry
              if (mensajes_waiting[xx].retries>=MSG_QUEUE_WAITING_MAX_RETRIES){ 
                // no pudo ser entregado
                txValue="NOT DELIVERED";
                //  se borra de la cola de espera
                //  for ( jj = xx; jj < total_mensajes_waiting; jj++) {
                //      mensajes_waiting[jj]=mensajes_waiting[jj+1];                       
                //  }
                //  total_mensajes_waiting--;
                break;    
              } else {
                // se hace un retry de envio
                if (total_mensajes_salientes<MAX_MSG_QUEUE){ 
                  //  mensajes_waiting[xx].retries++;
                  //  mensajes_waiting[xx].retry_timestamp=millis();
                    
                  //  mensajes_salientes[total_mensajes_salientes+1]=mensajes_waiting[xx];
                    //for ( jj = xx; jj < total_mensajes_waiting; jj++) {
                  //    mensajes_waiting[jj]=mensajes_waiting[jj+1];                       
                  //}
                 // total_mensajes_waiting--;
                } else {
                  // no se puede reenviar esta full la cola de mensajes, se debe esperar un poco mas para reintentar
                  // se actualiza el retry_timestamp para que tenga otro ciclo mas de tiempo mientras se libra la cola
                  mensajes_waiting[xx].retry_timestamp=millis();
                  
                }
              }
          }
          
      }
    }
    
    // 2) chequea rutas viejas: si hay que mandar un paquete route a ver si todavia esta activa la ruta
    // 3) vecinos no reportados desde hace mucho , mandarle un paquete route a ver si responden
    
    delay(40);
  }
}
