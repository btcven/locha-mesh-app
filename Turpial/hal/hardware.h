/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */
 
#ifndef HARDWARE_H
#define HARDWARE_H

// #define HELTEC_V1
#define HELTEC_V2


// Serial Baudrate
#define BAUDRATE 115200


#if defined(HELTEC_V1)
    #include "heltec_V1.h"
#elif defined(HELTEC_V2)
    #include "heltec_V2.h"
#endif


#endif // HARDWARE_H
