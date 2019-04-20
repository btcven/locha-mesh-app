# Updating new Firmware to device using esptool (python util)

- The command that will be executed is:

  - For Linux

```bash
esptool.py --chip esp32 --port /dev/ttyUSB0  --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 Turpial/boot_app0.bin 0x1000 Turpial/bootloader_qio_80m.bin 0x10000 Turpial/Turpial.Heltec-esp32.esp32.wifi_lora_32_V2.bin 0x8000 Turpial/Turpial.ino.partitions.bin
```

  - For macOS

```bash
esptool.py --chip esp32 --port /dev/cu.SLAB_USBtoUART --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size detect 0xe000 Turpial/boot_app0.bin 0x1000 Turpial/bootloader_qio_80m.bin 0x10000 Turpial/Turpial.Heltec-esp32.esp32.wifi_lora_32_V2.bin 0x8000 Turpial/Turpial.ino.partitions.bin
```