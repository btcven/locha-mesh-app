/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

// declaracion de librerias
#include <Arduino.h>
#include "debugging.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "general_functions.h"
#include "boards_def.h"
#include "memory_def.h"
#include "blacklist.h"
#include "radio.h"
#include "bluetooth.h"
#include "route.h"


//declaracion de variables
extern std::string txValue;
extern std::string rxValue;
extern std::string txValue_Lora;
extern std::string rxValue_Lora;
extern char* id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist_nodes[MAX_NODES_BLACKLIST];
extern rutas_blacklisted_t blacklist_routes[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 
extern uint8_t total_mensajes_waiting;
extern uint8_t total_rutas_blacklist; 
extern uint8_t total_nodos_blacklist;
extern String remote_debugging;


// Esta funcion imprime la salida por el serial y/o por el BLE
void imprimir_salida(String mensaje, bool mostrar_serial, bool mostrar_BLE){
  if (mostrar_serial){
    DEBUG_PRINT(mensaje);
  }
  if (mostrar_BLE){
    txValue=mensaje.c_str();
  }
}
void imprimir_salidaln(String mensaje, bool mostrar_serial, bool mostrar_BLE){
  imprimir_salida(mensaje, mostrar_serial, mostrar_BLE);
  if (mostrar_serial){
    DEBUG_PRINT('\n');
  }
  if (mostrar_BLE){
    txValue='\n';
  }
}


String getparamValue(String data, char separator, int index)
{
    uint8_t found = 0;
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
uint8_t delete_packet(uint8_t id_to_delete, message_queue_t (&mensajes_salientes_tmp)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes_tmp){
    uint8_t i;
     if (id_to_delete>0){
      
      for (i = id_to_delete; i < total_mensajes_salientes_tmp; i++) {
          mensajes_salientes_tmp[i]=mensajes_salientes_tmp[i+1];
      }
      total_mensajes_salientes_tmp--;
     
     }
      return 0;
    
}

// muestra el contenido del paquete recibido por parametro
uint8_t show_packet(packet_t el_packet, bool show_size){
    
  
  //  String tipo=convertir_packet_type_e_str(el_packet.header.type);
  //  String from=(String)el_packet.header.from;
  //  String to=(String)el_packet.header.to;
  //  String timest=(String)el_packet.header.timestamp;
  //  String payload=(String)el_packet.body.payload;

 if (show_size){
    DEBUG_PRINTLN("");
    DEBUG_PRINT(F("Packet contains:"));
    DEBUG_PRINTLN("");
 }
    
    DEBUG_PRINT(F("Type:"));
    DEBUG_PRINT(convertir_packet_type_e_str(el_packet.header.type));
    DEBUG_PRINT(F(" From:"));
    DEBUG_PRINT(el_packet.header.from);
    DEBUG_PRINT(F(" To:"));
    DEBUG_PRINT(el_packet.header.to);
    DEBUG_PRINT(F(" Payload:"));
    DEBUG_PRINT(el_packet.body.payload);
    DEBUG_PRINT(F(" Timestamp:"));
    DEBUG_PRINT(el_packet.header.timestamp);
    
     
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
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Vecinos del nodo: "));
  DEBUG_PRINTLN(node_id);
  DEBUG_PRINTLN();
  if (total_vecinos>0){
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
  for (i = 1; i <= total_vecinos; i++) {
    DEBUG_PRINT(F("ID "));
    DEBUG_PRINT(i);
    DEBUG_PRINT(F(" : "));
    DEBUG_PRINT(F("\t"));
    DEBUG_PRINTLN(vecinos[i].id);
  }
   DEBUG_PRINT(F("Tamaño de la tabla de vecinos :"));
   DEBUG_PRINT(tamano_arreglo);
   DEBUG_PRINTLN(F(" bytes"));
   DEBUG_PRINT(F("Total de vecinos: "));
   DEBUG_PRINTLN(total_vecinos);
   DEBUG_PRINTLN();
    return 0;   
  } else {
    return 1;
  }
}


uint8_t mostrar_blacklist(char* node_id, nodo_t blacklist_nodes[MAX_NODES_BLACKLIST], size_t total_nodos_blacklist, std::string tipo){
  uint8_t i;
  
  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Blacklist Nodes"));
  //DEBUG_PRINT(tipo.c_str());     // candidato al error 
  DEBUG_PRINT(F(": "));
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
  //  id_temporal=blacklist_nodes[i].id;    // candidato al error aqui deberia ser un copy_array_locha
    DEBUG_PRINTLN(blacklist_nodes[i].id);
  }
   DEBUG_PRINT(F("Blacklist nodes"));
 //  DEBUG_PRINT(tipo.c_str());    // candidato al error 
   DEBUG_PRINT(F(" table size:"));
   DEBUG_PRINT(sizeof(blacklist_nodes));
   DEBUG_PRINTLN(F(" bytes"));
   DEBUG_PRINT(F("Total Nodes "));
 //  DEBUG_PRINT(tipo.c_str());   // candidato al error 
   DEBUG_PRINT(F(": "));
   DEBUG_PRINTLN(total_nodos_blacklist);
   DEBUG_PRINTLN();
  return 0;   
}

uint8_t mostrar_blacklist_routes(char* node_id, rutas_blacklisted_t blacklist_routes[MAX_NODES_BLACKLIST], size_t total_nodos_blacklist, std::string tipo){
  uint8_t i;

  DEBUG_PRINTLN();
  DEBUG_PRINT(F("Blacklist Routes"));
 // DEBUG_PRINT(tipo.c_str());
  DEBUG_PRINT(F(": "));
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
 //   id_temporal=blacklist_routes[i].from;
    DEBUG_PRINTLN(blacklist_routes[i].from);
    DEBUG_PRINT(F("->"));
   // id_temporal=blacklist_routes[i].to;
    DEBUG_PRINTLN(blacklist_routes[i].to);
    
  }
   DEBUG_PRINT(F("Blacklist Route"));
  // DEBUG_PRINT(tipo.c_str());
   DEBUG_PRINT(F(" table size:"));
   DEBUG_PRINT(sizeof(blacklist_routes));
   DEBUG_PRINTLN(F(" bytes"));
   DEBUG_PRINT(F("Total Routes"));
 //  DEBUG_PRINT(tipo.c_str());
   DEBUG_PRINT(F(": "));
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
  DEBUG_PRINT(F("Rutas disponibles en el nodo: "));
  DEBUG_PRINT(node_id);
  DEBUG_PRINTLN();
   for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
  DEBUG_PRINTLN();
  if (total_rutas>0){
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
      DEBUG_PRINT(id_temporal_origen);
      DEBUG_PRINT(F(" -> "));
      if (!(compare_char(id_temporal_destino,id_temporal_next_neighbor))){
          DEBUG_PRINT(id_temporal_next_neighbor);
          DEBUG_PRINT(F(" -> "));
      }
     DEBUG_PRINT(id_temporal_destino);
     DEBUG_PRINT(F("\t"));
     DEBUG_PRINT(F("Age: "));
     DEBUG_PRINTLN(routeTable[i].age);
    DEBUG_PRINT(F("RSSI: "));
     DEBUG_PRINTLN(routeTable[i].RSSI_packet);
    DEBUG_PRINT(F("SNR: "));
     DEBUG_PRINTLN(routeTable[i].SNR_packet); 
    }
    DEBUG_PRINT(F("Tamaño de la tabla de rutas :"));
    DEBUG_PRINT(tamano_arreglo);
    DEBUG_PRINTLN(F(" bytes"));
    DEBUG_PRINT(F("Total de rutas :"));
    DEBUG_PRINTLN(total_rutas);
    DEBUG_PRINTLN();
    return 0;
  } else { 
    return 1;
  }
}




