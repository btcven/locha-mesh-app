#!/bin/bash

### ENV VARS

ARDUINO_CLI_VERSION=0.3.6-alpha.preview
export USER=root

### PROCESS

apt update -y && apt install make curl git python python-serial -y
curl -L https://github.com/arduino/arduino-cli/releases/download/0.3.6-alpha.preview/arduino-cli-$ARDUINO_CLI_VERSION-linux64.tar.bz2 | tar xj -C /tmp/
mv /tmp/arduino-cli-$ARDUINO_CLI_VERSION-linux64 /usr/local/bin/arduino-cli && chmod +x /usr/local/bin/arduino-cli
cd /builds/btcven/locha/app/
arduino-cli core update-index
arduino-cli core install Heltec-esp32:esp32
test -e arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-oled || git clone https://gitlab.com/btcven/locha/heltec-oled  arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-oled
test -e arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-lora || git clone https://gitlab.com/btcven/locha/heltec-lora  arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/heltec-lora
test -e arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/uBitcoin || git clone https://gitlab.com/btcven/locha/uBitcoin arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/uBitcoin
test -e arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/esp32_sqllite || git clone https://gitlab.com/btcven/locha/esp32_SQLite3 arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/esp32_sqllite
test -e https://github.com/jgromes/LoRaLib arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/LoRaLib || git clone https://github.com/jgromes/LoRaLib arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/libraries/LoRaLib
arduino-cli lib install Time
arduino-cli compile --fqbn Heltec-esp32:esp32:wifi_lora_32_V2 Turpial
python arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/tools/gen_esp32part.py -q arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/tools/partitions/default_8MB.csv Turpial/Turpial.ino.partitions.bin
cp arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/tools/sdk/bin/bootloader_qio_80m.bin Turpial/ && cp arduino_data/packages/Heltec-esp32/hardware/esp32/0.0.2-rc1/tools/partitions/boot_app0.bin Turpial/
