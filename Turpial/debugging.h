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
#include "route.h"


extern char *id_node ;
extern packet_t Buffer_packet;
extern rutas_t routeTable[255];
extern nodo_t vecinos[255];
extern message_queue_t mensajes_salientes[255];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 

 #ifdef DEBUG
    String mensaje;
    char* buffer_serial_received;
    int buffer_serial_received_size=0;
    String str_buffer_serial_received;
  #endif

#ifdef DEBUG
  #define DEBUG_BEGIN(x)                Serial.begin (x)
  #define DEBUG_PRINT(x)                Serial.print (x)
  #define DEBUG_WRITE(x)                Serial.write (x)
  #define DEBUG_PRINT_DEC(x)            Serial.print (x, DEC)
  #define DEBUG_PRINT_HEX(x)            Serial.print (x, HEX)
  #define DEBUG_PRINTLN(x)              Serial.println (x)
  #define DEBUG_PRINT_STR(x)            Serial.print (F(x))
  #define DEBUG_PRINTLN_STR(x)          Serial.println (F(x))
#else
  #define DEBUG_BEGIN(x)
  #define DEBUG_PRINT(x)
  #define DEBUG_WRITE(x)
  #define DEBUG_PRINT_DEC(x)
  #define DEBUG_PRINT_HEX(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT_STR(x)
  #define DEBUG_PRINTLN_STR(x)
#endif

// end of serial.print for debugging

String getparamValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void mostrar_packet(packet_t el_packet){
    int i;
    int j;
    int max_cols=4;
    int max_rows=4;
    String tabla[max_rows][max_cols];  // 4 columnas y max 5 filas

    tabla[0][0] = "Header - type";
    tabla[0][1] = "Header - from";
    tabla[0][2] = "Header - to";
    tabla[0][3] = F("Header - timestamp");
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
    DEBUG_PRINTLN();
    for (i = 1; i <= 80; i++) {
      DEBUG_PRINT(F("-"));
    }
     for (j = 0; i <= max_rows; j++) {
        for (i = 0; i <= max_cols; i++) {
            DEBUG_PRINT(F("\t"));
            DEBUG_PRINT(tabla[i][j]);
            DEBUG_PRINT(F(" | "));
        }
         DEBUG_PRINTLN(" ");
         if (j==0){
           for (i = 1; i <= 80; i++) {
              DEBUG_PRINT(F("-"));
            }
          
         }
      }
      DEBUG_PRINTLN();
      for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
     DEBUG_PRINTLN();
     DEBUG_PRINT(F("Tamaño del packet :"));
     DEBUG_PRINTLN(sizeof(el_packet));
     DEBUG_PRINTLN();
}

void vaciar_tablas(){
  total_vecinos=0;
  total_rutas=0;
  total_mensajes_salientes=0;
}


void mostrar_vecinos(char* node_id, nodo_t vecinos[255], size_t tamano_arreglo){
  int i;
  char* id_temporal;
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Vecinos del nodo: "));
  DEBUG_PRINTLN(node_id);
  DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
  for (i = 1; i <= total_vecinos; i++) {
    DEBUG_PRINT(F("ID "));
    DEBUG_PRINT(i);
    DEBUG_PRINT(F(" : "));
    DEBUG_PRINT(F("\t"));
    id_temporal=vecinos[i].id;
    DEBUG_PRINTLN((String)id_temporal);
  }
   DEBUG_PRINT(F("Tamaño de la tabla de vecinos :"));
   DEBUG_PRINT(tamano_arreglo);
   DEBUG_PRINTLN(F(" bytes"));
   DEBUG_PRINT(F("Total de vecinos: "));
   DEBUG_PRINTLN(total_vecinos);
   DEBUG_PRINTLN();
   
}

void mostrar_rutas(char* node_id, rutas_t routeTable[255], size_t tamano_arreglo){
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
          DEBUG_PRINT(F("-"));
      }
  DEBUG_PRINTLN();
  for (i = 1; i <= total_rutas; i++) {
    DEBUG_PRINT(F("Ruta "));
    DEBUG_PRINT(i);
    DEBUG_PRINT(F(" : "));
    DEBUG_PRINT(F("\t"));
      origen=routeTable[i].origen;
      destino=routeTable[i].destino;
      next_neighbor=routeTable[i].next_neighbor;
      id_temporal_origen=origen.id;
      id_temporal_destino=destino.id;
      id_temporal_next_neighbor=next_neighbor.id;
      DEBUG_PRINT((String)id_temporal_origen);
      DEBUG_PRINT(F(" -> "));
      if (id_temporal_destino!=id_temporal_next_neighbor){
          DEBUG_PRINT((String)id_temporal_next_neighbor);
          DEBUG_PRINT(F(" -> "));
      }
     DEBUG_PRINT((String)id_temporal_destino);
     DEBUG_PRINT(F("\t"));
     DEBUG_PRINT(F("Age: "));
     DEBUG_PRINTLN((String)routeTable[i].age);
    }
    DEBUG_PRINT(F("Tamaño de la tabla de rutas :"));
    DEBUG_PRINTLN(tamano_arreglo);
    DEBUG_PRINT(F("Total de rutas :"));
    DEBUG_PRINTLN(total_rutas);
    DEBUG_PRINTLN();
}

