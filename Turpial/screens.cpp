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
#include <Wire.h>
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
#include "fonts/DejaVu_Sans_10.h"
#include "fonts/DejaVu_Sans_12.h"
#include <SSD1306.h>
#include "OLEDDisplay.h"

#include "scr_images.h"

extern char* id_node;
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;   // extern SSD1306 display;

void drawframe_title_with_2_fields(SSD1306 display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2)
{
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_12);
  display.drawString(x, y, title);
  display.setFont(DejaVu_Sans_10);
  display.drawString(x, y + 20, sub_title1);
  display.drawString(x + 10, y + 30, field1);
  display.drawString(x, y + 40, sub_title2);
  display.drawString(x + 10, y + 50, field2);
}
