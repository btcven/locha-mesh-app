#ifndef TURPIAL_H
#define TURPIAL_H
#include <Arduino.h>
#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#endif // TURPIAL_H