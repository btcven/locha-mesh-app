#include <Arduino.h>
#include "hardware.h"
#include "general_functions.h"
#include "debugging.h"
#include "boards_def.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"


extern char *id_node ;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 




 #ifdef DEBUG
    String mensaje;
    char* buffer_serial_received;
    int buffer_serial_received_size=0;
    String str_buffer_serial_received;
  #endif


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

uint8_t mostrar_packet(packet_t el_packet){
    uint8_t i;
    uint8_t j;
    uint8_t max_cols=4;
    uint8_t max_rows=4;
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
     return 0;
}

uint8_t vaciar_tablas(){
  total_vecinos=0;
  total_rutas=0;
  total_mensajes_salientes=0;
  return 0;
}


uint8_t mostrar_vecinos(char* node_id, nodo_t vecinos[MAX_NODES], size_t tamano_arreglo){
  uint8_t i;
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
  return 0;   
}

uint8_t mostrar_rutas(char* node_id, rutas_t routeTable[MAX_ROUTES], size_t tamano_arreglo){
  uint8_t i;
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
    DEBUG_PRINT(tamano_arreglo);
    DEBUG_PRINTLN(F(" bytes"));
    DEBUG_PRINT(F("Total de rutas :"));
    DEBUG_PRINTLN(total_rutas);
    DEBUG_PRINTLN();
    return 0;
}

uint8_t mostrar_cola_mensajes(message_queue_t mensajes_salientes[MAX_MSG_QUEUE], size_t tamano_arreglo){
   uint8_t i;
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
     return 0;
}

// funcion para llenar manualmente los datos del modelo demo en la tabla vecinos y rutas
uint8_t iniciar_vecinos_y_rutas(char *id_nodo, nodo_t vecinos[255], rutas_t routeTable[255], size_t size_vecinos, size_t size_rutas)
{
   String nombre_nodo="";
  if (id_nodo == "turpial.0")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_actual.id = id_nodo;
    nombre_nodo="turpial_1";
    nodo_vecino.id = string2char(nombre_nodo);
    
    
    create_neighbor(nombre_nodo,vecinos);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
   
  }
  if (id_nodo == "turpial.1")
  {
    nodo_t nodo_actual;
    nodo_t nodo_vecino;
    nodo_t nodo_vecino2;
  
    nodo_actual.id = id_nodo;
    nombre_nodo="turpial_0";
    nodo_vecino.id = string2char(nombre_nodo);
    create_neighbor(nombre_nodo,vecinos);
    nombre_nodo="turpial_2";
    nodo_vecino2.id = string2char(nombre_nodo);
    
    create_neighbor(nombre_nodo,vecinos);
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
     nombre_nodo="turpial_1";
    nodo_vecino.id = string2char(nombre_nodo);
    
    create_neighbor(nombre_nodo,vecinos);
    // ruta T2
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
  }
  return 0;
}





uint8_t show_debugging_info(nodo_t vecinos[MAX_NODES]){
  #ifdef DEBUG
    uint8_t rpta;
    
    if (Serial.available()) {
      str_buffer_serial_received=Serial.readStringUntil('\n');
      str_buffer_serial_received.toUpperCase();
      str_buffer_serial_received.trim();
      str_buffer_serial_received.replace("  "," ");  // se elimina cualquier doble espacio en el input
   }
   
        mensaje="SHOW ROUTES";
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_rutas(id_node,routeTable, sizeof(routeTable));  
        }
        mensaje="SHOW NODES";
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_vecinos(id_node,vecinos,sizeof(vecinos));  
        }
        mensaje="SHOW QUEUE";
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_cola_mensajes(mensajes_salientes, sizeof(mensajes_salientes));  
        }
        
        mensaje="LOAD DEMO";
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            uint8_t rpta=iniciar_vecinos_y_rutas(id_node, vecinos, routeTable,sizeof(vecinos),sizeof(routeTable));
            DEBUG_PRINTLN(mensaje+" OK");
         }
        mensaje="CLEAR ALL";
       
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            uint8_t rpta=vaciar_tablas();
            DEBUG_PRINTLN(mensaje+" OK");
         }
         mensaje="CREATE NODE";
         if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
             Serial.print("param0:");
             Serial.println(getparamValue(str_buffer_serial_received, ' ', 0));
             Serial.print("param1:");
             Serial.println(getparamValue(str_buffer_serial_received, ' ', 1));
             Serial.print("param2:");
             Serial.println(getparamValue(str_buffer_serial_received, ' ', 2));
             
              Serial.print(F("Nombre del nodo:"));
              Serial.println(str_node_name);
              if (str_node_name.length()>0){
                
                uint8_t rpta=create_neighbor(str_node_name,vecinos);
                Serial.print("al salir el nombre del nodo que recibo es:");
                Serial.println(vecinos[total_vecinos].id);
                Serial.print("total vecinos:");
                Serial.println(total_vecinos);
                DEBUG_PRINTLN((String)mensaje+" "+(String)str_node_name+" OK");
              }  else {
                  DEBUG_PRINTLN((String)mensaje+" FAILED");
              }
              str_buffer_serial_received="";
         }
         mensaje="CLEAR SCREEN";
         if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            str_buffer_serial_received="";
            Serial.write(12);
         }
         return 0;
         }
        
  
  #endif
