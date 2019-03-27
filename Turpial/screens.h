/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas para salida por pantalla Oled
*/

#ifndef SCREENS_H 
#define SCREENS_H
#include <Arduino.h>
#include <Wire.h>
#include <SSD1306.h>
//#include "lib/Heltec_esp32/src/heltec.h"
//#include "lib/Heltec_esp32/src/oled/SSD1306Wire.h"
//#include "lib/Heltec_esp32/src/oled/OLEDDisplay.h"
//#include "lib/Heltec_esp32/src/oled/OLEDDisplayUi.h"
// libreria para el screen oled de Heltec
//extern Heltec_ESP32 Heltec;
//OLEDDisplay *display = Heltec.display;
  
extern char* id_node;
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;

//void drawframe_title_with_2_fields(SSD1306Wire display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2);
//void drawframe_title_with_2_fields(int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2);
//void drawframe_table_with_4_fields(SSD1306Wire display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4);
//void drawframe_table_with_4_fields( int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4);
//void drawframe_table_with_4_fields(int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4);
//void drawFrame5(SSD1306Wire display, int16_t x, int16_t y);
//void drawFrame5(int16_t x, int16_t y);
//void drawFrame_tech(SSD1306Wire display, int16_t x, int16_t y);
//void drawFrame_tech(int16_t x, int16_t y);
//void drawframe_rows(SSD1306Wire display, int16_t x, int16_t y, String title, String row1, String row2, String row3, String row4, String row5);
//void drawframe_rows(int16_t x, int16_t y, String title, String row1, String row2, String row3, String row4, String row5);

#endif //SCREENS_H
