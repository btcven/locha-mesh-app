/**
   procedimientos para manejo de paquetes salientes
*/

#ifndef OUTCOMING_H
#define OUTCOMING_H

extern char* id_node;
extern packet_t Buffer_packet;
extern rutas_t routeTable[3];
extern nodo_t vecinos[3];

int routing_outcoming_PACKET_MSG(char* id_node_destino, char* mensaje){
// 1) el destino es un vecino
//2) si  no es vecino existe una ruta al vecino (en caso negativo se devuelve packet not delivered por ahora en este demo)
//3) se arma el paquete
// 4) se envia el paquete

  if (es_vecino(id_node_destino)){
// 1) el destino es un vecino
    //1.1) se arma el paquete y se envia por una ruta directa
      packet_header_t header;
      packet_body_t body;
      header.type=MSG;
      header.from=id_node;
      header.to=id_node_destino;
      header.timestamp=millis();
      body.payload=mensaje;
      Buffer_packet.header=header;
      Buffer_packet.body=body;
      trasmit_package(Buffer_packet);
  } else {
    if (existe_ruta(id_node, id_node_destino)){
        // se arma el paquete y se envia por esa ruta 
        
      
    } else {
        // no existe una ruta al destino se devuelve un packet not delivered
         Serial.println(F("PACKET NOT DELIVERED"));
    }
  }

  return 0;
}


#endif // OUTCOMING_H
