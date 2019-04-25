/**
 * @file WAP.cpp
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
#include "WAP.h"


extern WiFiServer server;



bool load_config(){

    return true;
}

esp_err_t WAP_INIT()
{
    const char *TAG = "WAP";
    bool WAP_enabled = nvs_get_bool(TAG, "enabled", WAP_ENABLED);
    if (WAP_enabled){
            // WAP init
            if (load_config())
            {
                ESP_LOGD(TAG, "Loading config WAP OK");
                return ESP_OK;
            } else {
                ESP_LOGE(TAG, "Error Loading config WAP");
                return ESP_FAIL;
            }
            // starting WIFI SoftAP
            ESP_LOGD(TAG, "Starting WIFI SoftAP");
      
   } else {
       ESP_LOGD(TAG, "WAP disabled"); 
   }
}


 
/**
 * @brief function for wifi response management
 * 
 */
void responses_WAP(WiFiClient client){
   const char *TAG = "WAP Response";
  if (client) {                             // if you get a client,
    ESP_LOGD(TAG, "New Client");
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

           
            client.print("<!doctype html><html lang='en'><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1, shrink-to-fit=no, user-scalable=no'><title>Locha.mesh.io</title></head><body><H1>Hi! from Turpial</H1></body></html>");
           

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        
      }
    }
    // close the connection:
    client.stop();
}
}
