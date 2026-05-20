# Custom PCB Board Definition

Day 10 example: demonstrates how to register a custom board target in Zephyr.

The `boards/arm/my_custom_board/` directory contains a skeleton board definition
based on the nRF52840. Update pin numbers to match your actual PCB schematic.

## Build against the custom board
```bash
west build -b my_custom_board
```

## Build against nRF52840 DK (for testing without custom PCB)
```bash
west build -b nrf52840dk/nrf52840
```