void mostrar_cola_mensajes(message_queue_t mensajes_salientes[255], size_t tamano_arreglo){
   int i;
   DEBUG_PRINTLN();
   DEBUG_PRINT(F("Cola mensajes salientes: "));
   DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
      // se coloca el titulo de la tabla
      DEBUG_PRINT(F("ID Mensaje"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("Prioridad"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("From"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("To"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("Timestamp"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINTLN();
         for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
     for (i = 1; i <= total_mensajes_salientes; i++) {
          
          DEBUG_PRINT(i);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_salientes[i].prioridad);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_salientes[i].paquete.header.from);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_salientes[i].paquete.header.to);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_salientes[i].paquete.header.timestamp);
          DEBUG_PRINT(F("\t"));
          
     }
}

// funcion para llenar manualmente los datos del modelo demo en la tabla vecinos y rutas
void iniciar_vecinos_y_rutas(char *id_nodo, nodo_t vecinos[255], rutas_t routeTable[255], size_t size_vecinos, size_t size_rutas)
{
 
  if (id_nodo == "turpial.0")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_actual.id = id_nodo;
    
    nodo_vecino.id = "turpial_1";  // agregarle nodo_vecino.id = "turpial_0" mediante alguna conversion para prevenir los warnings
   
    
    create_neighbor(nodo_vecino.id);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
   
  }
  if (id_nodo == "turpial.1")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_t nodo_vecino2;
    nodo_actual.id = id_nodo;
    nodo_vecino.id = "turpial_0";
    nodo_vecino2.id = "turpial_2";
    create_neighbor(nodo_vecino.id);
    create_neighbor(nodo_vecino2.id);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
    // ruta T2
    create_route(nodo_actual, nodo_vecino2, nodo_vecino2);
  }
  if (id_nodo == "turpial.2")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_actual.id = id_nodo;
    nodo_vecino.id = "turpial_1";
    create_neighbor(nodo_vecino.id);
    // ruta T2
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
  }
}



char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

void show_debugging_info(){
  #ifdef DEBUG
    
    if (Serial.available()) {
      str_buffer_serial_received=Serial.readStringUntil("\r\n");
      str_buffer_serial_received.toUpperCase();
      str_buffer_serial_received.trim();
   }
   
        mensaje=F("SHOW ROUTES");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          mostrar_rutas(id_node,routeTable, sizeof(routeTable));  
        }
        mensaje=F("SHOW NODES");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          mostrar_vecinos(id_node,vecinos,sizeof(vecinos));  
        }
        mensaje=F("SHOW QUEUE");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          mostrar_cola_mensajes(mensajes_salientes, sizeof(mensajes_salientes));  
        }
        
        mensaje="LOAD DEMO";
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            iniciar_vecinos_y_rutas(id_node, vecinos, routeTable,sizeof(vecinos),sizeof(routeTable));
            DEBUG_PRINTLN(mensaje+" OK");
         }
        mensaje=F("CLEAR ALL");
       
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            vaciar_tablas();
            DEBUG_PRINTLN(mensaje+" OK");
         }
         mensaje=F("CREATE NODE");
         if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
             Serial.print(F("Nombre del nodo:"));
              Serial.println(str_node_name);
              if (str_node_name.length()>0){
                 char chr_node_name[16];
                str_node_name.toCharArray(chr_node_name, 16);
                Serial.print("tengo:");
                Serial.println(chr_node_name);
                create_neighbor(chr_node_name);
                Serial.print("el vecino:");
                Serial.println(vecinos[total_vecinos].id);
                Serial.print("total vecinos:");
                Serial.println(total_vecinos);
                  DEBUG_PRINTLN(mensaje+" "+str_node_name+" OK");
              }  else {
                  DEBUG_PRINTLN(mensaje+" FAILED");
              }
              str_buffer_serial_received="";
         }
         mensaje=F("CLEAR SCREEN");
         if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            Serial.write(12);
         }



  
         }
        
  
  #endif



#endif // DEBUGGING_H
