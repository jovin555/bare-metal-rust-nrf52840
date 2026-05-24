# Appendix B — Glossary

Key terms used throughout the course.

---

**ADC** — Analog-to-Digital Converter. Converts a voltage on a GPIO pin to a
digital value. Used in Day 20 to read battery voltage via the nRF52840's
built-in SAADC peripheral.

**BLE** — Bluetooth Low Energy. A short-range wireless protocol optimised for
battery-powered devices. Used in Days 17, 18, and 26.

**CBOR** — Concise Binary Object Representation (RFC 7049). A binary encoding
format used in Days 12 and 16 to serialise sensor data compactly over USB
CDC-ACM.

**CDC-ACM** — USB Communications Device Class – Abstract Control Model. The USB
class that makes the nRF52840 appear as a virtual serial port (`/dev/ttyACM0`)
on the host PC. Used in Days 11–13, 15–16.

**DTS / Device Tree Source** — A hardware description file (`.dts` / `.overlay`)
that tells Zephyr which peripherals exist, where they are mapped, and how they
are connected. Central to every Zephyr project.

**GATT** — Generic Attribute Profile. The BLE protocol layer that defines
services and characteristics. Used in Days 17 and 26.

**GPIO** — General Purpose Input/Output. Digital pins driven high/low as output
or sampled as input. Used in Days 5, 9, and throughout.

**I2C** — Inter-Integrated Circuit. A two-wire serial bus (SDA + SCL) for
communicating with sensors and peripherals at short range. Used in Days 14–17
and 25–27.

**ISR** — Interrupt Service Routine. A function called directly by hardware when
an interrupt fires. In Zephyr, ISRs must not sleep, block, or allocate memory —
use a work item or semaphore to defer to a thread.

**Kconfig** — The Linux-inherited build configuration system. Boolean and integer
options that enable/disable subsystems at compile time. Configured via `prj.conf`.

**NVS** — Non-Volatile Storage. A simple key-value store on a dedicated flash
partition that persists data across resets and power cycles. Used in Days 22
and 25–27.

**PPK2** — Nordic Power Profiler Kit 2. A USB measurement device for sampling
current consumption down to ~100 nA, used to verify sleep-state current figures.

**PWM** — Pulse Width Modulation. Controls average power to an output (e.g., LED
brightness) by varying the duty cycle of a digital signal. Used in Day 21.

**SDK** — Software Development Kit. Here: the Zephyr SDK (version 1.0.1), which
bundles the ARM GCC toolchain, OpenOCD, and J-Link tools required to compile and
flash nRF52840 firmware.

**SPI** — Serial Peripheral Interface. A four-wire full-duplex bus (MOSI, MISO,
SCK, CS) for high-speed peripherals such as flash memory. Used in Day 19 with
the W25Q SPI flash.

**System OFF** — The deepest power state on the nRF52840. The CPU is fully
stopped and RAM is not retained. Quiescent current is typically 0.4–0.7 µA.
Wake is via GPIO edge or reset. Used in Day 24 and the capstone (Day 27).

**West** — Zephyr's meta-build tool. Manages the workspace, fetches modules
with `west update`, and wraps CMake/Ninja with `west build` and `west flash`.

**Work Queue** — A Zephyr kernel object (`struct k_work`, `struct k_work_q`) that
executes work items in a dedicated thread. Used in Day 23 to defer processing
from ISR context safely to thread context.
