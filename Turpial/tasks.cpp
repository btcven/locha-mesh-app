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


void AUTO_HELLO(void *params) {
  // received params struct
  AData *prt2_param_received;
  prt2_param_received = (AData *) params;

  // variable for Auto hello timing control
  unsigned long time_since_last_auto_hello = millis();

  char *pChar = (char *)"";
  char *packet_in_char = (char *)"";
  const char *TAG = "AUTO_HELLO";
  packet_t packet_hello;

  uint8_t bytearr[sizeof(packet_hello)];


  while (true) {

    if (millis() < time_since_last_auto_hello) {
      time_since_last_auto_hello = millis();
    }
    if ((millis() - time_since_last_auto_hello) > HELLO_RETRY_TIMEOUT) {

      packet_hello = construct_packet_HELLO( prt2_param_received->id_node);


      packet_to_uint8_t(bytearr, packet_hello, sizeof(packet_hello));
      ESP_LOGD(TAG, "packet HELLO sending now...");
      bool rpta = radioSend(bytearr, sizeof(packet_hello));
      if (rpta) {
        ESP_LOGD(TAG, "Packet HELLO sent to radio OK");
      } else {
        ESP_LOGD(TAG, "Error sending Packet HELLO to Radio");
      }

    }

    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}


void NetworkPeer(void *params)
{
  // received params struct
  AData *prt_param_received;
  prt_param_received = (AData *) params;

  /**
    Handler for Auto Hello
  */
  TaskHandle_t AUTO_HELLO_Handler;



  const char *TAG = "NetworkPeer";
  ESP_LOGD(TAG, "Starting AUTO HELLO");

  // params for task AUTO HELLO
  xData xData_to_send_to_AUTO_HELLO;
  snprintf(xData_to_send_to_AUTO_HELLO.id_node, sizeof(prt_param_received->id_node), "%s", prt_param_received->id_node);

  BaseType_t xReturned_AUTO_HELLO;
  xReturned_AUTO_HELLO = xTaskCreatePinnedToCore(AUTO_HELLO, "AUTO_HELLO", 46080,  ( void * ) & ( xData_to_send_to_AUTO_HELLO ) , 5, &AUTO_HELLO_Handler, ARDUINO_RUNNING_CORE);
  if ( xReturned_AUTO_HELLO == pdPASS )
  {
    float temp1 = GetTaskHighWaterMarkPercent(AUTO_HELLO_Handler, 46080);
    ESP_LOGD(TAG, "calculating AUTO_HELLO stack size:%04.1f%%\r space free (unused)", temp1);
  } else {
    ESP_LOGD(TAG, "AUTO_HELLO task fail to start");
  }

  while (true)
  {
    /*
      WiFiClient client = server.available(); // listen for incoming clients
      responses_WAP(client);
    */

    vTaskDelay( 500 / portTICK_PERIOD_MS );
  }
}
