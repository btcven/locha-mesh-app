/**
 * (c) Copyright 2019 locha.io project developers
 * Licensed under a MIT license, see LICENSE file in the root folder
 * for a full text.
 */
#ifndef LANGUAGE_H
  #define LANGUAGE_H

  #define LANGUAGE_CONCAT(M)       #M
  #define GENERATE_LANGUAGE_INCLUDE(M)  LANGUAGE_CONCAT(language_##M.h)

  // fallback if no language is set, don't change this is helpful for international developers
  #ifndef LANGUAGE_INCLUDE
    #define LANGUAGE_INCLUDE GENERATE_LANGUAGE_INCLUDE(es)
  #endif
#endif //__LANGUAGE_H
