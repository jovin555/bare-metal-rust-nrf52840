# KXTJ3-1057 Accelerometer — CBOR Encoding over USB

Day 16 example: combines the KXTJ3-1057 custom I2C driver, zcbor encoding,
USB CDC-ACM, and Zephyr Shell. Each accelerometer reading is encoded as a
CBOR map and sent over USB. A Python decoder is included for the PC side.

## Wiring (nRF52840 DK)
| KXTJ3-1057 | nRF52840 DK |
|------------|-------------|
| VDD        | 3.3V        |
| GND        | GND         |
| SDA        | P0.26       |
| SCL        | P0.27       |
| ADDR       | GND (→ 0x0E)|

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Shell commands (over USB serial)
```
accel whoami          → verify WHO_AM_I = 0x35
accel read            → single reading as plain text
accel cbor            → send one CBOR-encoded frame
accel stream 50       → stream 50 CBOR frames at 200 ms intervals
accel range 4         → switch to ±4g range
```

## Python decoder
```bash
# Install dependencies
pip install pyserial cbor2 matplotlib

# Plain table output
python3 tools/decode_accel.py --port /dev/ttyACM0

# Live scrolling plot
python3 tools/decode_accel.py --port /dev/ttyACM0 --plot

# Stop after 100 frames
python3 tools/decode_accel.py --port /dev/ttyACM0 --count 100

# Windows
python3 tools/decode_accel.py --port COM5 --plot
```

## CBOR frame structure
```
{
  "seq":  <uint32>,    sequence number
  "x":    <int32>,     X-axis in mg
  "y":    <int32>,     Y-axis in mg
  "z":    <int32>,     Z-axis in mg
  "rng":  <uint8>      current range in g
}
```
Each frame is terminated with a newline (0x0A) as a delimiter.
