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

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

/**
 * @brief max retries to deliver a packet
 * 
 */
static const int MSG_QUEUE_WAITING_MAX_RETRIES = 20;

/**
 * @brief in millisecond, packet timeout before not delivered (total wait time)
 * 
 */
static const int MSG_QUEUE_WAITING_MAX_AGE = 30000;

/**
 * @brief  tiempo maximo desde que se recibio el ultimo packet para enviar un HELLO nuevamente y verificar si hay nodos nuevos
 * 
 */
static const int HELLO_RETRY_TIMEOUT = 30000;

/**
 * @brief max payload length per packet
 * 
 */
static const int SIZE_PAYLOAD = 200;

/**
 * @brief max id_node (NID) length 
 * 
 */
static const int SIZE_IDNODE = 12;

/**
 * @brief checksum length
 * 
 */
static const int SIZE_HASH_MSG = 20;

#endif // MEMORY_DEF_H
