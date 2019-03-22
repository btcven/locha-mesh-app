/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

  
*/

#ifndef MEMORY_DEF_H 
#define MEMORY_DEF_H


#ifdef MCU_ESP32
  static const int MAX_NODES=25;
  static const int MAX_ROUTES=50;
  static const int MAX_MSG_QUEUE=128;
  static const int MAX_NODES_BLACKLIST=25;
#else
  static const int MAX_NODES=5;
  static const int MAX_ROUTES=12;
  static const int MAX_MSG_QUEUE=15;
  static const int MAX_NODES_BLACKLIST=5;
#endif 

#endif // MEMORY_DEF_H
