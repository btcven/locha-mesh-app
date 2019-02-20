#ifndef LANGUAGE_H
#define LANGUAGE_H

#define LANGUAGE_CONCAT(M)       #M
#define GENERATE_LANGUAGE_INCLUDE(M)  LANGUAGE_CONCAT(language_##M.h)


#endif //__LANGUAGE_H
