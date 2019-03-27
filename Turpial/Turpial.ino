/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <Arduino.h>
#include <string.h>
#include <Wire.h>

// devices and default settings
#include "hardware.h"
#include "language_es.h"
#include "boards_def.h"
#include "memory_def.h"
#include "general_functions.h"
#include "packet.h"
#include "route.h"
#include "debugging.h"
#include "fonts/DejaVu_Sans_10.h"
#include "fonts/DejaVu_Sans_12.h"
//#include "screens.h"


#ifdef RAD_ENABLED
#include <SSD1306.h>
  
  #include "radio.h"
#endif

#ifdef WAP_ENABLED
  #include <WiFi.h>
#else
  #ifdef WST_ENABLED
    #include <WiFi.h>
  #endif
#endif
  
#ifdef SCR_ENABLED  

  #include "scr_images.h"
#endif
#ifdef BLE_ENABLED 
  #include "bluetooth.h"
#endif 



// variables fijas para este demo
// ID unico del nodo
char id_nodo_demo[]="turpial.0";
char* id_node=id_nodo_demo;


#ifdef SCR_ENABLED  
//SSD1306Wire display(SCR_ADD, SDA_OLED, SCL_OLED,RST_OLED);
SSD1306 display(SCR_ADD, SDA_OLED, SCL_OLED, RST_OLED);

  #define OLED_SCREEN_INTERVAL 5000 
#endif

// includes internos
uint8_t total_vecinos = 0; // cantidad de vecinos del nodo actual
uint8_t total_rutas = 0;   // cantidad de rutas del nodo actual (en iniciar_vecinos_y_rutas() se llenan manualmente las rutas a efectos del demo)
uint8_t total_mensajes_salientes = 0;   // cantidad de mensajes en la cola
uint8_t total_nodos_blacklist = 0;   // cantidad de nodos en blacklist

rutas_t routeTable[MAX_ROUTES];
nodo_t vecinos[MAX_NODES];
nodo_t blacklist[MAX_NODES_BLACKLIST];
message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
packet_t Buffer_packet;   // packet_t usado como buffer para mensajes incoming y outcoming

unsigned long tiempo;

// variables para trasmision BLE
  String rxValue="";
  String txValue="";


void setup()
{
  bool display_enabled=false;
  bool lora_enabled=false;
  bool serial_enabled=false;
  bool wifi_enabled=false;

 
  
 #ifdef DEBUG
    serial_enabled=true;
 #endif
 #ifdef SCR_ENABLED
 if (SCR_ENABLED) {
    display_enabled=true;
#ifdef V2
    pinMode(Vext, OUTPUT);
    #endif 
 } else {
      display_enabled=false;
    }
 #endif
 #ifdef RAD_ENABLED
 if (RAD_ENABLED) {
    lora_enabled=true;
 } else {
      lora_enabled=false;
    }
 #endif
 #ifdef WST_ENABLED
 if (WST_ENABLED) {
    wifi_enabled=true;
 } else {
      wifi_enabled=false;
    }
 #endif
  #ifdef WAP_ENABLED
 if (WAP_ENABLED) {
    wifi_enabled=true;
 } else {
      wifi_enabled=false;
    }
 #endif


 
//   Heltec.begin(display_enabled /*DisplayEnable Enable*/, !lora_enabled /*LoRa Disable*/, true /*Serial Enable*/);
  
//  Heltec.display->setContrast(255);
//  Heltec.display->setContrast(255);
 #ifdef DEBUG
    DEBUG_BEGIN(BAUDRATE);
    while(!Serial);
    Serial.setDebugOutput(true);
    delay(2000);
 #endif
  
    // se inicializa la libreria para Heltec
   // Heltec.begin(display_enabled /*DisplayEnable Enable*/, !lora_enabled /*LoRa Disable*/, serial_enabled /*Serial Enable*/);
    
  
 
  rxValue="";
  txValue="";

      #ifdef SCR_ENABLED  
      if (SCR_ENABLED)
      {
        DEBUG_PRINTLN(MSG_SCR_INIT);
        // activar módulo SCR
        // leer NVS,  verificar si existe registro
        // si existe aplicar, si no establecer parametros por defecto.
       
        
      // se inicializa el display
      //Heltec.display->setContrast(255);
      display.init();
      //ui.init();
      //Heltec.display->flipScreenVertically();
      display.flipScreenVertically();
      //display->init();
      //display->flipScreenVertically();
  
        
      }
      #endif

      #ifdef BLE_ENABLED
      if (BLE_ENABLED)
      {
        DEBUG_PRINTLN(F("[BLE] Initiating... "));
        // -- activar módulo ble --
        // 1.- leer NVS,  verificar si existe registro
        // 2.- si existe aplicar, si no establecer parametros por defecto.
        // 3.- activar la tarea
   
         xTaskCreate(task_bluetooth, "task_bluetooth", 2048, NULL, 5, NULL);
        
      }
      #endif

      #ifdef WAP_ENABLED
          if (WAP_ENABLED)
          {
            DEBUG_PRINT(F("[WAP] Initiating... "));
            // -- activar módulo wap --
          }
      #endif
  #ifdef WST_ENABLED
  if (WST_ENABLED)
  {
    DEBUG_PRINTLN(F("[WST] Initiating... "));
    // -- activar módulo wst --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
  }
   #endif
  #ifdef RAD_ENABLED
  if (RAD_ENABLED)
  {
    DEBUG_PRINTLN(F("[RAD] Initiating... "));
    // -- activar modulo de radio --
    // 1.- leer NVS,  verificar si existe registro
    // 2.- si existe aplicar, si no establecer parametros por defecto.
    // 3.- iniciar.
     xTaskCreate(task_radio, "task_radio", 2048, NULL, 5, NULL);
  }
 #endif
 
// se coloca el cursor en el terminal serial
 
      DEBUG_PRINTLN(F("Starting terminal"));
      // se genera el node_id solo si no existe
      if (id_node==""){
         create_unique_id(id_node);
      }
      DEBUG_PRINT(id_node);
      DEBUG_PRINT(F(" >"));


#ifdef LED_ENABLED
    pinMode(LED_PIN, OUTPUT);
    if (LED_ENABLED){
        digitalWrite(LED_PIN, HIGH);
    } else {
        digitalWrite(LED_PIN, LOW);  
    }
#endif

// se inicializa el control del tiempo
tiempo=millis();
}






