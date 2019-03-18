/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
/**
   procedimientos para manejo de paquetes salientes
*/

#ifndef OUTCOMING_H
#define OUTCOMING_H

#include <string.h>
#include "route.h"
#include "packet.h"

extern char id_node[16];
extern packet_t Buffer_packet;
extern rutas_t routeTable[255];
extern nodo_t vecinos[255];

int routing_outcoming_PACKET_MSG(char* id_node_destino, char* mensaje) {
  // 1) el destino es un vecino
  // 2) si  no es vecino existe una ruta al vecino (en caso negativo se devuelve packet not delivered por ahora en este demo)
  // 3) se arma el paquete
  // 4) se envia el paquete

  packet_header_t header;
  packet_body_t body;

  if (es_vecino(id_node_destino)) {
    // 1) el destino es un vecino
    // 1.1) se arma el paquete y se envia por una ruta directa

    header.type = MSG;
    header.from = id_node;
    header.to = id_node_destino;
    header.timestamp = millis();
    body.payload = mensaje;
    Buffer_packet.header = header;
    Buffer_packet.body = body;
    packet_to_send(Buffer_packet);  // se envia a la cola de mensajes salientes
  } else {
    if (existe_ruta(id_node, id_node_destino)) {
      // se arma el paquete y se envia por esa ruta
      header.type = MSG;
      header.from = id_node;
      header.to = id_node_destino;
      header.timestamp = millis();
      body.payload = mensaje;
      Buffer_packet.header = header;
      Buffer_packet.body = body;
      packet_to_send(Buffer_packet);  // se envia a la cola de mensajes salientes


    } else {
      // no existe una ruta al destino se devuelve un packet not delivered
      Serial.println(F("PACKET NOT DELIVERED"));
    }
  }

  return 0;
}


#endif // OUTCOMING_H
