/**
 * @file protocol.cpp
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
#include "protocol_packet_data.h"
#include "protocol_packet_routing.h"
#include "protocol_packet_security.h"

/**
 * @brief Process a received packet , is processed based on packet.header.type , each type had their own void to process
 * 
 * @param id_node 
 * @param packet_temporal 
 * @param RSSI_recibido 
 * @param SNR_recibido 
 * @param db 
 */
void process_received_packet(char id_node[SIZE_IDNODE], packet_t packet_temporal, int RSSI_recibido, int SNR_recibido, sqlite3 *db)
{
  uint8_t rpta;
  char *pChar = (char *)"";
  const char *TAG = "process_received_packet";

  // se verifica que el origen y destino no sea el mismo, para evitar ataques
  if (!(compare_char(packet_temporal.header.from, packet_temporal.header.to)))
  {

    // solo se procesa un packet si es para mi (to,next_neighbour) o si es un broadcats (to="")

    // si el packet es para mi nodo o es un broadcast
    if ((compare_char(packet_temporal.header.to, id_node)) or ((compare_char(packet_temporal.header.to, pChar))) or ((compare_char(packet_temporal.header.next_neighbor, id_node))))
    {

      ESP_LOGE(TAG, "A new packet incoming direct for my node, type:%s", convertir_packet_type_e_str(packet_temporal.header.packet_type));

      switch (packet_temporal.header.packet_type)
      {
      case EMPTY:
        ESP_LOGE(TAG, "Node:%s is sending EMPTY packets, review it or block it (add to blacklist)", packet_temporal.header.from);
        break;
      case DATA:
        protocol_incoming_PACKET_DATA(id_node, packet_temporal);
        break;
      case ROUTING:
        protocol_incoming_PACKET_ROUTING(id_node, packet_temporal, RSSI_recibido, SNR_recibido, db);
        break;
      case SECURITY:
        protocol_incoming_PACKET_SECURITY(id_node, packet_temporal);
        break;
      default:
        break;
      }
    }
  }
}
