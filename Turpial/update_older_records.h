/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef UPDATE_OLDER_RECORDS_H 
#define UPDATE_OLDER_RECORDS_H

#include <Arduino.h>
#include "memory_def.h"
#include "general_functions.h"
#include "bluetooth.h"
#include "radio.h"
#include "route.h"
#include "debugging.h"

extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes;

void task_update_older_records(void *params);
uint8_t pending_tasks(message_queue_t (&mensajes_waiting)[MAX_MSG_QUEUE],uint8_t &total_mensajes_waiting,message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE],uint8_t &total_mensajes_salientes);

#endif
