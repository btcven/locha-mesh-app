/**
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/

/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#include <Arduino.h>
#include <BLEServer.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Time.h>
#include <TimeLib.h>
#include "general_functions.h"
#include "bluetooth.h"
#include "radio.h"
#include "route.h"
#include "debugging.h"

extern char *id_node;
extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes; 
extern message_queue_t mensajes_waiting[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_waiting;
extern uint8_t total_rutas;
extern rutas_t routeTable[MAX_ROUTES];

extern String packet_return_BLE_str;
extern String packet_return_Lora_str;
extern String remote_debugging;

extern not_delivered_type_e why_not_delivered;   // causa de no entrega de algun packet

BLEServer *ble_server = NULL;
BLECharacteristic *tx_uart;
BLECharacteristic *rx_uart;

 std::string server_name = "mesh.locha.io";

extern std::string txValue;
extern std::string rxValue;

// variables para trasmision Lora
extern std::string rxValue_Lora;
extern std::string txValue_Lora;

extern char *uid ;
extern char *msg;
extern double timemsg;
extern char *hash_msg ;

bool deviceConnected = false;

class ServerCB : public BLEServerCallbacks
{
    void onConnect(BLEServer *ble_server)
    {
      deviceConnected = true;
    }
    void onDisconnect(BLEServer *ble_server)
    {
      deviceConnected = false;
      Serial.println("[BLE] restart advertising...");
      delay(500);
      ble_server->startAdvertising();
    }
};

class characteristicCB : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
     char* uid_temporal = NULL;
      char* msg_temporal = NULL;
      char* hash_temporal = NULL;
      char* time_temporal= NULL;
      
      // movil -> ble_server(Turpial)
      rxValue = pCharacteristic->getValue();
      // si tenemos datos podemos enviarlos via radio desde aqui.
      if (rxValue.size() > 0)
      {
        DEBUG_PRINT("[BLE] Received:");
        DEBUG_PRINTLN(rxValue.c_str());
       // radioSend(rxValue);   // el enviar por radio va en el main loop en packet_processing_outcoming(), enviar por radio lo que deberia hacer es leer la tabla mensajes_salientes y enviar de a un registro a la vez
        // por que el procesamiento de paquetes es FIFO y si hay paquetes esperando por salir por la radio Lora no puede salir primero un paquete BLE hacia la radio Lora
        // se procesa el ble incoming
        String parametro;
        parametro=String(rxValue.c_str());    // se convierte de string c null terminated a System:String
        // el siguiente void extrae del String BLE los 3 parametros: uid,msg,time
   
   json_receive(parametro,uid_temporal,msg_temporal,time_temporal,hash_temporal);
        DEBUG_PRINTLN(F("Json received:"));
        DEBUG_PRINT("uid:");
        DEBUG_PRINTLN(uid_temporal);
        DEBUG_PRINT("msg:");
        DEBUG_PRINTLN(msg_temporal);
        DEBUG_PRINT("time:");
        DEBUG_PRINTLN(time_temporal);
        DEBUG_PRINT("hash:");
        DEBUG_PRINTLN(hash_temporal);
        // se procesa el comando que se haya recibido por BLE
        
         // se valida el hash del msg para ver si esta integro
        if (is_valid_hash160(msg_temporal, hash_msg)){
          // se verifica la hora recibida por Bluetooth para saber si hay que sincronizarla
        //  if (elapsedDays((uint32_t) time_temporal)<elapsedDays()){
            uint32_t epoch_recibido=(uint32_t) time_temporal;
            setTime(epoch_recibido/1000);
          // se verifica si viene un comando remoto para el turpial por BLE
          String msg_tmp=(String)msg_temporal;
          msg_tmp.toUpperCase();
          int pos_remote=msg_tmp.indexOf("REMOTE:");

          if(pos_remote >= 0){
                DEBUG_PRINT(F("Remote command received:"));
                DEBUG_PRINTLN(msg_tmp);
                remote_debugging=(String)msg_tmp;
          } else {
                DEBUG_PRINTLN(F("antes de comenzar BLE incoming:"));
                
                BLE_incoming(uid_temporal,msg_temporal,time_temporal,hash_temporal,mensajes_salientes,total_mensajes_salientes,routeTable,total_rutas);   // este procesamiento coloca los paquetes broadcast en la cola de mensajes salientes, la cola se procesa en el main loop 
                DEBUG_PRINTLN(F("BLE process OK"));
          }
        } else { 
          // no es valido el hash del mensaje
          DEBUG_PRINTLN(F("Invalid hash received on BLE message"));
        }
         rxValue.clear();
        // se vacia el buffer BLE
      }
    }
    void onRead(BLECharacteristic *pCharacteristic)
    {
           DEBUG_PRINTLN(F("en onRead"));
           if (deviceConnected) {
//            Serial.println(F("enviando al BLE desde onRead"));
            pCharacteristic->setValue(txValue.c_str());
            pCharacteristic->notify();
  //          Serial.print(F("listo el envio al BLE, texto enviado:"));
   //         Serial.println(txValue.c_str());
            delay(100);
            txValue.clear();
        } else {
          DEBUG_PRINTLN(F("No hay dispositivo conectado"));
        }
    }

};




void task_bluetooth(void *params)
{
   String packet_in_process_str;
    String text_to_send;
    uint8_t jj;
    
  // asignamos el nombre al servidor
  // este será el que se mostrará en la app movil al escanear
  // dispositivos cercanos.
  BLEDevice::init(server_name);
  uint16_t mtu = 257;
  BLEDevice::setMTU(mtu);
  
  ble_server = BLEDevice::createServer();
  // asignamos el callback que se disparará cada vez que un cliente
  // conecta o desconecta.
  ble_server->setCallbacks(new ServerCB());
  // el id del servicio que ofrecemos es un UART.
  BLEService *server_service = ble_server->createService(SERVICE_UUID);
  // asignamos la caracteristica TX, la cual enviará al cliente movil nuevos mensajes.
  // desde el punto de vista de la app movil lee datos cuando estén disponibles.
  tx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);

  // sobre GATT puede ser interesante echar un vistazo a los siguientes enlaces:
  // https://www.bluetooth.com/specifications/gatt
  // sobre el descritor 0x2902
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  tx_uart->addDescriptor(new BLE2902());
  // RX lee nuevos mensajes desde la app movil.o dicho de otra forma
  // escribimos desde la app movil sobre la caracteristica.
  rx_uart = server_service->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  // asignamos el callback que se disparará cuando hay datos en la característica RX
  rx_uart->setCallbacks(new characteristicCB());

  // iniciamos el servicio y comenzamos a anunciar
  // el nombre de nuestro servidor.
  server_service->start();
  ble_server->getAdvertising()->start();

 
  // loop
  while (1)
  {
    
    if (deviceConnected)
    {
        if (txValue.size() > 0)
        {
            Serial.print("[BLE] Sending: ");
            packet_in_process_str=txValue.c_str();
            
                 if(packet_in_process_str.indexOf("|") > 0){ 
                  
                        packet_return_BLE_str=packet_in_process_str;      
                 }  else {
                     
                      tx_uart->setValue(txValue.c_str());
                      tx_uart->notify();
                      DEBUG_PRINT(F("Se envio al BLE:"));
                      DEBUG_PRINTLN(txValue.c_str());
                 }
              
                 txValue.clear();
                 text_to_send="";
                
            
            
            
        }
    } else {
      // si hay algo para envio pero no esta el dispositivo conectado
      if (txValue.size()>0){ 
        // solo si hay algo para mandar al BLE pero no hay cliente conectado
          Serial.println(F("No hay dispositivo BLE conectado"));
          why_not_delivered=BLE_NOT_CONNECTED;
          // se devuelve el packet como not delivered
          packet_return_BLE_str=(String)txValue.c_str();
          txValue.clear();
          text_to_send="";
      }
      
    }
    
    
      
      delay(100);
    
  } // WHILE

};



// funcion para procesar un mensaje BLE incoming
void BLE_incoming(char* uid2,char* msg_ble, char* timemsg, char* hash_msg, message_queue_t (&mensajes_salientes)[MAX_MSG_QUEUE], uint8_t &total_mensajes_salientes, rutas_t routeTable[MAX_ROUTES], uint8_t total_rutas){
  uint8_t i;
  uint8_t rpta;
  char *pChar = (char*)"broadcast";
  char *pCharempty = (char*)"";
  char *ptipo_msg=(char*)"MSG";   // todo lo que proviene de BLE genera un packet_type_e=MSG
char nodo_remitente[SIZE_IDNODE];
char nodo_destinatario[SIZE_IDNODE];
char nodo_siguiente[SIZE_IDNODE];
  
  // si es un mensaje tipo broadcast se envia a todos los vecinos 
  DEBUG_PRINTLN(F("Starting BLE_incoming"));
 
    if (compare_char(uid2,pChar)){ 
       DEBUG_PRINTLN(F("its a broadcast"));
       // se envia el packet a todos las rutas , no a todos los vecinos, ya que un vecino puede tener una ruta blacklisted
       if (total_vecinos>0){ 
         for (i = 1; i <= total_rutas; i++) {
          if (compare_char(routeTable[i].origen.id,id_node)){
             copy_array_locha(routeTable[i].origen.id,nodo_remitente,SIZE_IDNODE);
             copy_array_locha("",nodo_destinatario,SIZE_IDNODE);
             copy_array_locha(routeTable[i].next_neighbor.id,nodo_siguiente,SIZE_IDNODE);
          }
          if (compare_char(routeTable[i].destino.id,id_node)){
             copy_array_locha(routeTable[i].destino.id,nodo_remitente,SIZE_IDNODE);
             copy_array_locha("",nodo_destinatario,SIZE_IDNODE);
             copy_array_locha(routeTable[i].next_neighbor.id,nodo_siguiente,SIZE_IDNODE);
          }  

          if (compare_char(nodo_siguiente,pCharempty)){
            copy_array_locha(nodo_destinatario,nodo_siguiente,SIZE_IDNODE);
          }
// se verifica que el destinatario o el next_neigbour no sea un nodo blacklisted
          if (!(es_blacklisted(nodo_destinatario))){
            if (!(es_blacklisted(nodo_siguiente))){
              DEBUG_PRINTLN(F("Se coloca el packet del BLE recibido en la cola de mensajes salientes"));
              // se arma el packet y se envia a cada vecino que exista una ruta
              // solo si existe la ruta se envia al vecino el broadcast, asi se evita enviar packets a rutas blacklisted
              packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e(ptipo_msg), (char*)nodo_remitente, (char*)nodo_destinatario,(char*)nodo_siguiente,hash_msg, msg_ble);
              rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes,total_vecinos, total_rutas);
            }
          }
         }
       } 
    } else {
       // es un packet a un usuario en especifico

            // solo si existe la ruta se envia al vecino el broadcast, asi se evita enviar packets a rutas blacklisted
              packet_t tmp_packet=create_packet(id_node,convertir_str_packet_type_e(ptipo_msg), id_node, uid2,"","", msg_ble);
              rpta=packet_to_send(tmp_packet,mensajes_salientes,total_mensajes_salientes,total_vecinos, total_rutas);
    }
  DEBUG_PRINTLN(F("ready , packet sent to message queue"));
}
