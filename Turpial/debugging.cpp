#include <Arduino.h>
#include "debugging.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "general_functions.h"
#include "boards_def.h"
#include "memory_def.h"
#include "blacklist.h"
#include "radio.h"

extern std::string txValue;
extern std::string rxValue;
extern std::string txValue_Lora;
extern std::string rxValue_Lora;

#ifdef BLE_ENABLED
  #include "bluetooth.h"
#endif

extern char* id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[MAX_ROUTES];
extern nodo_t vecinos[MAX_NODES];
extern nodo_t blacklist[MAX_NODES_BLACKLIST];
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_vecinos;
extern uint8_t total_rutas; 
extern uint8_t total_mensajes_salientes; 
extern uint8_t total_mensajes_waiting;
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
  Serial.print("estoy dentro de mostrar_rutas con el id_nodo:");
  Serial.println((String)node_id);
  DEBUG_PRINTLN(MSG_SPACE);
  DEBUG_PRINT("Rutas disponibles en el nodo: ");
  Serial.println("coloque el titulo");
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
    DEBUG_PRINT(F("RSSI: "));
     DEBUG_PRINTLN((String)routeTable[i].RSSI_packet);
    DEBUG_PRINT(F("SNR: "));
     DEBUG_PRINTLN((String)routeTable[i].SNR_packet); 
    }
    DEBUG_PRINT(F("Tamaño de la tabla de rutas :"));
    DEBUG_PRINT(tamano_arreglo);
    DEBUG_PRINTLN(F(" bytes"));
    DEBUG_PRINT(F("Total de rutas :"));
    DEBUG_PRINTLN(total_rutas);
    DEBUG_PRINTLN();
    return 0;
}




uint8_t mostrar_cola_mensajes(message_queue_t (mensajes_encola)[MAX_MSG_QUEUE], uint8_t total_de_registros, size_t tamano_arreglo){
  uint8_t i;
  uint8_t j;
    uint8_t rptsx;
    
   DEBUG_PRINTLN();
   DEBUG_PRINT(F("Cola mensajes: "));
   DEBUG_PRINTLN();
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
          DEBUG_PRINT((String)mensajes_encola[i].prioridad);
          DEBUG_PRINT(F("\t"));
          rptsx=show_packet(mensajes_encola[i].paquete, false);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT((String)mensajes_encola[i].retries);
          DEBUG_PRINT(F("\t"));
          DEBUG_PRINT((String)mensajes_encola[i].retry_timestamp);
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
}

