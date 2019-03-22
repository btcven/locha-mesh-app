/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas para salida por pantalla Oled
*/
#include <Arduino.h>
#include "fonts/DejaVu_Sans_12.h"
#include "lib/heltec-oled/src/OLEDDisplayUi.h"
#include "scr_images.h"
  
void drawFrame2(OLEDDisplay *display, int16_t x, int16_t y) {
//void drawFrame1(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(DejaVu_Sans_12);
  display->drawString(x, y, "Node Locha Mesh");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Total Neigbours:");
  display->drawString(x+10, y + 30, (String)total_vecinos);
  display->drawString(x, y + 40, "Total Blacklisted:");
  display->drawString(x+10, y + 50, (String)total_nodos_blacklist);
}

void drawFrame3(OLEDDisplay *display, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Routes Locha Mesh");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Total Routes:");
  display->drawString(x, y + 30, (String)total_rutas);
}

void drawFrame4(OLEDDisplay *display, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Outcoming Queue");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Total packets queue:");
  display->drawString(x, y + 30, (String)total_mensajes_salientes);
}

void drawFrame5(OLEDDisplay *display, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Services");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Wifi:");
  String msg_screen="Inactive";
  #ifdef WAP_ENABLED
  msg_screen="Active";
  #else
    #ifdef WST_ENABLED
       msg_screen="Active";
    #endif
  #endif
  display->drawString(x, y + 30, "Lora:");
  msg_screen="Inactive";
  #ifdef RAD_ENABLED
    msg_screen="Active";
  #endif
  display->drawString(x, y + 40, msg_screen);
display->drawString(x, y + 50, "BLE:");
  msg_screen="Inactive";
  #ifdef BLE_ENABLED
    msg_screen="Active";
  #endif
  display->drawString(x, y + 60, msg_screen);
  
}


void drawFrame1(OLEDDisplay *display, int16_t x, int16_t y) {
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_16);
  display->drawString(x, y, "Routes Locha Mesh");
  display->setFont(ArialMT_Plain_10);
  display->drawString(x, y + 20, "Node id:");
  display->drawString(x, y + 30, (String)id_node);
}
