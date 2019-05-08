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

#include "WAP.h"
#include "NVS.h"
#include "hal/hardware.h"


/**
 * @brief Start Wifi Access Point, read ssid, password data from NVS
 * 
 * @return esp_err_t 
 */

esp_err_t WAP_INIT()
{
 char *storage_name = "WAP";   // NVS Storage name on char*
 const char *TAG(storage_name);   // NVS Storage name on const char* for ESP_LOGD
 char *elssi = "ssid";   // SSID key name on NVS
 char *elpass = "pass";  // password key name on NVS 

 char *elnombre = "ap.locha.io";   // default Access Point SSID
 
 char *pChar = (char*)"";          // default password 

  // getting values from nvs or set values by default from hal/hardware.h
  char *WAP_ssid = nvs_get_string(storage_name, elssi, elnombre, true);
  const char *ssid_received(WAP_ssid);
  ESP_LOGD(TAG, "AP SSID : %s", WAP_ssid);
  char *WAP_pass = nvs_get_string(storage_name, elpass, pChar, true);
  const char *pass_received(WAP_pass);
  ESP_LOGD(TAG, "AP PASSWORD : %s", pass_received);

  int32_t WAP_channel = nvs_get_int(storage_name, "channel", WAP_CHANNEL, true);
  int32_t WAP_maxconn = nvs_get_int(storage_name, "maxconn", WAP_MAXCONN, true);


  bool initAP = WiFi.softAP(ssid_received, pass_received, WAP_channel, 0, WAP_maxconn);

  if (initAP)
  {
    ESP_LOGD(TAG, "AP init OK");
    return ESP_OK;
  }
  else
  {
    ESP_LOGE(TAG, "AP init ERROR\n");
    return ESP_FAIL;
  }
}

/*
void responses_WAP(WiFiClient client)
{
  const char *TAG = "WAP Response";
  if (client)
  { // if you get a client,
    ESP_LOGD(TAG, "New Client");
    String currentLine = ""; // make a String to hold incoming data from the client
    while (client.connected())
    { // loop while the client's connected
      if (client.available())
      {                         // if there's bytes to read from the client,
        char c = client.read(); // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        // if the byte is a newline character
        if (c == '\n')
        {
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
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
          }
          else
          { // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r')
        {                   // if you got anything else but a carriage return character,
          currentLine += c; // add it to the end of the currentLine
        }
      }
    }
    // close the connection:
    client.stop();
  }
}

*/
