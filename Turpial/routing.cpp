/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#include "general_functions.h"
extern BUFFER_packet_t Buffer_packet;

#define HEADER_LENGTH 16

// tables
struct neighbor_entry_t neighborTable[255];
int neighborEntry = 0;

struct route_to_node_t routeTable[255];
int routeEntry = 0;

// timeout intervals
int _helloInterval = 10;
int _routeInterval = 10;
int _messageInterval = 5;
int _discoveryTimeout = 30;
int timeDistortion = 1;




// se arma el paquete a transmitir
packet_t build_packet(uint8_t max_hops, uint8_t from, uint8_t to, uint8_t sequence[2], String mensaje, packet_type_e type,uint8_t packetLength ){
    uint8_t dataLength = HEADER_LENGTH + packetLength;
    hash=0; // TODO build hash for this package
    struct Packet packet = {
        max_hops,
        dataLength,
        from,
        to,
        sequence[1,1],  // el primero es el numero del fragmento y el segundo es el total de fragmentos TODO esto habria que cambiarlo para manejar fragmentacion
        mensaje,
        hash,
        type 
    };
  return packet;    
}

int convert_msg_into_buffer(String msg_received){
  char splitted[8];
  split(msg_received,",",0,splitted);
  
  Buffer_packet.max_hops=splitted[0];    // max_hops no entra en el calculo de hash.
  Buffer_packet.length=splitted[1];      // longitud del mensaje.
  Buffer_packet.from=splitted[2];        // direccion del remitente
  Buffer_packet.to=splitted[3];          // enviamos el mensaje al id: to
  Buffer_packet.sequence=splitted[4]; // por ejemplo {1, 2} representa parte 1 de 2 en total
  Buffer_packet.payload=splitted[5]; // es la secuencia del mensaje
  Buffer_packet.msg_hash=splitted[6];    // first 4 bytes.
  Buffer_packet.type=splitted[7];
}


int procesar_buffer(){
    // funcion que toma el buffer , revisa su contenido , lo procesa y vacia nuevamente el buffer
    if (Buffer_packet.type==PACKET_HELLO){
      // se le devuelve el info del nodo 
    
      // se coloca como un neighbor
      
      neighbor_entry_t el_vecino;
      el_vecino.id=Buffer_packet.from;         // uniqueid
      el_vecino.offers={1,2,3};       // servicios que ofrece
      el_vecino.age=millis();            // edad conocida
      el_vecino.hops_away=1;      // saltos de distancia (metrica)
      neighborEntry++;
      neighborTable[neighborEntry]=el_vecino;
      // vacio el buffer
      packet_t buffer_vacio;
      Buffer_packet=buffer_vacio;
      // se devuelve la tabla de neighbours que tenga este nodo menos el ultimo item cargado packet type NET_JOIN
      for (i = 0; i < neighborEntry-1; i = i + 1) {
          
      }
    }
}


long transmitRoutes(lastRoutingTime){
  
}



// imprime la tabla de vecinos
void printNeighborTable(){
    Serial.printf("\n");
    Serial.printf("Neighbor Table: total Neighbors %d\n", neighborEntry);
    for( int i = 0 ; i < neighborEntry ; i++){
        Serial.printf("%02x", neighborTable[i].id);
        Serial.printf(" %3d ", neighborTable[i].hops_away);
        Serial.printf("%02x", neighborTable[i].age);
        // TODO poder visualizar los servicios del vecino y la parte de calidad del vinculo
        Serial.printf("\n");
    }
    Serial.printf("\n");
}

// imprime la tabla de rutas
void printRoutingTable(){
    Serial.printf("\n");
    Serial.printf("Routing Table: total routes %d\n", routeEntry);
    for( int i = 0 ; i < routeEntry ; i++){
        Serial.printf("% destination: ", routeTable[i].receiver);
        Serial.printf(" via ");
        Serial.printf("% via ", routeTable[i].next_neighbor);
        Serial.printf(" age %3d ", routeTable[i].age);
        Serial.printf("\n");
    }
    Serial.printf("\n");
}
