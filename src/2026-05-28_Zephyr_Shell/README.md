# Zephyr Shell Subsystem

Day 13 example: interactive shell over UART with custom commands.

## Commands available
- `ping`          — replies with "pong"
- `led on`        — turns led0 on
- `led off`       — turns led0 off
- `read temp`     — prints a simulated temperature

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Connect
Open a serial terminal on the UART (115200 baud).
Type `help` to list all available commands.