uint8_t mostrar_cola_mensajes(message_queue_t (mensajes_encola)[MAX_MSG_QUEUE], uint8_t total_de_registros, size_t tamano_arreglo){
  uint8_t i;
  uint8_t rptsx;
  
   DEBUG_PRINTLN();
   DEBUG_PRINT(F("Cola mensajes: "));
   DEBUG_PRINTLN();
   if (total_de_registros>0){
   for (i = 1; i <= 95; i++) {
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
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("\t"));
DEBUG_PRINT(F("Intentos de envio"));
      DEBUG_PRINT(F("\t"));
      DEBUG_PRINT(F("Timestamp"));
      DEBUG_PRINT(F("\t"));
     
      DEBUG_PRINTLN();
         for (i = 1; i <= 80; i++) {
          DEBUG_PRINT(F("-"));
      }
      DEBUG_PRINTLN();
     for (i = 1; i <= total_de_registros; i++) {
          
          DEBUG_PRINT(i);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_encola[i].prioridad);
          DEBUG_PRINT(F("\t"));
          rptsx=show_packet(mensajes_encola[i].paquete, false);
          if (rptsx>0){
                DEBUG_PRINT(F("Fail in show_packet"));
          }
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_encola[i].retries);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT(mensajes_encola[i].retry_timestamp);
          DEBUG_PRINT(F("\t"));
          
          
          DEBUG_PRINTLN();
          
     }
    DEBUG_PRINT(F("Tamaño de la cola:"));
    DEBUG_PRINT(tamano_arreglo);
    DEBUG_PRINTLN(F(" bytes"));
    DEBUG_PRINT(F("Total de paquetes en cola :"));
    DEBUG_PRINTLN(total_de_registros);
    DEBUG_PRINTLN();
     
     return 0;
   } else {
    DEBUG_PRINTLN(F("No hay mensajes en la cola"));
    return 1;
   }
}

