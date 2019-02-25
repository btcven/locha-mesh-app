#ifndef WATCHDOG_H
#define WATCHDOG_H

void setWD() {
  wdt = millis();
}

void watchDog(void *pvParameters)
{
  // hace reset si no responde en menos de WDtimeout
 #define WDtimeout 15000
  unsigned long wdc = millis();
  while (1) {
    if ((millis() - wdc) > 10000) {
      if ((millis() - wdt) > WDtimeout)  ESP.restart();
      wdc = millis();
    }
  }
}

#endif //WATCHDOG_H
