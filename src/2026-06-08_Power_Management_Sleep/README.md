# Power Management and Sleep

Day 24: demonstrates Zephyr automatic sleep between 5-second work cycles,
plus System OFF deep sleep triggered by holding button sw0.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Observe
- Connect J-Link RTT Viewer or `west debug` / `JLinkRTTClient` to see log output.
- Normal cycle: LED toggles, then sleeps 5 s (PM subsystem enters low-power state).
- Hold sw0 for > 2 s → enters System OFF (~0.4 µA).
- Press any button or RESET to wake from System OFF.

## Measuring current
Use nRF PPK2 or µCurrent meter in series with VDD.
Expected: ~8 µA average during 5 s sleep cycles.
