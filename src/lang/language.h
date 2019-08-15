/**
 * @file language.h
 * @author locha.io project developers (dev@locha.io)
 * @brief 
 * @version 0.1
 * @date 2019-07-31
 * @copyright Copyright (c) 2019 locha.io project developers
 * @license MIT license, see LICENSE file for details
 * 
 */

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#if defined(LANG_ES)
    #include "es.h"
#elif defined(LANG_EN)
    #include "en.h"
#endif

#endif // LANGUAGE_H_