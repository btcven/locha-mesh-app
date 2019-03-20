/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#ifndef GENERAL_FUNCTIONS_H_
#define GENERAL_FUNCTIONS_H_

void create_unique_id(char* &unique_id_created);
char* string2char_node_name(String command);
void string2char_node_name_v2(String command, char* &respuesta);
char* string2char(String command);
boolean isNumeric(String str);

long readVcc();
String freeRam();
String random_name(int numBytes);
void copy_array_locha(char* src, char* dst, int len);



#endif // GENERAL_FUNCTIONS_H_
