/**
 * @file rad_LoRaLib.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Arduino.h>
#include <LoRaLib.h>
#include <Preferences.h>
#include "hal/hardware.h"
#include "lang/language.h"
#include "dev/nvs/nvs.h"
#include "dev/core/general_utils.h"

SX1276 lora = new LoRa(RAD_CSS, RAD_DIO0, RAD_DIO1);

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void)
{
  // check if the interrupt is enabled
  if (!enableInterrupt)
  {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
  digitalWrite(RAD_PIN_RECEIVE_DATA, HIGH);
}

/**
 * @brief send a string throw Radio
 * 
 * @param data_to_send 
 * @return true 
 * @return false 
 */
bool radioSendLora(std::string data_to_send)
{
  bool rpta = false;
  int state;
  uint8_t data_uint8;
  const char *TAG = "RADIO Send LoRaLib";
  ESP_LOGD(TAG, "Inside RadioSend LoRaLib");
  // start scanning current channel
  //  state = lora.scanChannel();
  //  ESP_LOGD(TAG, "scan channel ready");
  // if (state == CHANNEL_FREE)
  // {
  //  ESP_LOGD(TAG, "Channel is free");
  byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
  const char *text_to_send(string2char(data_to_send));
  //state = lora.startTransmit(byteArr, 8);
  state = lora.startTransmit(text_to_send);
  if (state != ERR_NONE)
  {
    ESP_LOGD(TAG, "Transmission failed, code %s", state);
  }
  else
  {
    ESP_LOGD(TAG, "Send OK");
    rpta = true;
  }
  // } else {
  //   ESP_LOGD(TAG, "Channel is busy");
  // }
  // leave radio allow receive more packets
  state = lora.startReceive();
  ESP_LOGD(TAG, "Exit radio Send");
  return rpta;
}

bool radioInitLora()
{
  // find radio params on NVS
  const char *TAG = "RADIO";
  // default values based on https://jgromes.github.io/LoRaLib/class_s_x1276.html
  int RAD_FREQ = nvsGetInt(TAG, "RAD_FREQ", 915, true);
  int RAD_BANDWIDTH = nvsGetInt(TAG, "RAD_BANDWIDTH", 7.8, true);
  int RAD_SPREAD = nvsGetInt(TAG, "RAD_SPREAD", 12, true);
  int RAD_CODING_RAT = nvsGetInt(TAG, "RAD_CODING_RAT", 8, true);
  int RAD_POWER = nvsGetInt(TAG, "RAD_POWER", 20, true);
  int RAD_CUR_LIM_mA = nvsGetInt(TAG, "RAD_CUR_LIM_mA", 100, true);
  int RAD_PREAMBLE = nvsGetInt(TAG, "RAD_PREAMBLE", 8, true);
  int RAD_GAIN = nvsGetInt(TAG, "RAD_GAIN", 0, true);
  ESP_LOGD(TAG, "Starting RADIO...");

  int rad_isInit = lora.begin(RAD_FREQ, RAD_BANDWIDTH, RAD_SPREAD, RAD_CODING_RAT, SX127X_SYNC_WORD, RAD_POWER, RAD_CUR_LIM_mA, RAD_PREAMBLE, RAD_GAIN);

  if (rad_isInit == ERR_NONE)
  {
    ESP_LOGD(TAG, "Lora init:%s", MSG_OK);
  }
  else
  {
    ESP_LOGD(TAG, "Lora init:%s", MSG_FAIL);
    // kill task by task handler
    // se coloca reiniciar el ESP pero deberia ser reiniciar el task_radio pero no se consigue un comando para reiniciar
    // ver https://github.com/espressif/arduino-esp32/issues/1996
    ESP.restart();
    //xTaskAbortDelay(radioHandle);
  }
  // set the function that will be called
  // when new packet is received
  ESP_LOGD(TAG, "Setting DIO0");
  lora.setDio0Action(setFlag);

  rad_isInit = lora.startReceive();
  if (rad_isInit == ERR_NONE)
  {
    ESP_LOGD(TAG, "success!");
  }
  else
  {
    ESP_LOGD(TAG, "failed, code %s", rad_isInit);
    while (true)
      ;
  }
}

/**
 * @brief Processing incoming Radio packets
 * 
 */
byte *onRadioReceiveLora(int packetSize)
{
  const char *TAG = "RAD";
  byte byteArr[packetSize];
  //std::string rxValue_Lora;

  ESP_LOGD(TAG, "Receiving packet ...");
  if (packetSize == 0)
    return NULL;

  //char in_process;
  //uint8_t i;
  if (!receivedFlag)
  {
    if (packetSize)
    {
      
      int state = lora.readData(byteArr, packetSize);
      // estaba packesize-1 y se cambio packetsize
      //for (i = 0; i < packetSize; i++)
      //{
      //  in_process = (char)lora.read();
        // se coloca en el Buffer Lora
      //  rxValue_Lora = rxValue_Lora + in_process;
      //}
      receivedFlag = true;
    }
  }
  
  byte* c=byteArr;  
  
  digitalWrite(RAD_PIN_RECEIVE_DATA, LOW);
  return c;
  
}