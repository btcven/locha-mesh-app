/**
 * @file ESC_control.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */
#ifndef ESC_CONTROL_H_
#define ESC_CONTROL_H_

struct system_status_t
{
    float bat_lvl;
    float bat_lvl_percent;
};

/**
 * @brief Get the Battery Level object
 * 
 * @return int 
 */
float getBatteryLevel();

/**
 * @brief Get the Battery Level Percent object
 * 
 * @return int 
 */
float getBatteryLevelPercent();

/**
 * @brief 
 * 
 * @return esp_err_t 
 */
esp_err_t escInit();

#endif // ESC_CONTROL_H_
