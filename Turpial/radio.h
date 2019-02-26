#ifndef RADIO_H_
#define RADIO_H_

// HW abstraction layer for radio interfaces
#include "lora.h"

extern BUFFER_packet_t Buffer_packet;

int  startup_radio(){
if (RAD_ENABLED){
    if (RADIOTYPE==LORA){
      return startup_lora();
    }  
  } 
  return null;
  
}


int start_radio(){
  
int start_receive();
  if (RAD_ENABLED){
    if (RADIOTYPE==LORA){
      return start_receive_lora_packets();
    }  
  } 
  return null;
}

int trasmit_package(packet_t mensaje){
  if (RAD_ENABLED){
    if (RADIOTYPE==LORA){
      return trasmit_package_lora(mensaje){;
    }  
  } 
  return null;
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
    if (RADIOTYPE==LORA){
      return receive_package_lora(){;
    }  
  } 
  return null;
}

void scan_radio(String id_node){
// update lora neighbors
 scan_lora(String id_node);
}


#endif //RADIO_H_
