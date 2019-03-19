#include <Arduino.h>
#include "hardware.h"
#include "general_functions.h"
#include "debugging.h"
#include "boards_def.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"
#include "language_es.h"
#include "MemoryFree.h"

extern String rxValue;
extern String txValue;

#ifdef BLE_ENABLED
  #include "bluetooth.h"
#endif

extern char* id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 




 #ifdef DEBUG
   // String mensaje;
   // char* buffer_serial_received;
   // int buffer_serial_received_size=0;
    
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

    Serial.print("from:");
    Serial.println(from);
    Serial.print("to:");
    Serial.println(to);
    Serial.print("payload:");
    Serial.println(payload);
    
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
uint8_t iniciar_vecinos_y_rutas(char* id_nodo, nodo_t (&vecinos)[MAX_NODES], rutas_t routeTable[MAX_ROUTES], uint8_t &total_vecinos, size_t size_vecinos, size_t size_rutas)
{
   String str_nombre_nodo=""; 
   char arr[16];
  
   str_nombre_nodo=(String)id_nodo;
   nodo_t nodo_actual;
    nodo_t nodo_vecino;
    
  if (id_nodo == "turpial.0")
  {
  //  Serial.println("es el nodo 0");
   // str_nombre_nodo.toCharArray(nodo_actual.id, 16);
    copy_array_locha(string2char("turpial_0"), nodo_actual.id, 16);
   // str_nombre_nodo=F("turpial_1");
  //  str_nombre_nodo.toCharArray(nodo_vecino.id, 16);
    copy_array_locha(string2char("turpial_1"), nodo_vecino.id, 16);
     uint8_t rpta1=create_neighbor(nodo_vecino.id,vecinos,total_vecinos);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
   
  }
  if (id_nodo == "turpial.1")
  {
    nodo_t nodo_vecino2;
  
 //   str_nombre_nodo.toCharArray(nodo_actual.id , 16);
    copy_array_locha(id_nodo, nodo_vecino.id, 16);
  //  str_nombre_nodo=F("turpial_0");
  //  str_nombre_nodo.toCharArray(nodo_vecino.id , 16);
     copy_array_locha(string2char("turpial_0"), nodo_vecino.id, 16);
    uint8_t rpta2=create_neighbor(nodo_vecino.id,vecinos,total_vecinos);
  //  str_nombre_nodo=F("turpial_2");
  //  str_nombre_nodo.toCharArray(nodo_vecino2.id, 16);
     copy_array_locha(string2char("turpial_2"), nodo_vecino2.id, 16);
   // nodo_vecino2.id = nombre_nodo;
    
    
    uint8_t rpta3=create_neighbor(nodo_vecino2.id,vecinos,total_vecinos);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
    // ruta T2
    create_route(nodo_actual, nodo_vecino2, nodo_vecino2);
  }
  
  if (id_nodo == "turpial.2")
  {
   
  //  str_nombre_nodo.toCharArray(nodo_actual.id, 16);
  //  str_nombre_nodo=F("turpial_1");
  //  str_nombre_nodo.toCharArray(nodo_vecino.id, 16);
    //strcpy(arr, "turpial_1");
    copy_array_locha(string2char("turpial_1"), nodo_vecino.id, 16);
    //nodo_vecino.id=arr;
    uint8_t rpta=create_neighbor(nodo_vecino.id,vecinos,total_vecinos);
    // ruta T2
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
  }
  return 0;
}



uint8_t show_debugging_info(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos){

  #ifdef DEBUG
    uint8_t rpta;
    String str_buffer_serial_received="";
    String mensaje="";
    bool ejecute=false;
   
    if (Serial.available()) {
      str_buffer_serial_received=Serial.readStringUntil('\n');
      str_buffer_serial_received.toUpperCase();
      str_buffer_serial_received.replace("  "," ");  // se elimina cualquier doble espacio en el input
      str_buffer_serial_received.trim();
    
        Serial.flush();
        mensaje=F("SHOW ROUTES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN("CMD>"+mensaje);
          uint8_t rpta=mostrar_rutas(id_node,routeTable, sizeof(routeTable));  
          ejecute=true;
        }
        mensaje=F("SHOW NODES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN("CMD>"+mensaje);
          uint8_t rpta=mostrar_vecinos(id_node,vecinos,total_vecinos);  
          ejecute=true;
        }
        mensaje=F("SHOW QUEUE");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN("CMD>"+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_cola_mensajes(mensajes_salientes, sizeof(mensajes_salientes));  
          ejecute=true;
        }
        
        mensaje=F("LOAD DEMO");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN("CMD>"+mensaje);
            uint8_t rpta=iniciar_vecinos_y_rutas(id_node, vecinos, routeTable,total_vecinos,sizeof(vecinos),sizeof(routeTable));
            DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
            DEBUG_PRINTLN("CMD>"+mensaje);
            ejecute=true;
            
         }
        mensaje=F("CLEAR ALL");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN("CMD>"+mensaje);
            uint8_t rpta=vaciar_tablas();
            ejecute=true;
         }
          mensaje=F("INFO");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            // se muestra el estatus de la memoria del equipo
            DEBUG_PRINTLN("");
            DEBUG_PRINT(F("Program storage free:"));
            DEBUG_PRINT(freeMemory());
            DEBUG_PRINTLN(" bytes");
            DEBUG_PRINT(F("Memory for local variables:"));
            DEBUG_PRINT(freeRam());
            DEBUG_PRINTLN(" bytes");
            #ifndef MCU_ESP32   // no aplica para los ESPXX
              #ifndef TEENSY36 
                DEBUG_PRINT(F("Vcc:"));
                DEBUG_PRINT(readVcc()/1000);
                DEBUG_PRINTLN(F(" volts"));
              #endif
            #endif
            ejecute=true;
         }
        mensaje=F("CREATE BLE INCOMING");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
    //        DEBUG_PRINT("recibi por parametro:");
      //      DEBUG_PRINTLN(str_param_received);
            rxValue=str_param_received;
            str_buffer_serial_received="";
          //  DEBUG_PRINTLN(mensaje+F("TODO emular recepcion de un mensaje BLE "));
            DEBUG_PRINTLN((String)mensaje+F(" ")+MSG_OK);
            DEBUG_PRINTLN("CMD>"+mensaje);
            ejecute=true;
         }
         
         mensaje=F("CREATE BLE OUTCOMING");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
