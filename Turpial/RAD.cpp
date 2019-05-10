/**
 * @file RAD.cpp
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
#include <LoRaLib.h>
#include "RAD.h"
#include "hal/hardware.h"
#include "lang/language.h"
#include "memory_def.h"
#include "general_utils.h"
#include "NVS.h"


int Lora_FQERROR;
int Lora_RSSI;
int Lora_SNR;

TaskHandle_t radioHandle = NULL;

#ifdef RAD_ENABLED
  #ifdef RAD_CHIP
     #if RAD_CHIP == SX1276
        SX1276 lora = new LoRa(RAD_CSS, RAD_DIO0, RAD_DIO1);
     #else
        // aqui se pueden definir otros radios
        #define RAD_ENABLED false;
     #endif
  #else
     #define RAD_ENABLED false;
  #endif
#endif

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// disable interrupt when it's not needed
volatile bool enableInterrupt = true;



// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
void setFlag(void) {
  // check if the interrupt is enabled
  if(!enableInterrupt) {
    return;
  }

  // we got a packet, set the flag
  receivedFlag = true;
}

/**
 * @brief send a string throw Radio
 * 
 * @param data_to_send 
 * @return true 
 * @return false 
 */
bool radioSend(std::string data_to_send){
  bool rpta=false;
  const char *TAG = "RADIO Send";
     // start scanning current channel
  int state = lora.scanChannel();
  if(state == CHANNEL_FREE) {
    byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
    state = lora.startTransmit(byteArr, 8);
    if (state != ERR_NONE) {
        ESP_LOGD(TAG, "Transmission failed, code %s",state);
    } else {
      // leave radio allow receive more packets
      state = lora.startReceive();
      rpta=true;
    }
  }
  return rpta;
}

/**
 * @brief Task for async access to Radio interface, managed as callback
 * 
 * @param params 
 */
void task_radio(void *params) {
    char *TAG = "RADIO TASK";
    int state;

while (1) {
     // start scanning current channel
    
  state = lora.scanChannel();
  if(state == PREAMBLE_DETECTED) {
    // start listening for LoRa packets
    ESP_LOGD(TAG, "Starting to listen ...");
    state = lora.startReceive();
    if (state == ERR_NONE) {
      ESP_LOGD(TAG, "success!");
      receivedFlag = true;
    } else {
        ESP_LOGD(TAG, "failed, code:%s",state);  // en esta opcion nunca deberia entrar, se deja solo para desarrollo, si entra aqui es por un error fisico de hw
        while (true);
    }
    ESP_LOGD(TAG, "voy a receivedFlag");
    if (receivedFlag) {
      // disable the interrupt service routine while
    // processing the data
    enableInterrupt = false;

    // reset flag
    receivedFlag = false;
    ESP_LOGD(TAG, "voy a leer");
    // you can read received data as an Arduino String
    String str;
    int state = lora.readData(str);
    

    if (state == ERR_NONE) {
      // packet was successfully received
      
      ESP_LOGD(TAG, "Lora received packet:%s", str);
         
      Lora_RSSI=lora.getRSSI(); // in dBm
        
      Lora_SNR=lora.getSNR();  // in dB
     
      Lora_FQERROR=lora.getFrequencyError();  // in Hz
    

      // process incoming packet
      packet_t packet_received;
      char_to_packet(&packet_received,(uint8_t*)string2char(str.c_str()),str.length());
      
    } else if (state == ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      ESP_LOGD(TAG, "CRC error!:");
     }

    // we're ready to receive more packets,
    // enable interrupt service routine
    enableInterrupt = true;
  
    }
  }
  
    delay(50);
  }
      
}

/**
 * @brief Start Radio interface
 * 
 * @return esp_err_t 
 */
esp_err_t RAD_INIT()
{
    const char *TAG = "RADIO";

    
    bool RAD_enabled = nvs_get_bool(TAG, "enabled", RAD_ENABLED, true);

     if (RAD_enabled)
    {
        // find radio params on NVS
      
        // default values based on https://jgromes.github.io/LoRaLib/class_s_x1276.html
        int RAD_FREQ = nvs_get_int(TAG, "RAD_FREQ", 915, true);
        int RAD_BANDWIDTH = nvs_get_int(TAG, "RAD_BANDWIDTH", 7.8, true);
        int RAD_SPREAD = nvs_get_int(TAG, "RAD_SPREAD", 12, true);
        int RAD_CODING_RAT = nvs_get_int(TAG, "RAD_CODING_RAT", 8, true);
        int RAD_POWER = nvs_get_int(TAG, "RAD_POWER", 20, true);
        int RAD_CUR_LIM_mA = nvs_get_int(TAG, "RAD_CUR_LIM_mA", 100, true);
        int RAD_PREAMBLE = nvs_get_int(TAG, "RAD_PREAMBLE", 8, true);
        int RAD_GAIN = nvs_get_int(TAG, "RAD_GAIN", 0, true);
        ESP_LOGD(TAG, "Starting RADIO...");
 
        int rad_isInit = lora.begin(RAD_FREQ, RAD_BANDWIDTH, RAD_SPREAD, RAD_CODING_RAT, SX127X_SYNC_WORD,RAD_POWER,RAD_CUR_LIM_mA,RAD_PREAMBLE,RAD_GAIN);
       
        if (rad_isInit == ERR_NONE) {
            ESP_LOGD(TAG, "Lora init:%s", MSG_OK);
        } else {
            ESP_LOGD(TAG, "Lora init:%s", MSG_ERROR);
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


  xTaskCreatePinnedToCore(task_radio, "task_radio", 2048, NULL, 5, &radioHandle, ARDUINO_RUNNING_CORE);
}
    
    return ESP_OK;
}
