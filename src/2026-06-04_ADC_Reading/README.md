# ADC Reading

Day 20: reads analog voltage from AIN2 (P0.04) every second and logs
the raw count and millivolt value.

## Wiring
Connect a signal (0–3.3V) or potentiometer wiper to P0.04 on the nRF52840 DK.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```
