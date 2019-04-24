/**
 * @file language.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-04-24
 * 
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
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
