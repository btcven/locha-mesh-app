/**
 * @file memory_def.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
  */
#ifndef MEMORY_DEF_H
#define MEMORY_DEF_H

static const int MSG_QUEUE_WAITING_MAX_RETRIES = 20; // max retries to deliver a packet
static const int MSG_QUEUE_WAITING_MAX_AGE = 30000;  // in millisecond, packet timeout before not delivered (total wait time)
static const int HELLO_RETRY_TIMEOUT = 20000;        // tiempo maximo desde que se recibio el ultimo packet para enviar un HELLO nuevamente y verificar si hay nodos nuevos

static const int SIZE_PAYLOAD = 240; // Tamaño maximo del payload del packet
static const int SIZE_IDNODE = 16;   // Tamaño maximo del id_node
static const int SIZE_HASH_MSG = 20; // Tamaño maximo del hash del mensaje

#endif // MEMORY_DEF_H
