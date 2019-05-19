/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef MEMORY_DEF_H
#define MEMORY_DEF_H

// Estas son las definiciones del tamaño de los arreglos de variables en memoria , se pueden aumentar o disminuir de acuerdo a la capacidad de memoria del equipo
// todas las rutas, vecinos y colas se crea su contenido dinamicamente, esto solo provee un limite fisico maximo

static const int MAX_NODES = 6;                      // maximo de nodos vecinos a lo que va a estar escuchando a tiempo de ejecucion
static const int MAX_ROUTES = 32;                    // maximo de rutas que puede usar el equipo a tiempo de ejecucion
static const int MAX_MSG_QUEUE = 50;                 // maximo de mensajes en cola (mensajes no enviados ni siquiera la primera vez)
static const int MAX_NODES_BLACKLIST = 15;           // maximo de nodos bloqueados (blacklisted) a los que no se les va a permitir usar rutas desde/hacia ellos ni tampoco registrarse como vecinos
static const int MSG_QUEUE_WAITING_MAX_RETRIES = 20; // max retries to deliver a packet
static const int MSG_QUEUE_WAITING_MAX_AGE = 30000;  // in millisecond, packet timeout before not delivered (total wait time)
static const int HELLO_RETRY_TIMEOUT = 20000;        // tiempo maximo desde que se recibio el ultimo packet para enviar un HELLO nuevamente y verificar si hay nodos nuevos

static const int SIZE_PAYLOAD = 240; // Tamaño maximo del payload del packet
static const int SIZE_IDNODE = 16;   // Tamaño maximo del id_node
static const int SIZE_HASH_MSG = 20; // Tamaño maximo del hash del mensaje
#endif // MEMORY_DEF_H
