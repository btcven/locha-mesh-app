/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#include "SSD1306.h"
#include "screen.h"
#include "hardwareDefinitions.h"

SSD1306 _scr_(SCR_ADD, SCR_SDA, SCR_SCL, SCR_RST);

/**
   @returns: bool
*/
bool screen_init() {
  if (SCR_Vext) {
    pinMode(SCR_Vext, OUTPUT);
    digitalWrite(SCR_Vext, LOW);
    delay(50);
  }
  if (_scr_.init()) {
    _scr_.flipScreenVertically();
    _scr_.display();
    return true;
  } else {
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
void frame_rad() {
  
  _scr_.clear();
  _scr_.setFont(ArialMT_Plain_10);
  _scr_.setTextAlignment(TEXT_ALIGN_RIGHT);

  // __Bluetooth__
  if (BLEStatus.isActive) {
    // _scr_.drawXbm(posX, posY, img_w, img_h, img_bits);
    if (BLEStatus.isConnected) {
      // _scr_.drawString(posX, posY, "o");
    }
  }

  // __WST__
  if (WSTStatus.isActive) {
    // _scr_.drawXbm(posX, posY, img_w, img_h, img_bits);
    if (WSTStatus.isConnected) {
      // _scr_.drawString(posX, posY, "o");
    }
  }

  // __WAP__
  if (WAPStatus.isActive) {
    // _scr_.drawXbm(posX, posY, img_w, img_h, img_bits);
    // WAPStatus.clientCount || 0
    // _scr_.drawString(posX, posY, String(WAPStatus.clientCount));
  }
  
  // __RAD__
  if (RADStatus.isEnabled) {
    
    if (RADStatus.isActive) {
      // _scr_.drawString(posX, posY, "RSSI:" + String(RAD_rssi));
      // _scr_.drawString(posX, posY, "SNR: " + String(RAD_rssi));
    }
  }
  
  // show it
  _scr_.display();
}
