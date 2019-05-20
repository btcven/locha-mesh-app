#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H
#include <Arduino.h>

typedef struct
{
    bool isInit;
    bool isConnected;
} WST_status_t;

typedef struct
{
    bool isInit;
    uint8_t connections;
} WAP_status_t;

typedef struct
{
    bool isInit;
    int lastRSSI;
    float lastSNR;
} RAD_status_t;

typedef struct
{
    bool isInit;
    bool clientConnected;
} BLE_status_t;

typedef struct
{
    bool isInit;
    bool isActive;
} SCR_status_t;

typedef struct
{
    bool isCharging;
    uint8_t percent;
} BAT_status_t;

/*
WST_status_t WiFiStation;
WAP_status_t WiFiAP;
RAD_status_t RADIO;
BLE_status_t BluetoothLE;
SCR_status_t SCREEN;
BAT_status_t BATTERY;
*/

#endif // SYSTEM_STATUS_H