/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

  
*/

#ifndef MEMORY_DEF_H 
#define MEMORY_DEF_H


//#ifdef MCU_ESP32
  static const int MAX_NODES=25;
  static const int MAX_ROUTES=50;
  static const int MAX_MSG_QUEUE=128;
  static const int MAX_NODES_BLACKLIST=25;
  static const int MAX_MSG_QUEUE_WAITING=64;
  static const int MSG_QUEUE_WAITING_MAX_RETRIES=5; // max retries to deliver a packet
  static const int MSG_QUEUE_WAITING_MAX_AGE=30000;  // in millisecond, packet timeout before not delivered (total wait time
  
//#else
//  static const int MAX_NODES=5;
//  static const int MAX_ROUTES=12;
//  static const int MAX_MSG_QUEUE=15;
//  static const int MAX_NODES_BLACKLIST=5;
//#endif 

#endif // MEMORY_DEF_H
