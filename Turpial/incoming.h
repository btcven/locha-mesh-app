/**
   procedimientos para manejo de paquetes entrantes
*/

#ifndef INCOMING_H
#define INCOMING_H

extern char*  id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[3];
extern nodo_t vecinos[3];

int routing_incoming_PACKET_MSG(char* id_node, packet_t packet_received){
  
  // 1) el paquete recibido es para mi nodo : se procesa y se devuelve al origen via la ruta un packet ACK
  // 2) si no es para mi nodo se verifica si el destinatario existe en mi tabla de rutas
  // 3) si no es para mi nodo  y si existe el destinatario en mi tabla de rutas se reenvia a ese destinatario
  // 4) si no es para mi nodo  y no existe el destinatario en mi tabla de rutas se retorna un packet_not_delivered
  // 5) se actualiza el age de la ruta desde el recibido hasta el nodo actual
  
  return 0;
}


#endif // INCOMING_H
