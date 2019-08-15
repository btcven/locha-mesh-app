/**
   @file tasks.h
   @author Locha Mesh project developers (locha.io)
   @brief
   @version 0.1.1
   @date 2019-08-15

   @copyright Copyright (c) 2019 Locha Mesh project developers
   @license Apache 2.0, see LICENSE file for details

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

extern char *id_node;

// variable for Auto hello timing control
unsigned long time_since_last_auto_hello = millis();


/*
  extern WiFiServer server;
*/
void NetworkPeer(void *params)
{

  for (;;)
  {
    /*
      WiFiClient client = server.available(); // listen for incoming clients
      responses_WAP(client);
    */
  }
}


void AUTO_HELLO(void *params) {
  packet_t packet_hello;
  char *pChar = (char *)"";
  char* packet_in_char;
const char *TAG = "AUTO_HELLO";

  while (1) {
    if (millis() < time_since_last_auto_hello) {
      time_since_last_auto_hello = millis();
    }
    if ((millis() - time_since_last_auto_hello) > HELLO_RETRY_TIMEOUT) {
      ESP_LOGD(TAG, "Sending packet HELLO");
      packet_hello = construct_packet_HELLO(id_node, pChar);
      
      packet_to_char(packet_in_char, packet_hello, sizeof(packet_hello));
      std::string mystring(packet_in_char);
      bool rpta = radioSend(mystring);
      if (rpta) {
        ESP_LOGD(TAG, "Packet HELLO sent to database OK");
      } else {
        ESP_LOGD(TAG, "Error sending Packet HELLO to database");
      }
    }
    delay(50);
  }
}
