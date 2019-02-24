/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef HARDWAREDEFINITIONS_H_
#define HARDWAREDEFINITIONS_H_

/**
  Different boards can have different configurations.
  Comment out your board or use "CUSTOM" definition
  and set up your pinout.
*/

#define HELTEC
//#define CUSTOM

/* HELTEC HAL */
#ifdef HELTEC
#include "pins_HELTEC.h"
#endif // HELTEC_V1/V2

#ifdef CUSTOM
#include "pins_CUSTOM.h"  
#endif // CUSTOM

#endif // HARDWAREDEFINITIONS_H_
