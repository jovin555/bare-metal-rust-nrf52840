# Capstone — Wireless Sensor Node (Days 25–27)

Complete firmware combining BME280 I2C sensing, BLE GATT notifications,
NVS persistent storage, and System OFF sleep.

See the course learning documents for the full annotated source:
- Day 25 — Sensor and NVS Integration
- Day 26 — BLE GATT Environmental Service
- Day 27 — Sleep and Wake-Up Loop

## Build

```bash
source /home/jovin/workspace/My-Zephyr-project/myworkspace/.venv/activate
export ZEPHYR_BASE=/home/jovin/workspace/My-Zephyr-project/myworkspace/zephyr
west build -b nrf52840dk/nrf52840
west flash
```
