#ifndef SYSTEM_STATUS_H
#define SYSTEM_STATUS_H
#include <Arduino.h>


typedef struct {
    bool isInit;
    int lastRSSI;
    float lastSNR;
} RAD_status_t;

typedef struct {
    bool isInit;
    bool clientConnected;
} BLE_status_t;

typedef struct {
    bool isInit;
    bool isActive;
} SCR_status_t;








#endif // SYSTEM_STATUS_H