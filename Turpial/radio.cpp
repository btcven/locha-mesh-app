/**
   (c) Copyright 2019 locha.io project developers
   Licensed under a MIT license, see LICENSE file in the root folder
   for a full text.
*/
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>
#include "hardwareDefinitions.h"

// Pin mapping
const lmic_pinmap lmic_pins = {
  .nss = RAD_CSS,
  .rxtx = LMIC_UNUSED_PIN,
  .rst = RAD_RST,
  .dio = {RAD_DIO0, RAD_DIO1, RAD_DIO2},
};
