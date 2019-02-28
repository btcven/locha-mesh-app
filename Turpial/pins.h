/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef PINS_H_
#define PINS_H_

/**
  Different boards can have different configurations.
  Comment out your board or use "CUSTOM" definition
  and set up your pinout.
*/

#define HELTEC_V1
//#define CUSTOM

/* HARDWARE HAL */
#ifdef HELTEC_V1
  #include "pins_HELTEC_V1.h"
#elif HELTEC_V2
  #include "pins_HELTEC_V2.h"
#ifdef CUSTOM
  #include "pins_CUSTOM.h"  
#endif // CUSTOM

#endif // PINS_H_
