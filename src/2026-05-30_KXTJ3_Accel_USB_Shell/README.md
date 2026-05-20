# KXTJ3-1057 Accelerometer with USB Shell

Day 15 example: custom I2C driver for the KXTJ3-1057 3-axis accelerometer,
exposed through a Zephyr Shell over USB CDC-ACM.

## Wiring (nRF52840 DK)
| KXTJ3-1057 | nRF52840 DK |
|------------|-------------|
| VDD        | 3.3V        |
| GND        | GND         |
| SDA        | P0.26       |
| SCL        | P0.27       |
| ADDR       | GND (→ 0x0E)|

If ADDR is tied to 3.3V, change `KXTJ3_ADDR` in `main.c` to `KXTJ3_ADDR_HIGH`.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Connect
Open the USB serial port that appears after flashing.
- Linux:   `minicom -D /dev/ttyACM0`
- macOS:   `screen /dev/tty.usbmodem*`
- Windows: PuTTY on the new COMx port

## Shell commands
```
accel whoami          → Read WHO_AM_I (expect 0x35)
accel read            → Single X Y Z reading in mg
accel stream 20       → Stream 20 readings at 200 ms intervals
accel range 4         → Switch to ±4g measurement range
help                  → List all available commands
```
