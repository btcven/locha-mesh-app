/**
 * @file crypto_utils.cpp
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#include <Arduino.h>
#include <Bitcoin.h>
#include <Hash.h>
#include "crypto_utils.h"
#include "general_utils.h"
#include "memory_def.h"

// esta funcion verifica si el hash del mensaje es valido comparando el hash160
bool is_valid_hash160(char *mensaje, char *hash_recibido)
{
  byte hash[SIZE_HASH_MSG] = {0};

  hash160(mensaje, sizeof(mensaje), hash);

  if (compare_char(hash_recibido, (char *)hash))
  {
    return true;
  }
  else
  {
    return false;
  }
}

std::string hash160(char *mensaje)
{
  byte hash[SIZE_HASH_MSG] = {0};
  hash160(mensaje, sizeof(mensaje), hash);
  return (ToString(hash));
}
