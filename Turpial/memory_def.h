/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

  
*/

#ifndef MEMORY_DEF_H 
#define MEMORY_DEF_H


#ifdef MCU_ESP32
  static const int MAX_NODES=50;
  static const int MAX_ROUTES=255;
  static const int MAX_MSG_QUEUE=255;
#else
  static const int MAX_NODES=10;
  static const int MAX_ROUTES=128;
  static const int MAX_MSG_QUEUE=255;
#endif 

#endif MEMORY_DEF_H
