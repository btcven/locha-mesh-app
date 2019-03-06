/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef RADIO_H_
#define RADIO_H_

// HW abstraction layer for radio interfaces
#include "lora.h"

extern BUFFER_packet_t Buffer_packet;

int  startup_radio(){
if (RAD_ENABLED){
    if (RADIOTYPE==1){
      return startup_lora();
    }  
  } 
  return 0;
  
}


int start_radio(){
  
//int start_receive();
  if (RAD_ENABLED){
    if (RADIOTYPE==1){  //1=LORA
      return start_receive_lora_packets();
    }  
  } 
  return 0;
}

int trasmit_package(packet_t mensaje){
  if (RAD_ENABLED){
    if (RADIOTYPE==1){
      return trasmit_package_lora(mensaje){;
    }  
  } 
  }
  return 0;
}

int radio_isused(){
   if (RAD_ENABLED){
    if (RADIOTYPE==LORA){
      return lora_enableInterrupt;
      
    }
}
  return null;
}

int receive_package(){
  if (RAD_ENABLED){
    if (RADIOTYPE==1){
      return receive_package_lora(){;
    }  
  } 
  }
  return 0;
}

void scan_radio(String id_node){
// update lora neighbors
 scan_lora(id_node);
}


#endif //RADIO_H_
