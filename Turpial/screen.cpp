/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#include "lib/heltec-oled/src/SSD1306.h"
#include <Arduino.h>
#include "defaultStartup.h"
#include "screen.h"
#include "pins.h"

extern BLE_status_t  BLEStatus;
extern WAP_status_t  WAPStatus; 
extern RAD_status_t  RADStatus;
extern WST_status_t  WSTStatus;

SSD1306 display(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);

/**
   @returns: bool
*/
bool scr_init()
{
  if (SCR_Vext)
  {
    pinMode(SCR_Vext, OUTPUT);
    digitalWrite(SCR_Vext, LOW);
    delay(50);
  }
  if (display.init())
  {
    display.flipScreenVertically();
    display.display();
    return true;
  }
  else
  {
    return false;
  }
}

/**
 * rad. frame:
 *      ┌---------------┐
 *      |    lora    BAT| <- Battery level.
 *      |            BLE| <- BLE status.
 * ┌--->| RSSI 000   WST| <- WST   "
 * | ┌->| SNR   00   WAP| <- WAP   "
 * | |  └---------------┘
 * | └--- last radio packet's SNR level. 
 * └----- last radio packet's RSSI level.
 */

void frame_radio()
{
  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);

  // __Bluetooth__
  if (BLEStatus.isActive)
  {
    // display.drawXbm(posX, posY, img_w, img_h, img_bits);
    if (BLEStatus.isConnected)
    {
      // display.drawString(posX, posY, "o");
    }
  }

  // __WST__
  if (WSTStatus.isActive)
  {
    // display.drawXbm(posX, posY, img_w, img_h, img_bits);
    if (WSTStatus.isConnected)
    {
      // display.drawString(posX, posY, "o");
    }
  }

  // __WAP__
  if (WAPStatus.isActive)
  {
    // display.drawXbm(posX, posY, img_w, img_h, img_bits);
    // WAPStatus.clientCount || 0
    // display.drawString(posX, posY, String(WAPStatus.clientCount));
  }

  // __RAD__
  if (RADStatus.isEnabled)
  {

    if (RADStatus.isActive)
    {
      // display.drawString(posX, posY, "RSSI:" + String(RAD_rssi));
      // display.drawString(posX, posY, "SNR: " + String(RAD_rssi));
    }
  }

  // show it
  display.display();
}
