# Embedded Firmware with Zephyr RTOS
### A Practical Guide from LED to Custom PCB

**Embedded Firmware with Zephyr RTOS** is a hands-on, 24-day guide for engineers who want to build real firmware on the nRF52840 — not just blink an LED, but ship production-ready code.

Each day introduces one concept, one working example, and one buildable project. You start with the Zephyr build system and Device Tree basics, then progress through threads, GPIO interrupts, USB communication, CBOR encoding, and BLE advertising. By the end, you will have written a custom I2C accelerometer driver, streamed live sensor data to a PC over USB, defined your own PCB board target, and put the processor into sub-µA sleep — the same techniques used in commercial wearables and IoT products.

**What's inside:**
- 24 structured lessons with working Zephyr source code for every day
- Custom driver development for sensors not supported by upstream Zephyr
- USB CDC-ACM, CBOR encoding, and a Python decoder tool for live data visualization
- BLE peripheral advertising, SPI flash, ADC, PWM, NVS storage, and power management
- A complete custom PCB board definition — ready to adapt to your own hardware
- Setup guide covering Zephyr 4.x, West, and SDK 1.0.1 on Linux

**Who this is for:** Embedded engineers with C experience who are new to Zephyr, developers moving from bare-metal or Arduino to an RTOS, and hardware engineers writing firmware for their own nRF52840-based designs.

**Hardware required:** Nordic nRF52840 DK and a USB cable.

> **Target hardware:** Nordic nRF52840 DK (`nrf52840dk/nrf52840`)
> **Zephyr version:** 4.x (latest main branch)
> **SDK:** Zephyr SDK 1.0.1

---

## What you will build

By the end of this course you will have written firmware that:

- Reads a 3-axis MEMS accelerometer (KXTJ3-1057) over I2C
- Encodes sensor data as CBOR and streams it over USB CDC-ACM
- Decodes and plots the stream live with a Python tool
- Advertises a BLE peripheral with custom manufacturer data
- Reads flash, ADC, and PWM peripherals
- Persists state to NVS across resets
- Enters µA-level sleep states with System OFF wake on GPIO
- Runs on a fully custom PCB board definition you write yourself

---

## Course outline

