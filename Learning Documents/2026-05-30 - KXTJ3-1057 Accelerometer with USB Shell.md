# 2026-05-30 - KXTJ3-1057 Accelerometer with USB Shell

## Goal
Implement a custom I2C driver for the KXTJ3-1057 3-axis accelerometer and expose it through an interactive Zephyr Shell over USB CDC-ACM.

## What you will learn
- How to write a minimal custom I2C driver when no Zephyr driver exists
- How to read raw accelerometer data and convert it to g-force
- How to combine USB CDC-ACM and the Zephyr Shell in one application
- How to build shell commands that read and configure hardware

## KXTJ3-1057 Overview
The KXTJ3-1057 (Kionix) is a low-power 3-axis MEMS accelerometer:
- Interface: I2C (up to 400 kHz)
- I2C address: **0x0E** (ADDR/SDO pin low) or **0x0F** (ADDR/SDO pin high)
- Measurement ranges: ±2g, ±4g, ±8g, ±16g
- Output: 12-bit or 8-bit resolution
- WHO_AM_I register: `0x0F` → value `0x35`

### Key registers
| Register | Address | Description |
|----------|---------|-------------|
| XOUT_L   | 0x06    | X-axis low byte |
| XOUT_H   | 0x07    | X-axis high byte |
| YOUT_L   | 0x08    | Y-axis low byte |
| YOUT_H   | 0x09    | Y-axis high byte |
| ZOUT_L   | 0x0A    | Z-axis low byte |
| ZOUT_H   | 0x0B    | Z-axis high byte |
| WHO_AM_I | 0x0F    | Device ID (0x35) |
| CTRL_REG1| 0x1B    | Power/range/resolution control |

### CTRL_REG1 bit map
```
Bit 7: PC1   — 1 = operating mode, 0 = standby
Bit 6: RES   — 1 = 12-bit, 0 = 8-bit
Bit 4: GSEL1 — range select MSB
Bit 3: GSEL0 — range select LSB
  00 = ±2g, 01 = ±4g, 10 = ±8g (14-bit), 11 = ±8g
```

## Hardware wiring (nRF52840 DK)
| KXTJ3-1057 pin | nRF52840 DK pin |
|----------------|-----------------|
| VDD            | 3.3V            |
| GND            | GND             |
| SDA            | P0.26           |
| SCL            | P0.27           |
| ADDR           | GND (→ 0x0E)    |

## Project structure
```
2026-05-30_KXTJ3_Accel_USB_Shell/
├── CMakeLists.txt
├── prj.conf
├── app.overlay
├── drivers/
│   └── kxtj3/
│       ├── kxtj3.h        ← register map and API
│       └── kxtj3.c        ← I2C read/write implementation
└── src/
    └── main.c             ← USB init + shell commands
```

## prj.conf
```kconfig
CONFIG_I2C=y
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_CDC_ACM=y
CONFIG_UART_LINE_CTRL=y
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
```

## Shell commands implemented
| Command | Description |
|---------|-------------|
| `accel whoami` | Read WHO_AM_I register (expect 0x35) |
| `accel read` | Print X, Y, Z in mg |
| `accel stream <n>` | Print n readings at 200 ms intervals |
| `accel range <2\|4\|8\|16>` | Set measurement range |

## Converting raw counts to mg
The 12-bit result is left-justified in a 16-bit register pair.
Shift right by 4 to get the signed 12-bit value, then scale:
```c
int16_t raw = (int16_t)((high << 8) | low) >> 4;
int32_t mg  = (raw * 1000) / counts_per_g;
// counts_per_g = 1024 for ±2g, 512 for ±4g, 256 for ±8g
```

## Why this matters
Many sensors used in production products are not supported by Zephyr's upstream drivers. Writing a thin I2C wrapper is a core embedded skill. The shell interface lets you validate sensor output during bring-up without rebuilding firmware.

## Practice tasks
1. Verify WHO_AM_I returns `0x35` before reading data.
2. Hold the board still and confirm Z reads close to 1000 mg (1g).
3. Try `accel range 4` and confirm the readings change scale.
4. Extend `accel stream` to also encode data as CBOR (combine with Day 12).

## Example folder
See `src/2026-05-30_KXTJ3_Accel_USB_Shell` for the full driver and application code.

## Next topic
Future topics: BLE peripheral advertising, MCUboot firmware updates, low-power sleep modes.
