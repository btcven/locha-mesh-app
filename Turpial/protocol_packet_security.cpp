/**
 * @file protocol_packet_security.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-26
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * 
 */

#include <Arduino.h>
#include "hal/hardware.h"
#include "lang/language.h"
#include "memory_def.h"
#include "general_utils.h"
#include "packet.h"
#include "SQLite.h"
// #include "tables.h"
// #include "protocol_packet_security.h"

// manejo del packets  START
void protocol_incoming_PACKET_SECURITY_START(char id_node[SIZE_IDNODE], packet_t packet_temporal)
{
    // se verifica si el remitente esta en blacklist
}

// manejo del packets  TEST
void protocol_incoming_PACKET_SECURITY_TEST(char id_node[SIZE_IDNODE], packet_t packet_temporal)
{
}

// manejo del packets  RESPONSE
void protocol_incoming_PACKET_SECURITY_RESPONSE(char id_node[SIZE_IDNODE], packet_t packet_temporal)
{
}

void protocol_incoming_PACKET_SECURITY(char id_node[SIZE_IDNODE], packet_t packet_temporal)
{
    // se clasifica se acuerdo al subtipo de packet recibido
    switch (packet_temporal.header.packet_sub.security_type)
    {
    case EMPTY_SECURITY:
        break;
    case START:
        protocol_incoming_PACKET_SECURITY_START(id_node, packet_temporal);
        break;
    case TEST:
        protocol_incoming_PACKET_SECURITY_TEST(id_node, packet_temporal);
        break;
    case RESPONSE:
        protocol_incoming_PACKET_SECURITY_RESPONSE(id_node, packet_temporal);
        break;
    }
}
