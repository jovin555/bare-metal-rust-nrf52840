# BLE Peripheral Advertising

Day 18: nRF52840 advertises as "MyZephyrDevice" and restarts advertising
after each disconnection.

## Build and flash
```bash
west build -b nrf52840dk/nrf52840
west flash
```

## Test
Open nRF Connect (Android/iOS) → Scanner → find "MyZephyrDevice".
Connect and disconnect — confirm advertising restarts automatically.
