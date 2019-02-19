/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/

#ifndef DEFAULTSTARTUP_H_
#define DEFAULTSTARTUP_H_

#define SCR_ENABLED true
#define BLE_ENABLED true
#define RAD_ENABLED true
#define WAP_ENABLED true
#define WST_ENABLED true

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

#endif // DEFAULTSTARTUP_H_
