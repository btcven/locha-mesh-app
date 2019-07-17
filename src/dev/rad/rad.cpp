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
<<<<<<< HEAD

#endif
=======
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

    bool
    radioSendLora(std::string data_to_send);
bool radioInitLora();
byte *onRadioReceiveLora(int packetSize);

/* end prototypes */
>>>>>>> 7cf2e1d602086b817fab4e88ff94c4da2e8e351f

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
    // nvsClear(TAG);

    bool rad_enabled = nvsGetBool(TAG, "enabled", RAD_ENABLED, true);

    if (rad_enabled)
    {

        ESP_LOGD(TAG, "Radio not started: %s %S", MSG_FAIL, MSG_RADIO);
        return ESP_FAIL;
    }
    else
    {
        return ESP_FAIL;
    }
}