| | Topic | Folder |
|---|-------|--------|
| Setup 1 | [The Zephyr SDK Toolchain](Learning%20Documents/Setup%201%20%E2%80%94%20The%20Zephyr%20SDK%20Toolchain.md) | — |
| Setup 2 | [Zephyr Workspace and Folder Structure](Learning%20Documents/Setup%202%20%E2%80%94%20Zephyr%20Workspace%20and%20Folder%20Structure.md) | — |
| 1 | [Introduction to Zephyr RTOS](Learning%20Documents/Day%2001%20%E2%80%94%20Introduction%20to%20Zephyr%20RTOS.md) | [src](src/2026-05-16_Introduction_to_Zephyr_RTOS/) |
| 2 | [Zephyr Project Structure and Build System](Learning%20Documents/Day%2002%20%E2%80%94%20Zephyr%20Project%20Structure%20and%20Build%20System.md) | [src](src/2026-05-17_Zephyr_Project_Structure/) |
| 3 | [Device Tree Basics in Zephyr](Learning%20Documents/Day%2003%20%E2%80%94%20Device%20Tree%20Basics%20in%20Zephyr.md) | [src](src/2026-05-18_Device_Tree_Basics/) |
| 4 | [Kconfig and prj.conf Configuration](Learning%20Documents/Day%2004%20%E2%80%94%20Kconfig%20and%20prj.conf%20Configuration.md) | [src](src/2026-05-19_Kconfig_prj_conf/) |
| 5 | [Writing a Basic Zephyr Application](Learning%20Documents/Day%2005%20%E2%80%94%20Writing%20a%20Basic%20Zephyr%20Application.md) | [src](src/2026-05-20_Basic_Zephyr_Application/) |
| 6 | [Using Zephyr Drivers and Device Binding](Learning%20Documents/Day%2006%20%E2%80%94%20Using%20Zephyr%20Drivers%20and%20Device%20Binding.md) | [src](src/2026-05-21_Zephyr_Drivers_and_Binding/) |
| 7 | [Debugging and Testing Zephyr Applications](Learning%20Documents/Day%2007%20%E2%80%94%20Debugging%20and%20Testing%20Zephyr%20Applications.md) | [src](src/2026-05-22_Debugging_and_Testing/) |
| 8 | [Threads and Synchronization](Learning%20Documents/Day%2008%20%E2%80%94%20Threads%20and%20Synchronization.md) | [src](src/2026-05-23_Threads_and_Synchronization/) |
| 9 | [GPIO and Interrupts](Learning%20Documents/Day%2009%20%E2%80%94%20GPIO%20and%20Interrupts.md) | [src](src/2026-05-24_GPIO_and_Interrupts/) |
| 10 | [Custom PCB Board Definition](Learning%20Documents/Day%2010%20%E2%80%94%20Custom%20PCB%20Board%20Definition.md) | [src](src/2026-05-25_Custom_PCB_Board_Definition/) |
| 11 | [USB CDC-ACM Communication](Learning%20Documents/Day%2011%20%E2%80%94%20USB%20CDC-ACM%20Communication.md) | [src](src/2026-05-26_USB_CDC_ACM/) |
| 12 | [CBOR Encoding over USB](Learning%20Documents/Day%2012%20%E2%80%94%20CBOR%20Encoding%20over%20USB.md) | [src](src/2026-05-27_CBOR_over_USB/) |
| 13 | [Zephyr Shell Subsystem](Learning%20Documents/Day%2013%20%E2%80%94%20Zephyr%20Shell%20Subsystem.md) | [src](src/2026-05-28_Zephyr_Shell/) |
| 14 | [I2C Sensor Integration](Learning%20Documents/Day%2014%20%E2%80%94%20I2C%20Sensor%20Integration.md) | [src](src/2026-05-29_I2C_Sensor_Integration/) |
| 15 | [KXTJ3-1057 Accelerometer with USB Shell](Learning%20Documents/Day%2015%20%E2%80%94%20KXTJ3-1057%20Accelerometer%20with%20USB%20Shell.md) | [src](src/2026-05-30_KXTJ3_Accel_USB_Shell/) |
| 16 | [KXTJ3-1057 Accelerometer CBOR over USB](Learning%20Documents/Day%2016%20%E2%80%94%20KXTJ3-1057%20Accelerometer%20CBOR%20over%20USB.md) | [src](src/2026-05-31_KXTJ3_CBOR_USB/) |
| 17 | [BLE GATT Services](Learning%20Documents/Day%2017%20%E2%80%94%20BLE%20GATT%20Services.md) | [src](src/2026-06-01_BLE_GATT_Service/) |
| 18 | [BLE Peripheral Advertising](Learning%20Documents/Day%2018%20%E2%80%94%20BLE%20Peripheral%20Advertising.md) | [src](src/2026-06-02_BLE_Peripheral_Advertising/) |
| 19 | [SPI Communication](Learning%20Documents/Day%2019%20%E2%80%94%20SPI%20Communication.md) | [src](src/2026-06-03_SPI_Communication/) |
| 20 | [ADC Reading](Learning%20Documents/Day%2020%20%E2%80%94%20ADC%20Reading.md) | [src](src/2026-06-04_ADC_Reading/) |
| 21 | [PWM LED Dimming](Learning%20Documents/Day%2021%20%E2%80%94%20PWM%20LED%20Dimming.md) | [src](src/2026-06-05_PWM_LED_Dimming/) |
| 22 | [NVS Flash Storage](Learning%20Documents/Day%2022%20%E2%80%94%20NVS%20Flash%20Storage.md) | [src](src/2026-06-06_NVS_Flash_Storage/) |
| 23 | [Work Queues and Kernel Timers](Learning%20Documents/Day%2023%20%E2%80%94%20Work%20Queues%20and%20Kernel%20Timers.md) | [src](src/2026-06-07_Work_Queues_and_Timers/) |
| 24 | [Power Management and Sleep](Learning%20Documents/Day%2024%20%E2%80%94%20Power%20Management%20and%20Sleep.md) | [src](src/2026-06-08_Power_Management_Sleep/) |
| 25 | [Capstone Part 1 — Sensor and NVS Integration](Learning%20Documents/Day%2025%20%E2%80%94%20Capstone%20Part%201%20%E2%80%94%20Sensor%20and%20NVS%20Integration.md) | [src](src/capstone_wireless_sensor_node/) |
| 26 | [Capstone Part 2 — BLE GATT Environmental Service](Learning%20Documents/Day%2026%20%E2%80%94%20Capstone%20Part%202%20%E2%80%94%20BLE%20GATT%20Environmental%20Service.md) | [src](src/capstone_wireless_sensor_node/) |
| 27 | [Capstone Part 3 — Sleep and Wake-Up Loop](Learning%20Documents/Day%2027%20%E2%80%94%20Capstone%20Part%203%20%E2%80%94%20Sleep%20and%20Wake-Up%20Loop.md) | [src](src/capstone_wireless_sensor_node/) |

| | [Appendix A — Troubleshooting](Learning%20Documents/Appendix%20A%20%E2%80%94%20Troubleshooting.md) | |
| | [Appendix B — Glossary](Learning%20Documents/Appendix%20B%20%E2%80%94%20Glossary.md) | |

---

## Prerequisites

- Nordic nRF52840 DK (or compatible nRF52840 board)
- Linux or macOS host (Ubuntu 22.04 recommended)
- USB cable (data, not charge-only)
- Python 3.12

## Quick start

See [Docs/Zephyr_Workspace_Setup.md](Docs/Zephyr_Workspace_Setup.md) for the full workspace setup guide.

```bash
# Activate workspace (run at the start of every session)
source /path/to/myworkspace/.venv/bin/activate
export ZEPHYR_BASE=/path/to/myworkspace/zephyr

# Build and flash Day 1
cd src/2026-05-16_Introduction_to_Zephyr_RTOS
west build -b nrf52840dk/nrf52840
west flash
```

---

## Repository structure

```
.
├── Docs/                        # Setup guides and reference docs
├── Learning Documents/          # One MD + PDF per day
└── src/                         # One buildable Zephyr project per day
    ├── 2026-05-16_Introduction_to_Zephyr_RTOS/
    ├── ...
    └── 2026-06-08_Power_Management_Sleep/
```

---

## License

Code: MIT  
Learning documents: Creative Commons BY 4.0
