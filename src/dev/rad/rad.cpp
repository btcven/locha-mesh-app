/**
 * @file rad.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <Arduino.h>

#include "rad.h"
#include <Preferences.h>
#include "hal/hardware.h"
#include "lang/language.h"
#include "dev/nvs/nvs.h"
#include "dev/core/general_utils.h"


#if RAD_ENABLED
#ifdef RAD_TYPE
#if (RAD_TYPE == RFM95)
#include <LoRaLib.h>
#include "rad_LoRaLib.h"
#endif
#if (RAD_TYPE == E32)
#include <RH_Serial.h>
#endif
#endif
#endif

/*
Prototypes
 */

bool radioSendLora(std::string data_to_send);
bool radioInitLora();
byte *onRadioReceiveLora(int packetSize);

/* end prototypes */

bool radioreceiving;
bool radioCAD;
std::string radiorxvalue;

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t radInit()
{
    const char *TAG = "RAD";
    // only in dev stage, clear nvs before start:
    esp_err_t returned2 = nvsClear(TAG);

    bool rad_enabled = nvsGetBool(TAG, "enabled", RAD_ENABLED, true);

    if (rad_enabled)
    {
#if RAD_ENABLED
        ESP_LOGD(TAG, "%s %S", MSG_STARTING, MSG_RADIO);
#ifdef RAD_TYPE
#if (RAD_TYPE == RFM95)
        bool returned = radioInitLora();
#endif
#endif
        if (returned)
        {
            return ESP_OK;
        }
        else
        {
            ESP_LOGD(TAG, "Radio not started: %s %S", MSG_FAIL, MSG_RADIO);
            return ESP_FAIL;
        }
#endif
    }
    else
    {
        return ESP_FAIL;
    }
}

/**
 * @brief Processing incoming packets
 * 
 */
void onReceive(int packetSize)
{
#if RAD_ENABLED
#ifdef RAD_TYPE
#if (RAD_TYPE == RFM95)
    radiorxvalue = ToString(onRadioReceiveLora(packetSize));
#endif
#endif
#endif
}

/**
 * @brief Send data throw Radio
 * 
 */
bool radioSend(std::string data_to_send)
{
    bool returned = radioSendLora(data_to_send);
    return returned;
}
