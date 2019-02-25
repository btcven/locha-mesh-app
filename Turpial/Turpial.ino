/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include "defaultStartup.h"
#include "Configuration.h"
#include "general_functions.h"
#include "lora.h"

#include <WiFiUdp.h>  // using websockets

#include <TaskScheduler.h>  // https://github.com/arkhipenko/TaskScheduler



SCR_status_t  SCRStatus; // Screen Oled/lcd status
BLE_status_t  BLEStatus; // BLE status
WST_status_t  WSTStatus; // Wifi station client status
WAP_status_t  WAPStatus; // Wifi Access Point  status
RAD_status_t  RADStatus; // Radio status (acually Lora radio, but could works with other radios)
BAT_status_t  SCRStatus; // Battery status

// declare scheduler
Scheduler runner;

// watchdog 
unsigned long wdt;

// prototype voids to be scheduled
void scan_lora();
void scan_wifi();

// list of scheduled task
//Task t1(20000, TASK_ONCE, &void_to_execute);  // se puede colocar muchos tipos de TASK, en este caso TASK_ONCE,TASK_FOREVER,TASK_IMMEDIATE o 2,3,4,5... es el numero de veces que se ejecuta, tambien se puede colcoar infinitas veces
Task task_lora(30000, TASK_FOREVER, &scan_lora);  // se coloca cada 30 segundos por default, a tiempo de ejecucion se cambia segun lo que exista en el EPPROM colocado por el usuario
Task task_wifi(60000, TASK_FOREVER, &scan_wifi);  // se coloca cada 60 segundos por default, a tiempo de ejecucion se cambia segun lo que exista en el EPPROM colocado por el usuario


void scan_lora(){
// update lora neighbors
  // TODO
}
void scan_wifi(){
// update wifi neighbors
  // TODO
}

void setup() {
  Serial.begin(BAUDRATE);

  read_epprom_variables();
  
  // unique id to identify turpial node
  create_unique_id();
  
  // Screen (aka SCR) active on boot?
  if (SCR_ENABLED) {
    SCRStatus.isEnabled = true;
    Serial.printf(MSG_SCR_ALLOCATING);
    if (screen_init()) {
      SCRStatus.isActive = true;
      Serial.printf(MSG_OK);
    } else {
      SCRStatus.isActive = false;
      Serial.printf(MSG_ERROR);
    }
  }

  // BLE iface active on boot?
  if (BLE_ENABLED) {
    BLEStatus.isEnabled = true;
    // este task no esta casado a ningun core
    xTaskCreate(bluetooth_task, "bluetooth_task", 4096, NULL, 5, NULL);
  }

  // RAD iface active on boot?
  if (RAD_ENABLED) {
     startup_lora();
      start_receive_lora_packets();
      // start scheduled jobs
      //runner.init();
      // scheduled control jobs
      runner.addTask(task_lora);
  }

  // WAP iface active on boot?
  if (WAP_ENABLED) {
     // call to function start or task
  }

  // WST iface active on boot?
  if (WST_ENABLED) {
    // start scheduled jobs
    //runner.init();
    // scheduled control jobs
    runner.addTask(task_wifi);
  }

// for watchdog 
 setWD();
 xTaskCreatePinnedToCore(watchDog, "watchdog", 512, NULL, 1, NULL, 0);
}

// main loop for app
void loop() { 
    runner.execute();
    unsigned long now = millis();

    // TODO check if deep sleep/wakeup enabled based on ESP32 UL coprocessor
    
}
