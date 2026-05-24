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

| Day | Date | Topic | Folder |
|-----|------|-------|--------|
| 1 | 2026-05-16 | [Introduction to Zephyr RTOS](Learning%20Documents/2026-05-16%20-%20Introduction%20to%20Zephyr%20RTOS.md) | [src](src/2026-05-16_Introduction_to_Zephyr_RTOS/) |
| 2 | 2026-05-17 | [Project Structure and Build System](Learning%20Documents/2026-05-17%20-%20Zephyr%20Project%20Structure%20and%20Build%20System.md) | [src](src/2026-05-17_Project_Structure/) |
| 3 | 2026-05-18 | [Device Tree Basics](Learning%20Documents/2026-05-18%20-%20Device%20Tree%20Basics%20in%20Zephyr.md) | [src](src/2026-05-18_Device_Tree_Basics/) |
| 4 | 2026-05-19 | [Kconfig and prj.conf](Learning%20Documents/2026-05-19%20-%20Kconfig%20and%20prj.conf%20Configuration.md) | [src](src/2026-05-19_Kconfig/) |
| 5 | 2026-05-20 | [Writing a Basic Zephyr Application](Learning%20Documents/2026-05-20%20-%20Writing%20a%20Basic%20Zephyr%20Application.md) | [src](src/2026-05-20_Basic_Application/) |
| 6 | 2026-05-21 | [Drivers and Device Binding](Learning%20Documents/2026-05-21%20-%20Using%20Zephyr%20Drivers%20and%20Device%20Binding.md) | [src](src/2026-05-21_Drivers/) |
| 7 | 2026-05-22 | [Debugging and Testing](Learning%20Documents/2026-05-22%20-%20Debugging%20and%20Testing%20Zephyr%20Applications.md) | [src](src/2026-05-22_Debugging/) |
| 8 | 2026-05-23 | [Threads and Synchronization](Learning%20Documents/2026-05-23%20-%20Threads%20and%20Synchronization.md) | [src](src/2026-05-23_Threads_Sync/) |
| 9 | 2026-05-24 | [GPIO and Interrupts](Learning%20Documents/2026-05-24%20-%20GPIO%20and%20Interrupts.md) | [src](src/2026-05-24_GPIO_Interrupts/) |
| 10 | 2026-05-25 | [Custom PCB Board Definition](Learning%20Documents/2026-05-25%20-%20Custom%20PCB%20Board%20Definition.md) | [src](src/2026-05-25_Custom_PCB/) |
| 11 | 2026-05-26 | [USB CDC-ACM Communication](Learning%20Documents/2026-05-26%20-%20USB%20CDC-ACM%20Communication.md) | [src](src/2026-05-26_USB_CDC_ACM/) |
| 12 | 2026-05-27 | [CBOR Encoding over USB](Learning%20Documents/2026-05-27%20-%20CBOR%20Encoding%20over%20USB.md) | [src](src/2026-05-27_CBOR_USB/) |
| 13 | 2026-05-28 | [Zephyr Shell Subsystem](Learning%20Documents/2026-05-28%20-%20Zephyr%20Shell%20Subsystem.md) | [src](src/2026-05-28_Shell/) |
| 14 | 2026-05-29 | [I2C Sensor Integration (BME280)](Learning%20Documents/2026-05-29%20-%20I2C%20Sensor%20Integration.md) | [src](src/2026-05-29_I2C_Sensor/) |
| 15 | 2026-05-30 | [KXTJ3-1057 Accelerometer + USB Shell](Learning%20Documents/2026-05-30%20-%20KXTJ3-1057%20Accelerometer%20with%20USB%20Shell.md) | [src](src/2026-05-30_KXTJ3_Accel_USB_Shell/) |
| 16 | 2026-05-31 | [KXTJ3-1057 CBOR over USB + Python decoder](Learning%20Documents/2026-05-31%20-%20KXTJ3-1057%20Accelerometer%20CBOR%20over%20USB.md) | [src](src/2026-05-31_KXTJ3_CBOR_USB/) |
| 17 | 2026-06-01 | [KXTJ3 CBOR on Custom PCB](Learning%20Documents/2026-06-01%20-%20KXTJ3%20CBOR%20on%20Custom%20PCB.md) | [src](src/2026-06-01_KXTJ3_CBOR_Custom_PCB/) |
| 18 | 2026-06-02 | [BLE Peripheral Advertising](Learning%20Documents/2026-06-02%20-%20BLE%20Peripheral%20Advertising.md) | [src](src/2026-06-02_BLE_Peripheral/) |
| 19 | 2026-06-03 | [SPI Communication (W25Q Flash)](Learning%20Documents/2026-06-03%20-%20SPI%20Communication.md) | [src](src/2026-06-03_SPI_Flash/) |
| 20 | 2026-06-04 | [ADC Reading (Battery Voltage)](Learning%20Documents/2026-06-04%20-%20ADC%20Reading.md) | [src](src/2026-06-04_ADC/) |
| 21 | 2026-06-05 | [PWM LED Dimming](Learning%20Documents/2026-06-05%20-%20PWM%20LED%20Dimming.md) | [src](src/2026-06-05_PWM_LED/) |
| 22 | 2026-06-06 | [NVS Flash Storage](Learning%20Documents/2026-06-06%20-%20NVS%20Flash%20Storage.md) | [src](src/2026-06-06_NVS_Storage/) |
| 23 | 2026-06-07 | [Work Queues and Kernel Timers](Learning%20Documents/2026-06-07%20-%20Work%20Queues%20and%20Kernel%20Timers.md) | [src](src/2026-06-07_Work_Queues/) |
| 24 | 2026-06-08 | [Power Management and Sleep](Learning%20Documents/2026-06-08%20-%20Power%20Management%20and%20Sleep.md) | [src](src/2026-06-08_Power_Management_Sleep/) |

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
