/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */

#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include "routing.h"

using namespace std;

extern String id_node;
extern packet_t Buffer_packet;
extern route_to_node_t routeTable[255];
extern neighbor_entry_t neighborTable[255];
extern int neighborEntry;
extern int routeEntry;

int routing_outcoming_PACKET_HELLO(String  id_node, packet_t packet_received){

	return 0;
}

int routing_incoming_PACKET_HELLO(String  id_node, packet_t packet_received){
	 // se le devuelve el info del nodo 
    uint8_t age_now=millis();

	quality_link_t calidad = {
		1,1,1
	};


      // se coloca como un neighbor
           neighbor_entry_t el_vecino = {
				Buffer_packet.from,// uniqueid
				1,  // servicios que ofrece
				1,  // servicios que ofrece
				1,  // servicios que ofrece
				1,  // servicios que ofrece
				1,  // servicios que ofrece
				age_now,  // edad conocida
				1,   // saltos de distancia (metrica)
				calidad
};
        
      neighborEntry++;
      neighborTable[neighborEntry]=el_vecino;
      // vacio el buffer
      packet_t buffer_vacio;
      Buffer_packet=buffer_vacio;
      // se devuelve la tabla de neighbours que tenga este nodo menos el ultimo item cargado packet type NET_JOIN
	  uint8_t i;
      for (i = 0; i < neighborEntry-1; i = i + 1) {
          
      }
	  return 0;
}
