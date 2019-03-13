/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

   Rutinas pra debugging de la app
*/

#ifndef DEBUGGING_H 
#define DEBUGGING_H

#include <Arduino.h>
#include <iostream>
// lib para tablas seriales debugging
#include "fort.hpp"
#include "packet.h"


  void mostrar_packet_via_serial(packet_t el_packet){
    
    
     fort::table table;
    table << fort::header;
    /* Fill each cell with operator[] */
    table [0][0] = "Header - type";
    table [0][1] = "Header - from";
    table [0][2] = "Header - to";
    table [0][3] = "Header - timestamp";
    table [0][4] = "Body - payload";
    table << fort::endr;

    String tipo=String(el_packet.header.type);
    String from=(String)el_packet.header.from;
    String to=(String)el_packet.header.to;
    String timest=(String)el_packet.header.timestamp;
    String payload=(String)el_packet.body.payload;
     /* Fill table using iostream operator << */
    table << tipo << from << to << timest << payload << fort::endr;
    std::cout << table.to_string() << std::endl;
  }

#endif // DEBUGGING_H
