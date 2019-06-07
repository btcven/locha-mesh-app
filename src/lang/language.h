/**
 * @file language.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-06-07
 * 
 * @copyright Copyright (c) 2019
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