uint8_t process_debugging_command(String str_buffer_serial_received, bool &ejecute){
  
       String mensaje="";
        mensaje=F("SHOW ROUTES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          uint8_t rpta=mostrar_rutas(id_node,routeTable, sizeof(routeTable));  
          if (rpta==0){
            ejecute=true;
            return 1;
          } else {
            ejecute=false;
          }
        }
        
        mensaje=F("SHOW NODES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received="";
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          uint8_t rpta=mostrar_vecinos(id_node,vecinos,sizeof(vecinos));  
          if (rpta==0){
            ejecute=true;
            return 1;
          } else {
            ejecute=false;
          }
        }
        
        mensaje=F("SHOW QUEUE");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          DEBUG_PRINTLN(F("Cola de mensajes salientes:"));
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_cola_mensajes(mensajes_salientes, total_mensajes_salientes,sizeof(mensajes_salientes));  
          if (rpta==0){
            ejecute=true;
            return 1;
          } else {
            ejecute=false;
          }
          }
         
        mensaje=F("SHOW WAITING");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          DEBUG_PRINTLN(F("Cola de mensajes esperando reintento/ack:"));
          uint8_t rpta=mostrar_cola_mensajes(mensajes_waiting, total_mensajes_waiting,sizeof(mensajes_waiting));  
          ejecute=true;
          return 1;
          }
     
        mensaje=F("BLACKLIST NODES SHOW");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received=F("SHOW BLACKLIST NODES");
        }
        mensaje=F("BLACK NODES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received=F("SHOW BLACKLIST NODES");
        }
        mensaje=F("SHOW BLACKLIST NODES");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_blacklist(id_node, blacklist_nodes, total_nodos_blacklist,"Nodes");
          ejecute=true;
          return 1;
        }

        mensaje=F("SHOW BLACK ROUTES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received=F("BLACKLIST ROUTES SHOW");
        }
        mensaje=F("BLACK ROUTES");
        if (str_buffer_serial_received==mensaje){
          str_buffer_serial_received=F("BLACKLIST ROUTES SHOW");
        }
        mensaje=F("BLACKLIST ROUTES SHOW");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_blacklist_routes(id_node, blacklist_routes, total_rutas_blacklist,"Routes");
          ejecute=true;
          return 1;
        }
        
        mensaje=F("CLEAR ALL");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            uint8_t rpta=vaciar_tablas();
            ejecute=true;
            return 1;
         }
        
         mensaje=F("SYSTEM RESET");
         if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            broadcast_bye(id_node,vecinos,total_vecinos, mensajes_salientes,total_mensajes_salientes);
            ESP.restart();
            return 1;
         }
         
         mensaje=F("BLE CREATE INCOMING");
         if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
            rxValue=str_param_received.c_str();
            str_buffer_serial_received="";
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
         }

         mensaje=F("BLE TEST");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            txValue="123456789012345678901234567890";
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
         }

         mensaje=F("BLE CREATE OUTCOMING");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
            txValue=str_param_received.c_str();
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
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
            return 1;
         }

         mensaje=F("MSG RADIO");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            String msg_received_for_radio = getparamValue(str_buffer_serial_received, ' ', 2);  
            str_buffer_serial_received="";
            DEBUG_PRINT(F("Voy a enviar MSG debugging:"));
            DEBUG_PRINTLN(msg_received_for_radio);
            uint8_t rpta_rad=radioSend(msg_received_for_radio.c_str());
             if (rpta_rad==0){ 
              DEBUG_PRINTLN(F("Mensaje no enviado, radio no disponible"));
             }
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
         }
         mensaje=F("BLE SHOW");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINT("txValue=");
            DEBUG_PRINTLN(txValue.c_str());
            DEBUG_PRINT("rxValue=");
            DEBUG_PRINTLN(rxValue.c_str());
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
         }

         mensaje=F("BLE INFO");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINT(F("enviando info al BLE"));
            String a_enviar=(String)id_node;
            txValue=a_enviar.c_str();
            
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            return 1;
         }

         mensaje=F("SYSTEM INFO");
         if (str_buffer_serial_received==mensaje){
             str_buffer_serial_received="";
             // se muestra el estatus de la memoria del equipo
             DEBUG_PRINTLN("");
             DEBUG_PRINT(F("Node ID:"));
             DEBUG_PRINTLN(id_node);
            DEBUG_PRINT(F("Unique ID (NOTA:no esta correcto):"));
            String id_device_detected=get_id_mac();
             DEBUG_PRINTLN(id_device_detected);
            
            str_buffer_serial_received="";
            ejecute=true;
            return 1;
         }
                 
         mensaje=F("NODE CREATE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                
                uint8_t rptax=create_neighbor(string2char(str_node_name),vecinos,total_vecinos,blacklist_nodes,total_nodos_blacklist);
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
         mensaje=F("NODE SETNAME");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                uint8_t rpta=vaciar_tablas();
                char nombre_tmp[SIZE_IDNODE];
                str_node_name.toCharArray(nombre_tmp,SIZE_IDNODE);
                copy_array_locha(nombre_tmp,id_node,SIZE_IDNODE);

                // se manda un mensaje por Lora tipo HELLO para que los vecinos lo identifiquen y le hagan JOIN
               DEBUG_PRINTLN(F("Enviando mensaje HELLO para mis vecinos"));
                radioSend(packet_serialize(construct_packet_HELLO(id_node)));
                    
        // se coloca el radio nuevamente en modo receives (se hace por segunda vez porque detectamos algunos casos en donde el radio no cambio de modo dentro del radioSend()
        LoRa.receive();
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
                
                uint8_t rpta=blacklist_node_add((char*)str_node_name.c_str(),vecinos, total_vecinos, blacklist_nodes, total_nodos_blacklist , routeTable, total_rutas);
                  
                }
                DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
                mensaje="";
                DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
               str_buffer_serial_received="";
                ejecute=true;
                return 1;
         } 


         mensaje=F("BLACKLIST ROUTE");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
              String str_node_name2 = getparamValue(str_buffer_serial_received, ' ', 3);  
            
              if (str_node_name.length()>0){
                  if (str_node_name2.length()>0){
                      uint8_t rpta= blacklist_route_add((char*)str_node_name.c_str(),(char*)str_node_name2.c_str(),vecinos, total_vecinos, blacklist_routes,  total_rutas_blacklist , routeTable, total_rutas);
                  }
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

               
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(string2char(str_type)), string2char(str_from),id_node, "","", string2char(str_payload));
                
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
             
                Buffer_packet=create_packet(id_node, convertir_str_packet_type_e(string2char(str_type)), string2char(id_node),string2char(str_to), "","", string2char(str_payload));
              
                uint8_t rptad=packet_to_send(Buffer_packet,mensajes_salientes,total_mensajes_salientes);  // se envia a la cola de mensajes salientes
                
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
         
         mensaje=F("PACKET DELETE QUEUE");  
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                  DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                  String id_to_delete = getparamValue(str_buffer_serial_received, ' ', 3); 
                  
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

        mensaje=F("PACKET DELETE WAITING");  
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
                  DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
                  String id_to_delete = getparamValue(str_buffer_serial_received, ' ', 3); 
                  
                  if (isNumeric(id_to_delete)){ 
                      
                      uint8_t rpta=delete_packet(id_to_delete.toInt(),mensajes_waiting,total_mensajes_waiting);
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
            return 1;
         }
