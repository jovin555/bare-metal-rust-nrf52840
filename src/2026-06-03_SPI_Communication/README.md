# SPI Communication

Day 19: Read JEDEC ID from a W25Q SPI NOR flash, then write and read back data.

## Wiring (nRF52840 DK)
| W25Q pin | nRF52840 DK |
|----------|-------------|
| MOSI (DI)| P0.20       |
| MISO (DO)| P0.21       |
| SCK (CLK)| P0.19       |
| CS  (/CS)| P0.17       |
| VCC      | 3.3V        |
| GND      | GND         |

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```