void drawframe_title_with_2_fields(int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2){
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_12);
  display.drawString(x, y, title);
  display.setFont(DejaVu_Sans_10);
  display.drawString(x, y + 20, sub_title1);
  display.drawString(x+10, y + 30, field1);
  display.drawString(x, y + 40, sub_title2);
  display.drawString(x+10, y + 50, field2);
}

//void drawframe_table_with_4_fields(SSD1306Wire display, int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4){
void drawframe_table_with_4_fields(int16_t x, int16_t y, String title, String sub_title1, String field1, String sub_title2, String field2, String sub_title3, String field3, String sub_title4, String field4){
  uint8_t borde_tabla=60;
  uint8_t margen=10;
  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_12);
  display.drawString(x, y, title);
  display.setFont(DejaVu_Sans_10);
  display.drawString(x, y + 20, sub_title1);
  display.drawString(x+margen, y + 30, field1);
  display.drawString(x+borde_tabla, y + 20, sub_title2);
  display.drawString(x+margen+borde_tabla, y + 30, field2);
  display.drawString(x, y + 40, sub_title3);
  display.drawString(x+10, y + 50, field3);
  display.drawString(x+borde_tabla, y + 40, sub_title4);
  display.drawString(x+margen+borde_tabla, y + 50, field4);
}

void drawframe_rows(int16_t x, int16_t y, String title, String row1, String row2, String row3, String row4, String row5){
  //void drawframe_rows(SSD1306Wire display, int16_t x, int16_t y, String title, String row1, String row2, String row3, String row4, String row5){
   
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(DejaVu_Sans_12);
  display.drawString(x, y, title);
  display.setFont(DejaVu_Sans_10);
  display.drawString(x, y + 20, row1);
  display.drawString(x, y + 30, row2);
  display.drawString(x, y + 40, row3);
  display.drawString(x, y + 50, row4);
  display.drawString(x, y + 60, row5);
}



//void drawFrame_tech(SSD1306Wire display, int16_t x, int16_t y) {
void drawFrame_tech( int16_t x, int16_t y) {
  String linea;
  linea=(String)esp_get_idf_version();
  linea=linea.substring(0,10);
  drawframe_table_with_4_fields( x, y, "System info", "Heap free:", (String)heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL), "Chip Size:", (String)(ESP.getFlashChipSize()/1024)+" Mb", "IDF ver.:", linea, "Sketch:" , (String)(ESP.getSketchSize()/1024)+" Kb");
}