// si llega hasta aqui fue que no proceso o no proceso mal algun comando
         return 0;
}


uint8_t show_debugging_info(struct nodo_t (&vecinos)[MAX_NODES], uint8_t &total_vecinos, String &remote_debugging){

 
    uint8_t rpta=0;
    String str_buffer_serial_received="";
    String mensaje="";
    bool ejecute=false;
    
    if (Serial.available()) {
      
      str_buffer_serial_received=Serial.readStringUntil('\n');
      str_buffer_serial_received.toUpperCase();
      str_buffer_serial_received.replace("  "," ");  // se elimina cualquier doble espacio en el input
      str_buffer_serial_received.trim();


      rpta=process_debugging_command(str_buffer_serial_received,ejecute);
     } 
    // tambien puedo recibir un comando por la variable: remote_debugging
    if (remote_debugging.length()>0){
      int pos_remote=remote_debugging.indexOf("REMOTE:");
          if(pos_remote >= 0){
              // es un comando remoto, se filtra y se manda a ejecutar
                      remote_debugging=remote_debugging.substring(pos_remote+7,remote_debugging.length());
                      rpta=process_debugging_command(remote_debugging,ejecute);  
                      // se vaia el comando remoto una vez efectuado
                      remote_debugging="";
                      return rpta;              
          }
        remote_debugging="";
    }
           
    
         if (ejecute){
            DEBUG_PRINTLN(">");
         } else{
          if (mensaje!=""){
            DEBUG_PRINTLN((String)str_buffer_serial_received+MSG_SPACE+MSG_FAIL);
          }
         }

         return rpta;
}
        
  
