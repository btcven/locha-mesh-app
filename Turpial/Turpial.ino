/**
   @file Turpial.ino
   @author locha.io project developers (dev@locha.io)
   @brief
   @version 0.1
   @date 2019-04-24

   @copyright Copyright (c) 2019 locha.io project developers
   @license MIT license, see LICENSE file for details

*/

#include "Turpial.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "tasks.h"
#include "SCR.h"
#include "RAD.h"
#include "WAPST.h"
#include "BLE.h"
#include "SQLite.h"






void setup()
{
  /**
       @brief Generating Unique Device ID
  */

  char id_node[SIZE_IDNODE];
  const char *TAG = "Setup";
  
  std::string mac_device = getMacAddress_onlychars();
  snprintf(id_node, sizeof(id_node), "%s", mac_device.c_str());
  // Starting serial terminal for debugging
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.setDebugOutput(true);
  
  ESP_LOGD(TAG, "Id device:%s", id_node);
  esp_err_t sys_init;

  /**
     @brief Initialize filesystem
  */
  sys_init = open_fs();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
     @brief Initialize the database

  */
  sys_init = SQLite_INIT();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
     @brief Display enabled on boot?

  */
  sys_init = SCR_INIT();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
     @brief BLE enabled on boot?

  */
  sys_init = BLE_INIT();
  if (sys_init != ESP_OK)
    esp_restart();

  /**
     @brief WAP or WST ifaces enabled on boot?

  */
  sys_init = WiFi_INIT();
  if (sys_init != ESP_OK)
  {
    //esp_restart();
    ESP_LOGE(TAG, "Error starting WiFi modules");
  }

  /**
     @brief RAD iface enabled on boot?

  */
  sys_init = RAD_INIT();
  if (sys_init != ESP_OK)
    esp_restart();


  ESP_LOGD(TAG, "Starting NetworkPeer");
  /**
     @brief Start a master task, running network peer routines.

  */
  // params for task
  AData xData_to_send;
  snprintf(xData_to_send.id_node, sizeof(id_node), "%s", id_node);
  BaseType_t xReturned_NetworkPeer;
  
  
  xReturned_NetworkPeer = xTaskCreatePinnedToCore(NetworkPeer, "NetworkPeer", 10240, ( void * ) & ( xData_to_send ), 5, &peerHandler, ARDUINO_RUNNING_CORE);
  // if xtask was created ok then calc stack size
  if ( xReturned_NetworkPeer == pdPASS )
  {
    ESP_LOGD(TAG, "Task NetworkPeer started OK");
    float temp2 = GetTaskHighWaterMarkPercent(peerHandler, 10240);
    ESP_LOGD(TAG, "calculating NetworkPeer stack size:%04.1f%%\r space free (unused)", temp2);
  } else {
    ESP_LOGD(TAG, "NetworkPeer task fail to start");
  }
  
  ESP_LOGD(TAG, "Finishing setup");
}

void loop()
{
  // nothing here please
}
