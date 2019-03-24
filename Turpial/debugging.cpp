#include <Arduino.h>
#include "hardware.h"
#include "general_functions.h"
#include "debugging.h"
#include "boards_def.h"
#include "memory_def.h"
#include "packet.h"
#include "route.h"
#include "blacklist.h"
#include "language_es.h"
#if defined(__AVR__) 
  #include "MemoryFree.h"
#endif


extern String rxValue;
extern String txValue;

#ifdef BLE_ENABLED
  #include "bluetooth.h"
#endif

extern char* id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 
extern uint8_t total_nodos_blacklist;




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


// borra un packet de la cola de paquetes salientes
uint8_t delete_packet(uint8_t id_to_delete, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes){
    uint8_t i;
     if (id_to_delete>0){
      
      for (i = id_to_delete; i < total_mensajes_salientes; i++) {
          mensajes_salientes[i]=mensajes_salientes[i+1];
      }
      total_mensajes_salientes--;
     
     }
      return 0;
    
}

// muestra el contenido del paquete recibido por parametro
uint8_t show_packet(packet_t el_packet, bool show_size){
    
  
    String tipo=convertir_packet_type_e_str(el_packet.header.type);
    String from=(String)el_packet.header.from;
    String to=(String)el_packet.header.to;
    String timest=(String)el_packet.header.timestamp;
    String payload=(String)el_packet.body.payload;

 if (show_size){
    DEBUG_PRINTLN("");
    DEBUG_PRINT(F("Packet contains:"));
    DEBUG_PRINTLN("");
 }
    
    DEBUG_PRINT(F("Type:"));
    DEBUG_PRINT(tipo);
    DEBUG_PRINT(F(" From:"));
    DEBUG_PRINT(from);
    DEBUG_PRINT(F(" To:"));
    DEBUG_PRINT(to);
    DEBUG_PRINT(F(" Payload:"));
    DEBUG_PRINT(payload);
    DEBUG_PRINT(F(" Timestamp:"));
    DEBUG_PRINT(timest);
    
     
    if (show_size){
        DEBUG_PRINT(F(" Tamaño del packet :"));
        DEBUG_PRINT(sizeof(el_packet));
        DEBUG_PRINT(F(" bytes"));
        DEBUG_PRINTLN();
    }
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


uint8_t mostrar_blacklist(char* node_id, nodo_t blacklist[MAX_NODES_BLACKLIST], size_t total_nodos_blacklist){
  uint8_t i;
  char* id_temporal;
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Blacklist Node: "));
  DEBUG_PRINTLN(node_id);
  DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
  for (i = 1; i <= total_nodos_blacklist; i++) {
    DEBUG_PRINT(F("ID "));
    DEBUG_PRINT(i);
    DEBUG_PRINT(F(" : "));
    DEBUG_PRINT(F("\t"));
    id_temporal=blacklist[i].id;
    DEBUG_PRINTLN((String)id_temporal);
  }
   DEBUG_PRINT(F("Blacklist Table Size:"));
   DEBUG_PRINT("falta esta info");
   DEBUG_PRINTLN(F(" bytes"));
   DEBUG_PRINT(F("Total de vecinos: "));
   DEBUG_PRINTLN(total_nodos_blacklist);
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
  DEBUG_PRINTLN(MSG_SPACE);
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
      if ((String)id_temporal_destino!=(String)id_temporal_next_neighbor){
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




uint8_t mostrar_cola_mensajes(struct message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], size_t tamano_arreglo){
  uint8_t i;
  uint8_t j;
    
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
      DEBUG_PRINT(F("Packet"));
      DEBUG_PRINT(F("\t"));
      
      DEBUG_PRINTLN();
         for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
     for (i = 1; i <= total_mensajes_salientes; i++) {
          
          DEBUG_PRINT(i);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT((String)mensajes_salientes[i].prioridad);
          DEBUG_PRINT(F("\t"));
          show_packet(mensajes_salientes[i].paquete, false);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINTLN();
          
     }
    DEBUG_PRINT(F("Tamaño de la cola:"));
    DEBUG_PRINT(tamano_arreglo);
    DEBUG_PRINTLN(F(" bytes"));
    DEBUG_PRINT(F("Total de paquetes en cola :"));
    DEBUG_PRINTLN(total_mensajes_salientes);
    DEBUG_PRINTLN();
     
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

  char id_nodo_demo0[]="turpial.0";
  char id_nodo_demo1[]="turpial.1";
  char id_nodo_demo2[]="turpial.2";


//char id_nodo_demo[]="turpial.0";

#define OLED_SCREEN_INTERVAL 4000 


  
  char* id_node=id_nodo_demo0;
  
  if ((String)id_nodo == (String)id_nodo_demo0)
  {
    
      copy_array_locha(id_nodo_demo0, nodo_actual.id, 16);
      copy_array_locha(id_nodo_demo1, nodo_vecino.id, 16);
      uint8_t rpta1=create_neighbor(nodo_vecino.id,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
     
      // ruta T1
      create_route(nodo_actual, nodo_vecino, nodo_vecino);
   
  }
  if ((String)id_nodo == (String)id_nodo_demo1)
  {
    nodo_t nodo_vecino2;
  
    copy_array_locha(id_nodo, nodo_vecino.id, 16);
    copy_array_locha(id_nodo_demo0, nodo_vecino.id, 16);
    uint8_t rpta2=create_neighbor(nodo_vecino.id,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
    copy_array_locha(id_nodo_demo2, nodo_vecino2.id, 16);
    
    uint8_t rpta3=create_neighbor(nodo_vecino2.id,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
    // ruta T1
    create_route(nodo_actual, nodo_vecino, nodo_vecino);
    // ruta T2
    create_route(nodo_actual, nodo_vecino2, nodo_vecino2);
  }
  
  if ((String)id_nodo == (String)id_nodo_demo2)
  {
    copy_array_locha(id_nodo_demo1, nodo_vecino.id, 16);
    uint8_t rpta=create_neighbor(nodo_vecino.id,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
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
    
        //Serial.flush();
        mensaje=F("SHOW ROUTES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          uint8_t rpta=mostrar_rutas(id_node,routeTable, sizeof(routeTable));  
          ejecute=true;
        }
        mensaje=F("SHOW NODES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          uint8_t rpta=mostrar_vecinos(id_node,vecinos,total_vecinos);  
          ejecute=true;
        }
        mensaje=F("SHOW QUEUE");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_cola_mensajes(mensajes_salientes, sizeof(mensajes_salientes));  
          ejecute=true;
        }
        mensaje=F("SHOW BLACKLIST");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_blacklist(id_node, blacklist, sizeof(blacklist));
          ejecute=true;
        }
        
        
        
        mensaje=F("LOAD DEMO");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            // primero se vacian las tablas para que el ambiente quede solo para el demo
             uint8_t rpta=vaciar_tablas();
            rpta=iniciar_vecinos_y_rutas(id_node, vecinos, routeTable,total_vecinos,sizeof(vecinos),sizeof(routeTable));
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            
         }
        mensaje=F("CLEAR ALL");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            uint8_t rpta=vaciar_tablas();
            ejecute=true;
         }
          mensaje=F("INFO");
        if (str_buffer_serial_received==mensaje){
             str_buffer_serial_received="";
             // se muestra el estatus de la memoria del equipo
             DEBUG_PRINTLN("");
             DEBUG_PRINT(F("Node ID:"));
             DEBUG_PRINTLN(id_node);
             #if defined(__AVR__)   // solo aplica para los arduino
                DEBUG_PRINT(F("Program storage free:"));
                DEBUG_PRINT(freeMemory());
                DEBUG_PRINTLN(" bytes");
                DEBUG_PRINT(F("Memory for local variables:"));
                DEBUG_PRINT(freeRam());
                DEBUG_PRINTLN(" bytes");
            
                DEBUG_PRINT(F("Vcc:"));
                DEBUG_PRINT(readVcc()/1000);
                DEBUG_PRINTLN(F(" volts"));
               DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            #endif
            str_buffer_serial_received="";
            ejecute=true;
         }
        mensaje=F("BLE CREATE INCOMING");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
            rxValue=str_param_received;
            str_buffer_serial_received="";
          //  DEBUG_PRINTLN(mensaje+F("TODO emular recepcion de un mensaje BLE "));
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
         }
         
         mensaje=F("BLE CREATE OUTCOMING");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  

            txValue=str_param_received;
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            
            ejecute=true;
         }
         mensaje=F("BLE SHOW");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINT("txValue=");
            DEBUG_PRINTLN(txValue);
            DEBUG_PRINT("rxValue=");
            DEBUG_PRINTLN(rxValue);
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            
            ejecute=true;
         }
         // limpia el buffer BLE (rxValue y txValue)
         mensaje=F("BLE CLEAR");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            txValue="";
            rxValue="";
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            
            ejecute=true;
         }
         mensaje=F("NODE CREATE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                
                uint8_t rptax=create_neighbor(str_node_name,vecinos,total_vecinos,blacklist,total_nodos_blacklist);
                DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
               
                ejecute=true;
                return 1;
              }  else {
                  DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_FAIL);
              }
              str_buffer_serial_received="";
         }

         mensaje=F("BLACKLIST NODE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                
                uint8_t rpta=blacklist_add(str_node_name,vecinos, total_vecinos, blacklist, total_nodos_blacklist , routeTable, total_rutas);
                  
                }
                DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
               str_buffer_serial_received="";
                ejecute=true;
                return 1;
              } 
              
         
         

         mensaje=F("PACKET CREATE INCOMING");   // formato: CREATE PACKET INCOMING TYPE FROM PAYLOAD 
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                String str_type = getparamValue(str_buffer_serial_received, ' ', 3);  
                String str_from = getparamValue(str_buffer_serial_received, ' ', 4);  
                String str_payload = getparamValue(str_buffer_serial_received, ' ', 5);  

               
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(str_type), string2char(str_from),id_node, string2char(str_payload));
                
                DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                str_buffer_serial_received="";
                ejecute=true;
                return 1;
         }
          mensaje=F("PACKET CREATE OUTCOMING");   // formato: CREATE PACKET OUTCOMING TYPE TO PAYLOAD 
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                String str_type = getparamValue(str_buffer_serial_received, ' ', 3);  
                String str_to = getparamValue(str_buffer_serial_received, ' ', 4);  
                String str_payload = getparamValue(str_buffer_serial_received, ' ', 5);  
             
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(str_type), string2char(id_node),string2char(str_to), string2char(str_payload));
              
                packet_to_send(Buffer_packet);  // se envia a la cola de mensajes salientes
                
                DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                str_buffer_serial_received="";
                ejecute=true;
                return 1;
         }
         mensaje=F("PACKET SHOW");  
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                  DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                  uint8_t rpta=show_packet(Buffer_packet,true);
                  DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                  mensaje="";
                  DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                  str_buffer_serial_received="";
                  ejecute=true;
                  return 1;
         }
         mensaje=F("PACKET DELETE");  
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                  DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                  String id_to_delete = getparamValue(str_buffer_serial_received, ' ', 2); 
                  
                  if (isNumeric(id_to_delete)){ 
                      
                      uint8_t rpta=delete_packet(id_to_delete.toInt(),mensajes_salientes,total_mensajes_salientes);
                      if (rpta==0){
                        DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                        mensaje="";
                        DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                        str_buffer_serial_received="";
                        ejecute=true;
                        return 1;
                      } else {
                         DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_FAIL);
                      }
                  } else {
                      DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_FAIL);
                  }
         }
         
         mensaje=F("CLEAR SCREEN");  // falta probar este comando
         if (str_buffer_serial_received==mensaje){
           
            str_buffer_serial_received="";
            Serial.write(12);
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
         }
         
        // Serial.println("sali de la funcion debugging");
         if (ejecute){
            DEBUG_PRINTLN(">");
         } else{
          if (mensaje!=""){
            DEBUG_PRINTLN((String)str_buffer_serial_received+MSG_SPACE+MSG_FAIL);
          }
         }

    }
           #endif   // del #ifdef DEBUG
         return 0;
         }
        
  
