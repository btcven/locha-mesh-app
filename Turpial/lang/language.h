/**
 * @Copyright:
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text
 */

#ifndef LANGUAGE_H
#define LANGUAGE_H

#define _EN_
// #define _ES_
// #define _PT_

#if defined(_EN_)
    #include "en.h"
#elif defined(_ES_)
    #include "es.h"
#elif defined(_PT_)
    #include "pt.h"
#else
    #include "en.h"
#endif


#endif // LANGUAGE_H
