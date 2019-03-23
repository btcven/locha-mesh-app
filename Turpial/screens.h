/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas para salida por pantalla Oled
*/

#ifndef SCREENS_H 
#define SCREENS_H

#include "lib/heltec-oled/src/OLEDDisplayUi.h"

extern char* id_node;
extern uint8_t total_mensajes_salientes;  
extern uint8_t total_vecinos;  
extern uint8_t total_rutas;
extern uint8_t total_nodos_blacklist;

void drawframe_title_with_2_fields(OLEDDisplay *display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2);
void drawframe_table_with_4_fields(OLEDDisplay *display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4);
void drawFrame5(OLEDDisplay *display, int16_t x, int16_t y);
void drawFrame_tech(OLEDDisplay *display, int16_t x, int16_t y);

#endif //SCREENS_H