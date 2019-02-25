/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef DEFAULTSTARTUP_H_
#define DEFAULTSTARTUP_H_



typedef struct  {
  bool isEnabled;
  bool isActive;
} SCR_status_t;

typedef struct {
  bool isEnabled;
  bool isActive;
  bool isConnected;
} BLE_status_t;

typedef struct {
  bool isEnabled;
  bool isActive;
  bool isConnected;
} WST_status_t;

typedef struct {
  bool isEnabled;
  bool isActive;
  // this may need a better approach:
  // client_t[max_clients]
} WAP_status_t;

typedef struct {
  bool isEnabled;
  bool isActive;
} RAD_status_t;

typedef struct {
  bool isEnabled;
  bool isActive;
} BAT_status_t;

#endif // DEFAULTSTARTUP_H_
