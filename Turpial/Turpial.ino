/**
 * @file Turpial.ino
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include "Turpial.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "memory_def.h"
#include "tasks.h"
#include "SCR.h"
#include "RAD.h"
#include "WAPST.h"
#include "SQLite.h"


char *id_node;
/*
WiFiServer server(80);
*/

void setup()
{
  esp_err_t sys_init;

  /**
   * @brief Initialize filesystem
   * 
   */
  sys_init = open_fs();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
   * @brief Initialize the database 
   * 
   */
  sys_init = SQLite_INIT();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
   * @brief Display enabled on boot?
   * 
   */
  sys_init = SCR_INIT();
  if (sys_init != ESP_OK)
    esp_restart();

  // init. BLE on boot?

  /**
   * @brief WAP or WST ifaces enabled on boot?
   * 
   */
  sys_init = WiFi_INIT();
  if (sys_init != ESP_OK){
    //esp_restart();
    ESP_LOGE(TAG, "Error starting WiFi modules");
  }

  /**
   * @brief RAD iface enabled on boot?
   * 
   */
 // sys_init = RAD_INIT();
 // if (sys_init != ESP_OK)
 //   esp_restart();

  /**
   * @brief Start a master task, running network peer routines.
   * 
   */
  xTaskCreatePinnedToCore(NetworkPeer, "NetworkPeer", 2048, NULL, 5, &peerHandler, ARDUINO_RUNNING_CORE);
}

void loop()
{
  // nothing here please
}
