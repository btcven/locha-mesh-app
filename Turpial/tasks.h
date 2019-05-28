/**
 * @file tasks.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#ifndef TASKS_H
#define TASKS_H
#include <Arduino.h>
#include "memory_def.h"
#include "RAD.h"


extern char id_node[SIZE_IDNODE];

// params for tasks
typedef struct AData{ 
  char id_node[SIZE_IDNODE]; 
 } xData;
 

 void NetworkPeer(void *params);
 void AUTO_HELLO(void *params);
 
#endif // TASKS_H
