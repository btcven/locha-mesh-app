#ifndef HARDWARE_H
#define HARDWARE_H


//#define HELTEC_V1
#define HELTEC_V2


#if defined(HELTEC_V1)
    #include "heltec_V1.h"
#elif defined(HELTEC_V2)
    #include "heltec_V2.h"
#endif


#endif // HARDWARE_H