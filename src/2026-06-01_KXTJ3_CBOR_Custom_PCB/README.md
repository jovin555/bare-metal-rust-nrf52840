# KXTJ3-1057 CBOR Accelerometer on Custom PCB

Day 17: Same CBOR-over-USB accelerometer application as Day 16,
now targeting a custom nRF52840 board definition instead of the DK.

## Key difference from Day 16
The KXTJ3-1057 I2C node is declared inside the **board DTS** (not app.overlay)
because it is soldered to the PCB. The app.overlay only adds the USB CDC-ACM
shell routing — making the application code board-agnostic.

## Before building — update pin assignments
Edit `boards/arm/my_custom_board/my_custom_board.dts` and change the
pinctrl pin numbers to match your actual PCB schematic:

| Signal   | Default pin | Where to change |
|----------|-------------|-----------------|
| I2C SDA  | P0.04       | `i2c0_default` group |
| I2C SCL  | P0.05       | `i2c0_default` group |
| UART TX  | P0.06       | `uart0_default` group |
| UART RX  | P0.08       | `uart0_default` group |
| LED      | P1.10       | `leds` node |

If KXTJ3 ADDR pin is tied to VDD, change `reg = <0x0e>` to `<0x0f>` in
the board DTS and update `KXTJ3_ADDR` in `src/main.c`.

## Build and flash
```bash
west build -b my_custom_board
west flash
```

## Verify resolved pin assignments
```bash
cat build/zephyr/zephyr.dts | grep -A5 "i2c0"
```

## Shell commands (over USB)
```
accel whoami        → verify WHO_AM_I = 0x35
accel read          → plain text reading
accel cbor          → send one CBOR frame
accel stream 50     → stream 50 CBOR frames at 200 ms
accel range 4       → set ±4g range
```

## Python decoder
```bash
pip install pyserial cbor2 matplotlib
python3 tools/decode_accel.py --port /dev/ttyACM0 --plot
```
