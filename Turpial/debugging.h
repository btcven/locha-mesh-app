/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas pra debugging de la app
*/

#ifndef DEBUGGING_H 
#define DEBUGGING_H


#include <Arduino.h>
#include "hardware.h"
//#include <iostream>
//#include "packet.h"
#include "route.h"

#ifdef DEBUG
  #define DEBUG_BEGIN(x)                Serial.begin (x)
  #define DEBUG_PRINT(x)                Serial.print (x)
  #define DEBUG_PRINT_DEC(x)            Serial.print (x, DEC)
  #define DEBUG_PRINT_HEX(x)            Serial.print (x, HEX)
  #define DEBUG_PRINTLN(x)              Serial.println (x)
  #define DEBUG_PRINT_STR(x)            Serial.print (F(x))
  #define DEBUG_PRINTLN_STR(x)          Serial.println (F(x))
#else
  #define DEBUG_BEGIN(x)
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINT_DEC(x)
  #define DEBUG_PRINT_HEX(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT_STR(x)
  #define DEBUG_PRINTLN_STR(x)
#endif

// end of serial.print for debugging



  void mostrar_packet(packet_t el_packet){
    int i;
    int j;
    int max_cols=4;
    int max_rows=4;
    String tabla[max_rows][max_cols];  // 4 columnas y max 5 filas

    tabla[0][0] = "Header - type";
    tabla[0][1] = "Header - from";
    tabla[0][2] = "Header - to";
    tabla[0][3] = "Header - timestamp";
    tabla[0][4] = "Body - payload";
  
    String tipo=String(el_packet.header.type);
    String from=(String)el_packet.header.from;
    String to=(String)el_packet.header.to;
    String timest=(String)el_packet.header.timestamp;
    String payload=(String)el_packet.body.payload;
    tabla[1][0] = tipo;
    tabla[1][1] = from;
    tabla[1][2] = to;
    tabla[1][3] = timest;
    tabla[1][4] = payload;
    DEBUG_PRINTLN(" ");
    for (i = 1; i <= 80; i++) {
      DEBUG_PRINT("-");
    }
     for (j = 0; i <= max_rows; j++) {
        for (i = 0; i <= max_cols; i++) {
            DEBUG_PRINT("\t");
            DEBUG_PRINT(tabla[i][j]);
            DEBUG_PRINT(" | ");
        }
         DEBUG_PRINTLN(" ");
         if (j==0){
           for (i = 1; i <= 80; i++) {
              DEBUG_PRINT("-");
            }
          
         }
      }
      DEBUG_PRINTLN(" ");
      for (i = 1; i <= 80; i++) {
          DEBUG_PRINT("-");
      }
     DEBUG_PRINTLN(" ");
     DEBUG_PRINT("Tamaño del packet :");
     DEBUG_PRINTLN(sizeof(el_packet));
     DEBUG_PRINTLN(" ");
}


void mostrar_vecinos(char* node_id, nodo_t vecinos[255]){
  int i;
  char* id_temporal;
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("Vecinos del nodo: ");
  DEBUG_PRINTLN(node_id);
  DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT("-");
      }
      DEBUG_PRINTLN();
  for (i = 1; i <= total_vecinos; i++) {
    DEBUG_PRINT("ID ");
    DEBUG_PRINT(i);
    DEBUG_PRINT(" : ");
    DEBUG_PRINT("\t");
    id_temporal=vecinos[i].id;
    DEBUG_PRINTLN((String)id_temporal);
  }
   DEBUG_PRINT("Tamaño de la tabla de vecinos :");
   DEBUG_PRINTLN(sizeof(vecinos));
   DEBUG_PRINT("Total de vecinos: ");
   DEBUG_PRINTLN(total_vecinos);
   DEBUG_PRINTLN();
   
}

void mostrar_rutas(char* node_id, rutas_t routeTable[255]){
   int i;
  char* id_temporal_origen;
  char* id_temporal_destino;
  char* id_temporal_next_neighbor;
  nodo_t origen;
  nodo_t destino;
  nodo_t next_neighbor;
  
  DEBUG_PRINTLN(" ");
  DEBUG_PRINT("Rutas disponibles en el nodo: ");
  DEBUG_PRINT(node_id);
  DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT("-");
      }
  DEBUG_PRINTLN();
  for (i = 1; i <= total_rutas; i++) {
    DEBUG_PRINT("Ruta ");
    DEBUG_PRINT(i);
    DEBUG_PRINT(" : ");
    DEBUG_PRINT("\t");
      origen=routeTable[i].origen;
      destino=routeTable[i].destino;
      next_neighbor=routeTable[i].next_neighbor;
      id_temporal_origen=origen.id;
      id_temporal_destino=destino.id;
      id_temporal_next_neighbor=next_neighbor.id;
      DEBUG_PRINT((String)id_temporal_origen);
      DEBUG_PRINT(" -> ");
      if (id_temporal_destino!=id_temporal_next_neighbor){
          DEBUG_PRINT((String)id_temporal_next_neighbor);
          DEBUG_PRINT(" -> ");
      }
     DEBUG_PRINT((String)id_temporal_destino);
     DEBUG_PRINT("\t");
     DEBUG_PRINT("Age: ");
     DEBUG_PRINTLN((String)routeTable[i].age);
    }
    DEBUG_PRINT("Tamaño de la tabla de rutas :");
    DEBUG_PRINTLN(sizeof(routeTable));
    DEBUG_PRINT("Total de rutas :");
    DEBUG_PRINTLN(total_rutas);
    DEBUG_PRINTLN();
}

void mostrar_cola_mensajes(){
  
}

#endif // DEBUGGING_H
