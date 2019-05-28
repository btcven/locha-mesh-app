/**
   @file tasks.h
   @author locha.io project developers (dev@locha.io)
   @brief
   @version 0.1
   @date 2019-04-24

   @copyright Copyright (c) 2019 locha.io project developers
   @license MIT license, see LICENSE file for details

*/
#include <Arduino.h>
#include "hal/hardware.h"
#include "memory_def.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "NVS.h"
#include "WAP.h"
#include "RAD.h"
#include "tasks.h"
#include "general_utils.h"
#include "packet.h"



/**
 * @brief Handler for Auto Hello
 * 
 */
TaskHandle_t AUTO_HELLO_Handler;


// variable for Auto hello timing control
unsigned long time_since_last_auto_hello = millis();





void AUTO_HELLO(void *params) {
   // received params struct
  AData *prt2_param_received;
  prt2_param_received = (AData *) params;
  

  char *pChar = (char *)"";
  char *packet_in_char = (char *)"";
const char *TAG = "AUTO_HELLO";
ESP_LOGD(TAG, "Inside AUTO HELLO id_node:%s",prt2_param_received->id_node);

  while (1) {
   
    ESP_LOGD(TAG, "Tengo id_node inside AUTO HELLO:%s",prt2_param_received->id_node);
    if (millis() < time_since_last_auto_hello) {
      time_since_last_auto_hello = millis();
    }
    if ((millis() - time_since_last_auto_hello) > HELLO_RETRY_TIMEOUT) {
      packet_t packet_hello;
      packet_hello = construct_packet_HELLO( prt2_param_received->id_node);
      ESP_LOGD(TAG, "Sending packet HELLO from:%s",packet_hello.header.from);
      ESP_LOGD(TAG, "Sending packet HELLO to:%s",packet_hello.header.to);
      ESP_LOGD(TAG, "Sending packet HELLO size:%d",sizeof(packet_hello));
      ESP_LOGD(TAG, "Sending packet HELLO next:%s",packet_hello.header.next_neighbor);
      ESP_LOGD(TAG, "packet HELLO already construct ...");
      uint8_t bytearr[sizeof(packet_hello)];
      packet_to_uint8_t(bytearr, packet_hello, sizeof(packet_hello));
      ESP_LOGD(TAG, "packet HELLO already sending now...");
      bool rpta = radioSend(bytearr,sizeof(packet_hello));
      if (rpta) {
        ESP_LOGD(TAG, "Packet HELLO sent to database OK");
      } else {
        ESP_LOGD(TAG, "Error sending Packet HELLO to database");
      }
//      delete packet_hello;
 //     delete bytearr;
      
    }
    
    ESP_LOGD(TAG, "justo antes de salir id_node_inside:%s",prt2_param_received->id_node);
    delay(50);
  }
}

/*
  extern WiFiServer server;
*/
void NetworkPeer(void *params)
{
  // received params struct
  AData *prt_param_received;
  prt_param_received = (AData *) params;

  const char *TAG = "NetworkPeer";
  ESP_LOGD(TAG, "Inside NetworkPeer id_node:%s",prt_param_received->id_node);
  ESP_LOGD(TAG, "Starting AUTO HELLO");

     // params for task AUTO HELLO
   xData xData_to_send_to_AUTO_HELLO;
   strcpy(xData_to_send_to_AUTO_HELLO.id_node,prt_param_received->id_node);
   
  xTaskCreatePinnedToCore(AUTO_HELLO, "AUTO_HELLO", 8192,  ( void * ) &( xData_to_send_to_AUTO_HELLO ), 5, &AUTO_HELLO_Handler, ARDUINO_RUNNING_CORE);
  float temp1 = GetTaskHighWaterMarkPercent(AUTO_HELLO_Handler, 8192);
  ESP_LOGD(TAG, "calculating AUTO_HELLO stack size:%04.1f%%\r space free (unused)",temp1);
  
  while(1)
  {
    /*
      WiFiClient client = server.available(); // listen for incoming clients
      responses_WAP(client);
    */
    //ESP_LOGD(TAG, "Inside network peer");
    delay(50);
  }
}