//void drawFrame5(SSD1306Wire display, int16_t x, int16_t y) {
void drawFrame5(int16_t x, int16_t y) {
  String msg_screen_wifi="Inactive";
  #ifdef WAP_ENABLED
    msg_screen_wifi="Active WAP";
    if (WAP_ENABLED){
       #ifdef WST_ENABLED
          if (WST_ENABLED){
             msg_screen_wifi="Active WAP/WST";
          }
       #endif
      msg_screen_wifi=msg_screen_wifi+"-"+MSG_ENABLED;
      
    } else {
      msg_screen_wifi=msg_screen_wifi+"-"+MSG_DISABLED;
    }
  #else
    #ifdef WST_ENABLED
       msg_screen_wifi="Active WST";
       if (WST_ENABLED){
          msg_screen_wifi=msg_screen_wifi+"-"+MSG_ENABLED;
        } else {
          msg_screen_wifi=msg_screen_wifi+"-"+MSG_DISABLED;
        }
    #endif
  #endif

 
 
  String  msg_screen_radio="Inactive";
  #ifdef RAD_ENABLED
    msg_screen_radio="Active";
     if (RAD_ENABLED){
          msg_screen_radio=msg_screen_radio+"-"+MSG_ENABLED;
        } else {
          msg_screen_radio=msg_screen_radio+"-"+MSG_DISABLED;
        }
  #endif
   
 String msg_screen_ble="Inactive";
  #ifdef BLE_ENABLED
    msg_screen_ble="Active";
     if (BLE_ENABLED){
          msg_screen_ble=msg_screen_ble+"-"+MSG_ENABLED;
        } else {
          msg_screen_ble=msg_screen_ble+"-"+MSG_DISABLED;
        }
  #endif
 //drawframe_rows(display, 0, 0, "Services", "Wifi: "+msg_screen_wifi, "Lora: "+msg_screen_radio, "BLE: "+msg_screen_ble, "", "");
drawframe_rows(0, 0, "Services", "Wifi: "+msg_screen_wifi, "Lora: "+msg_screen_radio, "BLE: "+msg_screen_ble, "", "");
}



int pantalla_activa=1;

void loop()
{

    
    if (millis()-tiempo>OLED_SCREEN_INTERVAL){
      //Heltec.display->clear();
     
      display.clear();
      switch (pantalla_activa) {
    case 1:
      //drawframe_title_with_2_fields(Heltec.display, 0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      drawframe_title_with_2_fields(0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      //drawframe_title_with_2_fields(display, 0, 0, "Locha Mesh", "Node id:", (String)id_node, "", "");
      
      break;
    case 2:
      //drawframe_table_with_4_fields(Heltec.display, 0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos)+" bytes", "Size:", (String)sizeof(blacklist)+" bytes");
      drawframe_table_with_4_fields( 0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos)+" bytes", "Size:", (String)sizeof(blacklist)+" bytes");
      //drawframe_table_with_4_fields(display, 0, 0, "Node Locha Mesh", "Neigbours:", (String)total_vecinos, "Blacklisted:", (String)total_nodos_blacklist, "Size:", (String)sizeof(vecinos)+" bytes", "Size:", (String)sizeof(blacklist)+" bytes");
      break;
    case 3:
      //drawframe_title_with_2_fields(Heltec.display, 0, 0, "Routes Locha Mesh","Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable)+" bytes");
      drawframe_title_with_2_fields( 0, 0, "Routes Locha Mesh","Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable)+" bytes");
     // drawframe_title_with_2_fields(display, 0, 0, "Routes Locha Mesh","Total Routes:", (String)total_rutas, "Size:", (String)sizeof(routeTable)+" bytes");
      break;
    case 4:
      //drawframe_title_with_2_fields(Heltec.display, 0, 0, "Outcoming Queue","Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes)+" bytes");
      drawframe_title_with_2_fields(0, 0, "Outcoming Queue","Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes)+" bytes");
      //drawframe_title_with_2_fields(display, 0, 0, "Outcoming Queue","Total packets queue:", (String)total_mensajes_salientes, "Size:", (String)sizeof(mensajes_salientes)+" bytes");
      break;
    case 5:
      //drawFrame5(Heltec.display, 0, 0);
      drawFrame5( 0, 0);
      //drawFrame5(display, 0, 0);
      break;
    case 6:
      //drawFrame_tech(Heltec.display, 0, 0);
      drawFrame_tech( 0, 0);
      //drawFrame_tech(display, 0, 0);
      break;
    default:
     pantalla_activa=0;  //para que aparezca la primera pantalla
      break;
  }
  
  pantalla_activa++;
  tiempo=millis();
  if (pantalla_activa>6){
    pantalla_activa=1;
  }
  
//      Heltec.display.display();
  //   Heltec.display->display();
   //Heltec.display->display();
     display.display();
    }
    
if (millis()-tiempo<0){
  tiempo=millis();
}
   
    
    
    
 // se efectua el procesamiento de paquetes entrantes
          packet_processing_incoming();
          // se efectua el procesamiento de paquetes salientes
          packet_processing_outcoming();   
          // solo se agrega la consola de comandos cuando se esta compilando para DEBUG
          #ifdef DEBUG
         
              uint8_t rpta=show_debugging_info(vecinos,total_vecinos);
          #endif

  
}
