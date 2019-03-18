/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef GENERAL_FUNCTIONS_H_
#define GENERAL_FUNCTIONS_H_

char* create_unique_id();
char* string2char(String command);

long readVcc();
uint8_t freeRam();
String random_name(int numBytes);
void copy_array(char* src, char* dst, int len);

#endif // GENERAL_FUNCTIONS_H_
