/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas para salida por pantalla Oled
*/
#include <Arduino.h>
#include "Esp.h"
#include "rom/spi_flash.h"
#include "esp_sleep.h"
#include "esp_spi_flash.h"

#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
#include "fonts/DejaVu_Sans_10.h"
#include "fonts/DejaVu_Sans_12.h"
#include "lib/heltec-oled/src/OLEDDisplayUi.h"
#include "scr_images.h"

extern char* id_node;
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;

void drawframe_title_with_2_fields(OLEDDisplay *display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2){
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(DejaVu_Sans_12);
  display->drawString(x, y, title);
  display->setFont(DejaVu_Sans_10);
  display->drawString(x, y + 20, sub_title1);
  display->drawString(x+10, y + 30, field1);
  display->drawString(x, y + 40, sub_title2);
  display->drawString(x+10, y + 50, field2);
}

void drawframe_table_with_4_fields(OLEDDisplay *display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4){
  uint8_t borde_tabla=60;
  uint8_t margen=10;
  
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(DejaVu_Sans_12);
  display->drawString(x, y, title);
  display->setFont(DejaVu_Sans_10);
  display->drawString(x, y + 20, sub_title1);
  display->drawString(x+margen, y + 30, field1);
  display->drawString(x+borde_tabla, y + 20, sub_title2);
  display->drawString(x+margen+borde_tabla, y + 30, field2);
  display->drawString(x, y + 40, sub_title3);
  display->drawString(x+10, y + 50, field3);
  display->drawString(x+borde_tabla, y + 40, sub_title4);
  display->drawString(x+margen+borde_tabla, y + 50, field4);
  
}




void drawFrame_tech(OLEDDisplay *display, int16_t x, int16_t y) {
  String linea;
  linea=(String)esp_get_idf_version();
 linea=linea.substring(0,10);
  
  drawframe_table_with_4_fields(display, x, y, "System info", "Heap free:", (String)heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL), "Chip Size:", (String)(ESP.getFlashChipSize()/1024)+" Mb", "IDF ver.:", linea, "App size:" , (String)(ESP.getSketchSize()/1024)+" Kb");
}


void drawFrame5(OLEDDisplay *display, int16_t x, int16_t y) {

  
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(DejaVu_Sans_12);
  display->drawString(x, y, "Services");
  display->setFont(DejaVu_Sans_10);
   String msg_screen="Inactive";
  #ifdef WAP_ENABLED
    msg_screen="Active";
    if (WAP_ENABLED){
      msg_screen=msg_screen+F("-"+MSG_ENABLED);
    } else {
      msg_screen=msg_screen+F("-"+MSG_DISABLED);
    }
  #else
    #ifdef WST_ENABLED
       msg_screen="Active";
       if (WST_ENABLED){
          msg_screen=msg_screen+F("-"+MSG_ENABLED);
        } else {
          msg_screen=msg_screen+F("-"+MSG_DISABLED);
        }
    #endif
  #endif
  display->drawString(x, y + 20, "Wifi: "+msg_screen);
 
   msg_screen="Inactive";
  #ifdef RAD_ENABLED
    msg_screen="Active";
     if (RAD_ENABLED){
          msg_screen=msg_screen+F("-"+MSG_ENABLED);
        } else {
          msg_screen=msg_screen+F("-"+MSG_DISABLED);
        }
  #endif
  display->drawString(x, y + 30, "Lora: "+msg_screen);
 msg_screen="Inactive";
  #ifdef BLE_ENABLED
    msg_screen="Active";
     if (BLE_ENABLED){
          msg_screen=msg_screen+F("-"+MSG_ENABLED);
        } else {
          msg_screen=msg_screen+F("-"+MSG_DISABLED);
        }
  #endif
display->drawString(x, y + 40, "BLE: "+msg_screen);

}
