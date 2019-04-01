#ifndef BLUETOOTH_H
#define BLUETOOTH_H
// Service UUID
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
// Characteristic RX
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
// Characteristic TX
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#include "route.h"

extern message_queue_t mensajes_salientes[MAX_MSG_QUEUE];
extern uint8_t total_mensajes_salientes; 

//extern char *uid ;
//extern char *msg;
//extern double timemsg;
//extern std::string txValue;
//extern std::string rxValue;

void task_bluetooth(void *params);

#endif // BLUETOOTH_H
