#ifndef LORA_H
#define LORA_H

#include <LoRaLib.h>  // libreria para Lora https://github.com/jgromes/LoRaLib

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
// is received by the module
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


#endif //LORA_H
