# I2C Sensor Integration

Day 14 example: reads temperature, pressure, and humidity from a BME280 over I2C
and logs the values every 2 seconds.

## Wiring (nRF52840 DK)
| BME280 pin | nRF52840 DK pin |
|------------|-----------------|
| VCC        | 3.3V            |
| GND        | GND             |
| SDA        | P0.26           |
| SCL        | P0.27           |

If your BME280 module has SDO pulled high, change `reg = <0x76>` to `<0x77>` in app.overlay.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```
