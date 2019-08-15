/**
 * @file language.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-08-15
 * 
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 * 
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
