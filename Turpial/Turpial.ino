/**
 * @file Turpial.ino
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */
#include <Arduino.h>
#include "Turpial.h"
#include "SCR.h"

void setup()
{
  esp_err_t sys_init;

  // init. SCR on boot?
  sys_init = SCR_INIT();
  if (sys_init != ESP_OK)
    esp_restart();
  
  // init. BLE on boot?

  // init. WAP on boot?

  // init. WST on boot?

  // init. RAD on boot?

}

void loop()
{
  // nothing here please
}