//            DEBUG_PRINT("recibi por parametro:");
  //          DEBUG_PRINTLN(str_param_received);
            txValue=str_param_received;
            DEBUG_PRINTLN(mensaje+F(" ")+MSG_OK);
            str_buffer_serial_received="";
            DEBUG_PRINTLN("CMD>"+mensaje);
            
            ejecute=true;
         }
         mensaje=F("SHOW BLE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
              
            DEBUG_PRINT("txValue=");
            DEBUG_PRINTLN(txValue);
            DEBUG_PRINT("rxValue=");
            DEBUG_PRINTLN(rxValue);
            
            DEBUG_PRINTLN("CMD>"+mensaje);
            
            ejecute=true;
         }
         mensaje=F("CREATE NODE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              DEBUG_PRINTLN("CMD>"+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                
                uint8_t rpta=create_neighbor(str_node_name,vecinos,total_vecinos);
                DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN("CMD>"+mensaje);
               
                ejecute=true;
                return 1;
              }  else {
                  DEBUG_PRINTLN((String)mensaje+" "+MSG_FAIL);
              }
              str_buffer_serial_received="";
         }

         mensaje=F("CREATE PACKET INCOMING");   // formato: CREATE PACKET INCOMING TYPE FROM PAYLOAD 
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                DEBUG_PRINTLN("CMD>"+mensaje);
                String str_type = getparamValue(str_buffer_serial_received, ' ', 3);  
                String str_from = getparamValue(str_buffer_serial_received, ' ', 4);  
                String str_payload = getparamValue(str_buffer_serial_received, ' ', 5);  

               
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(str_type), string2char_node_name(str_from),string2char_node_name(id_node) , string2char(str_payload));
                
                DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN("CMD>"+mensaje);
               
                ejecute=true;
                return 1;
         }
          mensaje=F("CREATE PACKET OUTCOMING");   // formato: CREATE PACKET OUTCOMING TYPE TO PAYLOAD 
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                DEBUG_PRINTLN("CMD>"+mensaje);
                String str_type = getparamValue(str_buffer_serial_received, ' ', 3);  
                String str_to = getparamValue(str_buffer_serial_received, ' ', 4);  
                String str_payload = getparamValue(str_buffer_serial_received, ' ', 5);  
                Serial.println(F("voy a entrar"));
               
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(str_type), string2char_node_name(id_node),string2char_node_name(str_to), string2char(str_payload));
               Serial.println(F("sali"));
                packet_to_send(Buffer_packet);  // se envia a la cola de mensajes salientes
                
                DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN("CMD>"+mensaje);
               
                ejecute=true;
                return 1;
         }
          mensaje=F("SHOW PACKET");  
           Serial.println(F("sigoooo"));
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
          Serial.println(F("voyyyyyy"));
                  DEBUG_PRINTLN("CMD>"+mensaje);

                  uint8_t rpta=mostrar_packet(Buffer_packet);
                    
                  DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
                  mensaje="";
                  DEBUG_PRINTLN("CMD>"+mensaje);
               
                ejecute=true;
                return 1;
         }
         
         mensaje=F("CLEAR SCREEN");  // falta probar este comando
         if (str_buffer_serial_received==mensaje){
           
            str_buffer_serial_received="";
            Serial.write(12);
            DEBUG_PRINTLN((String)mensaje+" "+MSG_OK);
                DEBUG_PRINTLN("CMD>"+mensaje);
         }
         }
         if (ejecute){
            DEBUG_PRINTLN(">");
         } else{
          if (mensaje!=""){
            DEBUG_PRINTLN((String)str_buffer_serial_received+" "+MSG_FAIL);
          }
         }
           #endif   // del #ifdef DEBUG
         return 0;
         }
        
  
