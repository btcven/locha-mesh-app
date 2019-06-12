/**
 * @file ESC_control.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-04
 * 
 * @copyright Copyright (c) 2019
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
