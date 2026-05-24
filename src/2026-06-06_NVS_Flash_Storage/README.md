# NVS Flash Storage

Day 22: counts boots and persists a device config struct across power cycles
using Zephyr's NVS subsystem on internal flash.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

Reset the board multiple times and observe the boot counter incrementing in
the serial log.
