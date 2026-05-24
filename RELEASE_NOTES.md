# Release Notes — Embedded Firmware with Zephyr RTOS

## Version 1.0 — May 2026

**Initial release.**

This is the first edition of *Embedded Firmware with Zephyr RTOS: A Practical Guide from LED to Custom PCB*.

### What's included

- 24 structured lessons covering Zephyr RTOS on the nRF52840
- Complete, buildable source code for every day
- Custom I2C driver for the KXTJ3-1057 accelerometer
- USB CDC-ACM communication with CBOR encoding and Python decoder
- BLE peripheral advertising, SPI flash, ADC, PWM, and NVS storage
- Work queues, kernel timers, and power management (sub-µA System OFF)
- Full custom PCB board definition skeleton
- Zephyr 4.x workspace setup guide (SDK 1.0.1, Python 3.12)

### Hardware covered

- Nordic nRF52840 DK
- KXTJ3-1057 3-axis accelerometer
- W25Q SPI flash
- Custom nRF52840-based PCB (board definition template)

### Known limitations

- Examples target Linux (Ubuntu 22.04). Windows/macOS setup not covered.
- No MCUboot OTA update example in this edition.
- BLE GATT services and LwM2M are planned for a future edition.

---

## Planned for Version 1.1

- MCUboot OTA firmware update
- BLE GATT service with sensor data notifications
- LwM2M cloud integration example
