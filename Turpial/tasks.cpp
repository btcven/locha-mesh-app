/**
 * @file tasks.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#include <Arduino.h>
#include "hal/hardware.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "NVS.h"
#include "WAP.h"
#include "tasks.h"
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