// funcion para llenar manualmente los datos del modelo demo en la tabla vecinos y rutas
uint8_t iniciar_vecinos_y_rutas(char* id_nodo, nodo_t (&vecinos)[MAX_NODES], rutas_t routeTable[MAX_ROUTES], uint8_t &total_vecinos, size_t size_vecinos, size_t size_rutas)
{
  String str_nombre_nodo=""; 
  char arr[16];
  
  str_nombre_nodo=(String)id_nodo;
  nodo_t nodo_actual;
  nodo_t nodo_vecino;

  char* id_nodo_demo0="TURPIAL.0";
  //char const *id_nodo_demo0 = "TURPIAL.0"; // valid and safe in either C or C++.
  char* id_nodo_demo1="TURPIAL.1";
 // char const *id_nodo_demo1 = "TURPIAL.1"; // valid and safe in either C or C++.
  char* id_nodo_demo2="TURPIAL.2";
  //char const *id_nodo_demo2 = "TURPIAL.2"; // valid and safe in either C or C++.
  
//strcpy (id_nodo_demo0,"TURPIAL.0");
//strcpy (id_nodo_demo1,"TURPIAL.1");
//strcpy (id_nodo_demo2,"TURPIAL.2");


#define OLED_SCREEN_INTERVAL 4000 


   // se colocan los id_nodo en mayusculas
 // id_nodo_demo0=char_to_uppercase(id_nodo_demo0, 16);
 // id_nodo_demo1=char_to_uppercase(id_nodo_demo1, 16);
 //   id_nodo_demo2=char_to_uppercase(id_nodo_demo2, 16);
  
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


uint8_t process_debugging_command(String str_buffer_serial_received, bool &ejecute){
  
       String mensaje="";
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
          DEBUG_PRINTLN(F("Cola de mensajes salientes:"));
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_cola_mensajes(mensajes_salientes, total_mensajes_salientes,sizeof(mensajes_salientes));  
          ejecute=true;
          }
         
        
        mensaje=F("SHOW WAITING");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          DEBUG_PRINTLN(F("Cola de mensajes esperando reintento/ack:"));
          uint8_t rpta=mostrar_cola_mensajes(mensajes_waiting, total_mensajes_waiting,sizeof(mensajes_waiting));  
          ejecute=true;
          }
     mensaje=F("SHOW BLACKLIST");
        if (str_buffer_serial_received==mensaje){
          DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
          str_buffer_serial_received="";
          uint8_t rpta=mostrar_blacklist(id_node, blacklist, sizeof(blacklist));
          ejecute=true;
        }
         mensaje=F("CLEAR ALL");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            uint8_t rpta=vaciar_tablas();
            ejecute=true;
         }
         mensaje=F("SYSTEM RESET");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ESP.restart();
           // ejecute=true;  
         }
         
        mensaje=F("BLE CREATE INCOMING");
        if (str_buffer_serial_received.substring(0,mensaje.length())==mensaje){
            
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
            rxValue=str_param_received.c_str();
            str_buffer_serial_received="";
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
         }


         
         mensaje=F("BLE TEST");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            String str_param_received = "123456789012345678901234567890";  
            //txValue=str_param_received.c_str();
            txValue="123456789012345678901234567890";
            Serial.println("");
            Serial.print("convertido:");
            Serial.println(txValue.c_str());
            delay(100);
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
         }

         
         mensaje=F("BLE CREATE OUTCOMING");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            String str_param_received = getparamValue(str_buffer_serial_received, ' ', 3);  
            txValue=str_param_received.c_str();
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            str_buffer_serial_received="";
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

         mensaje=F("MSG RADIO");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            String msg_received_for_radio = getparamValue(str_buffer_serial_received, ' ', 2);  
            uint8_t rpta_rad=radioSend(msg_received_for_radio.c_str());
             if (rpta_rad==0){ 
              DEBUG_PRINTLN(F("Mensaje no enviado, radio no disponible"));
             }
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
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
         }

mensaje=F("BLE INFO");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINT("enviando info al BLE");
            String a_enviar=(String)id_node;
            txValue=a_enviar.c_str();
            DEBUG_PRINTLN(mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
         }

          mensaje=F("LOAD DEMO");
        if (str_buffer_serial_received==mensaje){
            str_buffer_serial_received="";
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
           
            // primero se vacian las tablas para que el ambiente quede solo para el demo
             uint8_t rpta=vaciar_tablas();
          
            rpta=iniciar_vecinos_y_rutas(id_node, vecinos, routeTable,total_vecinos,sizeof(vecinos),sizeof(routeTable));
            // se manda un mensaje por Lora tipo HELLO para que los vecinos lo identifiquen y le hagan JOIN
            DEBUG_PRINTLN(F("Enviando mensaje HELLO para mis vecinos"));
            radioSend(packet_serialize(construct_packet_HELLO(id_node)));
            
            DEBUG_PRINTLN((String)mensaje+MSG_SPACE+MSG_OK);
            DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
            ejecute=true;
            
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
         mensaje=F("NODE SETNAME");
         if (str_buffer_serial_received.substring(0, mensaje.length())==mensaje){
              // getparamValue (0) devuelve CREATE , (1) devuelve NODE , (2) devuelve el nombre el nodo recibido por parametro
              DEBUG_PRINTLN(MSG_COMMAND_LINE+mensaje);
              String str_node_name = getparamValue(str_buffer_serial_received, ' ', 2);  
            
              if (str_node_name.length()>0){
                uint8_t rpta=vaciar_tablas();
                char nombre_tmp[16];
                str_node_name.toCharArray(nombre_tmp,16);
                copy_array_locha(nombre_tmp,id_node,16);

                // se manda un mensaje por Lora tipo HELLO para que los vecinos lo identifiquen y le hagan JOIN
               DEBUG_PRINTLN(F("Enviando mensaje HELLO para mis vecinos"));
                radioSend(packet_serialize(construct_packet_HELLO(id_node)));
                    
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
        
  
