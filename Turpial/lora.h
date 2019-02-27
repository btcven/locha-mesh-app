#ifndef LORA_H
#define LORA_H

#include <LoRaLib.h>  // libreria para Lora https://github.com/jgromes/LoRaLib

#include "routing.h"

extern BUFFER_packet_t Buffer_packet;

// flag to indicate that a lora packet was received
volatile bool lora_receivedFlag = false;

// disable interrupt for lora when it's not needed
volatile bool lora_enableInterrupt = true;

// create instance of LoRa class using SX1278 module
// this pinout corresponds to RadioShield
// https://github.com/jgromes/RadioShield
// NSS pin:   10 (4 on ESP32/ESP8266 boards)
// DIO0 pin:  2
// DIO1 pin:  3
if (LORACHIP==SX1278){
  SX1278 loraSX = new LoRa;
}
// create another instance of LoRa class using 
// SX1272 module and user-specified pinout
// NSS pin:   6
// DIO0 pin:  4
// DIO1 pin:  5
if (LORACHIP==SX1272){
  SX1272 loraSX = new LoRa(6, 4, 5);
}
// create third instance of LoRa class using 
// SX1276 module and user-specified pinout
// we ran out of Uno digital pins, so here we use 
// analog ones
// NSS pin:   14 (A0)
// DIO0 pin:  15 (A1)
// DIO1 pin:  16 (A2)
if (LORACHIP==SX1276){
  SX1276 loraSX = new LoRa;
}

// create third instance of LoRa class using 
// SX1276 module and user-specified pinout
// we ran out of Uno digital pins, so here we use 
// analog ones
// NSS pin:   14 (A0)
// DIO0 pin:  15 (A1)
// DIO1 pin:  16 (A2)
//if (LORACHIP==SX1276){
//  SX1276 loraSX1276 = new LoRa(14, 15, 16);
//}

int  startup_lora(){
// initialize LORA/SX/RFM with default settings
// example for SX1278:
// carrier frequency:           434.0 MHz
  // bandwidth:                   125.0 kHz
  // spreading factor:            9
  // coding rate:                 7
  // sync word:                   0x12
  // output power:                17 dBm
  // current limit:               100 mA
  // preamble length:             8 symbols
// amplifier gain:              0 (automatic gain control)

  int state = lora.begin();

  // set the function that will be called 
  // when new packet is received
  if (state == ERR_NONE) {
    lora.setDio0Action(setFlag);
   start_receive_lora_packets();
    
  }
  return state;
}

int start_receive_lora_packets(){
   // start listening for LoRa packets
    state = lora.startReceive();
    
   // NOTE: 'listen' mode will be disabled 
  // automatically by calling any of the 
  // following methods:
  //
  // lora.standby()
  // lora.sleep()
  // lora.transmit()
  // lora.receive()
  // lora.scanChannel()
  //
  // LoRa module will not receive any new
  // packets until 'listen' mode is re-enabled
  // by calling lora.startReceive()
  return state;
}

// this function is called when a complete packet
// is received or transmited  by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!lora_enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  lora_receivedFlag = true;
}

int trasmit_package_lora(packet_t mensaje){
  // you can transmit C-string or Arduino string up to
  // 256 characters long
  lora_enableInterrupt = true;
  state = lora.transmit(mensaje);
  lora_enableInterrupt = false;
  if (state != ERR_NONE) {
      Serial.print(F("failed, code "));
      Serial.println(state);
}
int state_new=start_receive_lora_packets();
  return state;
}

int receive_package_lora(){
  if (lora_receivedFlag){
  // you can transmit C-string or Arduino string up to
  // 256 characters long
  String str;
   lora_enableInterrupt = false;
  int state = lora.receive(str);
  
  if (state == ERR_NONE) {
      // packet was successfully received
      Serial.println("Received packet!");
  
      // print data of the packet
      Serial.print("Data:\t\t\t");
      Serial.println(str);
  
      // print RSSI (Received Signal Strength Indicator) 
      Serial.print("RSSI:\t\t\t");
      Serial.print(lora.getRSSI());
      Serial.println(" dBm");
  
      // print SNR (Signal-to-Noise Ratio) 
      Serial.print("SNR:\t\t\t");
      Serial.print(lora.getSNR());
      Serial.println(" dB");

      // print frequency error
      Serial.print("Frequency error:\t");
      Serial.print(lora.getFrequencyError());
      Serial.println(" Hz");

       // se coloca el packet recibido en el buffer
      convert_msg_into_buffer(str);
      procesar_buffer();
      
    } else if (state == ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println("CRC error!");
      
  }
  int state_new=start_receive_lora_packets();
  lora_enableInterrupt = true;
  lora_receivedFlag=false;
  }
  return str;
}

void scan_lora(String id_node){
// update lora neighbors
  // TODO
  // si no esta en uso se escanea buscando actividad en el canal
  if (!lora_receivedFlag){
     Serial.print("Scanning channel for LoRa preamble ... ");

  // start scanning current channel
  int state = lora.scanChannel();
  
  if(state == PREAMBLE_DETECTED) {
    // LoRa preamble was detected
    Serial.println(" detected preamble!");
    start_receive_lora_packets();
    // se envia un packet HELLO
    // en to y maxhops se coloca 0 porque es para cualquiera que lo reciba
    build_packet(0, id_node, 0, {1, 1},"HELLO", 6,sizeof(Buffer_packet))
    
  } else if(state == CHANNEL_FREE) {
    // no preamble was detected, channel is free
    Serial.println(" channel is free!");
    
  }

  }
}


#endif //LORA_H
