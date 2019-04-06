/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.

  
*/

#ifndef MEMORY_DEF_H 
#define MEMORY_DEF_H

  static const int MAX_NODES=12;
  static const int MAX_ROUTES=35;
  static const int MAX_MSG_QUEUE=90;
  static const int MAX_NODES_BLACKLIST=25;
  //static const int MAX_MSG_QUEUE_WAITING=64;   // se elimino para poder reutlizar las funciones de debugging entre msg_queue y mas_waiting ya que comparten el mismo typedef
  static const int MSG_QUEUE_WAITING_MAX_RETRIES=5; // max retries to deliver a packet
  static const int MSG_QUEUE_WAITING_MAX_AGE=30000;  // in millisecond, packet timeout before not delivered (total wait time

#endif // MEMORY_DEF_H
