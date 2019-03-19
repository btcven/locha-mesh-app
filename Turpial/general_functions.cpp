#include <Arduino.h>
#include <string.h> 
#include "route.h"

String random_name(int numBytes){
char* msg; 
uint8_t i;
String respuesta;
byte randomValue;
memset(msg, 0, numBytes);
      for(i = 0; i < numBytes; i++) {
        randomValue = random(0, 37);
        msg[i] = randomValue + 'a';
        if(randomValue > 26) {
          msg[i] = (randomValue - 26) + '0';
        }
      }
        respuesta=(String)msg;
        respuesta="locha_"+respuesta;
return respuesta;
}


char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    }
}

//char* string2char_node_name(String command){
//     char nombre_temporal[command.length()];
//     memset(nombre_temporal, ' ', sizeof(nombre_temporal)); 
//     command.toCharArray(nombre_temporal, command.length());
//     return  nombre_temporal;         
//}



void copy_array_locha(char* src, char* dst, int len) {
    for (int i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}

void create_unique_id(char* &unique_id_created)
{
  // se genera un unique id con chipid+random+timestamp de la primera configuracion guardada en epprom
  // se adiciona el random porque puede que un mcu no tenga RTC integrado y de esa forma se evitan duplicados
  //TODO
  // se arma el unique id
  uint64_t chipid;
  char uniqueid3[16];
  memset(uniqueid3, ' ', sizeof(uniqueid3));
  #ifdef MCU_ARDUINO
    String aleatorio=random_name(11);  // un string de 4 digitos aleatorios
    aleatorio="locha"+aleatorio;   // se le agrega el prefijo locha
    aleatorio.toCharArray(uniqueid3, 32);
  #endif 
  #ifdef MCU_ESP32
    chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
    char ssid[23]=string2char("");
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(ssid, 23, "LOCH%04X%08X", chip, (uint32_t)chipid);
    String uniqueid2 = String(ssid);
    uniqueid2.toCharArray(uniqueid3, 32);
  #endif 
  
  copy_array_locha(uniqueid3, unique_id_created, 16);
//  return uniqueid3;
}






// verifica la cantidad de memoria disponible libre
 String freeRam () 
    {
      #if defined(__AVR__)
      extern uint8_t __heap_start, *__brkval; 
      uint8_t v; 
      uint8_t rpta= (uint8_t) &v - (__brkval == 0 ? (uint8_t) &__heap_start : (uint8_t) __brkval);
      return (String)rpta;
      #else
      return (String)ESP.getFreeHeap();
     #endif
    }


// lee el voltaje interno de trabajo del arduino
long readVcc() {
  #if defined(__AVR__)
        // Read 1.1V reference against AVcc
        // set the reference to Vcc and the measurement to the internal 1.1V reference
        #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
          ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
          ADMUX = _BV(MUX5) | _BV(MUX0);
        #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
          ADMUX = _BV(MUX3) | _BV(MUX2);
        #else
          ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        #endif  
      
        delay(2); // Wait for Vref to settle
        ADCSRA |= _BV(ADSC); // Start conversion
        while (bit_is_set(ADCSRA,ADSC)); // measuring
      
        uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
        uint8_t high = ADCH; // unlocks both
      
        long result = (high<<8) | low;
      
        result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
        return result; // Vcc in millivolts
  #else
      // return para todo lo demas que no sea Arduino
      
      return 0;
  #endif
  
}
