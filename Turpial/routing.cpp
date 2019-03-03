
/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */

#include "stdafx.h"
#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <random>
#include "routing.h"
#include <chrono>
#include <fstream>




using namespace std;
extern String id_nodo;
extern packet_t Buffer_packet;
extern route_to_node_t routeTable[255];
extern neighbor_entry_t neighborTable[255];
extern int neighborEntry;
extern int routeEntry;
extern clock_t start;

#define HEADER_LENGTH 16;


// se incluyen los eventos para externos para cada tipo de packet_t
extern int routing_outcoming_PACKET_HELLO(String  id_node, packet_t packet_received);
extern int routing_incoming_PACKET_HELLO(String  id_node, packet_t packet_received);
extern int routing_outcoming_NET_BYE(String  id_node, packet_t packet_received);
extern int routing_incoming_NET_BYE(String  id_node, packet_t packet_received);
extern int routing_outcoming_NET_JOIN(String  id_node, packet_t packet_received);
extern int routing_incoming_NET_JOIN(String  id_node, packet_t packet_received);
extern int routing_outcoming_NET_ROUTE(String  id_node, packet_t packet_received);
extern int routing_incoming_NET_ROUTE(String  id_node, packet_t packet_received);
extern int routing_outcoming_PACKET_ACK(String  id_node, packet_t packet_received);
extern int routing_incoming_PACKET_ACK(String  id_node, packet_t packet_received);
extern int routing_outcoming_PACKET_GOSSIP(String  id_node, packet_t packet_received);
extern int routing_incoming_PACKET_GOSSIP(String  id_node, packet_t packet_received);
extern int routing_outcoming_PACKET_MSG(String  id_node, packet_t packet_received);
extern int routing_incoming_PACKET_MSG(String  id_node, packet_t packet_received);
extern int routing_outcoming_PACKET_TXN(String  id_node, packet_t packet_received);
extern int routing_incoming_PACKET_TXN(String  id_node, packet_t packet_received);


// timeout intervals
int _helloInterval = 10;
int _routeInterval = 10;
int _messageInterval = 5;
int _discoveryTimeout = 30;
int timeDistortion = 1;







// se arma el paquete a transmitir
packet_t buildpacket(uint8_t max_hops, String from, String to, uint8_t sequence[2], String payload, packet_type_e type,uint8_t packetLength ){
   // uint8_t dataLength = (uint8_t)HEADER_LENGTH + (uint8_t)packetLength;
	 uint8_t dataLength = packetLength;
    uint8_t hash=0; // TODO build hash for this package
	 packet_t packet = {
        max_hops,
        dataLength,
        from,
        to,
		sequence[0],  // el primero es el numero del fragmento y el segundo es el total de fragmentos TODO esto habria que cambiarlo para manejar fragmentacion
        sequence[1],
		payload,
        hash,
        type 
    };
  return packet;    
}

packet_type_e convertir(String str)
{
	if(str == "NET_JOIN") return NET_JOIN;
    if(str == "NET_BYE") return NET_BYE;
	if(str == "NET_ROUTE") return NET_ROUTE;
	if(str == "PACKET_ACK") return PACKET_ACK;
	if(str == "PACKET_MSG") return PACKET_MSG;
	if(str == "PACKET_TXN") return PACKET_TXN;
	if(str == "PACKET_HELLO") return PACKET_HELLO;
	if(str == "PACKET_GOSSIP") return PACKET_GOSSIP;
	// cualquier otra cosa es GOSSIP
	return PACKET_GOSSIP;
}


// for string delimiter
std::vector<String> split(String s, String delimiter) {
	 std::vector<String> splitted;

size_t pos = 0;
int pos_arreglo = 1;
String token;
while ((pos = s.find(delimiter)) != String::npos) {
    token = s.substr(0, pos);
	splitted[pos_arreglo]=token;
    //std::cout << token << std::endl;
    s.erase(0, pos + delimiter.length());
}
return splitted;
}



int convert_msg_into_buffer(String msg_received, packet_t Buffer_packet){
  std::vector<String> splitted;
  char* original;
 
  original = (char*)msg_received.c_str();
  //split(original,",",0,splitted);

    splitted = split (original, ",");

  Buffer_packet.max_hops=reinterpret_cast<const uint8_t>(&splitted[0]);    // max_hops no entra en el calculo de hash.
  Buffer_packet.length=reinterpret_cast<const uint8_t>(&splitted[1]);      // longitud del mensaje.
  Buffer_packet.from=splitted[2];        // direccion del remitente
  Buffer_packet.to=splitted[3];          // enviamos el mensaje al id: to
  Buffer_packet.sequence[1]=1; // por ejemplo {1, 2} representa parte 1 de 2 en total TODO pendiente
  Buffer_packet.sequence[2]=1;
  Buffer_packet.payload=splitted[5]; // es la secuencia del mensaje
  Buffer_packet.msg_hash=reinterpret_cast<const uint8_t>(&splitted[6]);    // first 4 bytes.

  String xx;
  xx = splitted[7];
  Buffer_packet.type = convertir(xx);
  return 0;
}




int procesar_buffer(packet_t Buffer_packet, int neighborEntry,neighbor_entry_t neighborTable[255], int type_of_process){
    // funcion que toma el buffer , revisa su contenido , lo procesa y vacia nuevamente el buffer

	// type_of_process indica si es un paquete incoming (0) o outcoming (1)
    if (Buffer_packet.type==NET_BYE){
		//incoming
		if (type_of_process==0){
			routing_incoming_NET_BYE(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_NET_BYE(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==NET_JOIN){
		//incoming
		if (type_of_process==0){
			routing_incoming_NET_JOIN(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_NET_JOIN(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==NET_ROUTE){
		//incoming
		if (type_of_process==0){
			routing_incoming_NET_ROUTE(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_NET_ROUTE(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==PACKET_ACK){
		//incoming
		if (type_of_process==0){
			routing_incoming_PACKET_ACK(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_PACKET_ACK(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==PACKET_HELLO){
		//incoming
		if (type_of_process==0){
			routing_incoming_PACKET_HELLO(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_PACKET_HELLO(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==PACKET_MSG){
		//incoming
		if (type_of_process==0){
			routing_incoming_PACKET_MSG(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_PACKET_MSG(id_nodo, Buffer_packet);
		}
    }
	if (Buffer_packet.type==PACKET_TXN){
		//incoming
		if (type_of_process==0){
			routing_incoming_PACKET_TXN(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_PACKET_TXN(id_nodo, Buffer_packet);
		}
    }
		if (Buffer_packet.type==PACKET_GOSSIP){
		//incoming
		if (type_of_process==0){
			routing_incoming_PACKET_GOSSIP(id_nodo, Buffer_packet);
		}
		//outcoming
		if (type_of_process==1){
			routing_outcoming_PACKET_GOSSIP(id_nodo, Buffer_packet);
		}
    }
	return 0;
}
