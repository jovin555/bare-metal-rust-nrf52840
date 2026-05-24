# Embedded Firmware with Zephyr RTOS
## A Practical Guide from LED to Custom PCB

> **Author:** Jovin  
> **Target hardware:** Nordic nRF52840 DK  
> **Zephyr version:** 4.x  
> **SDK:** Zephyr SDK 1.0.1  

---

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

---


---

# Before You Begin — Workspace Setup

Before Day 1, make sure your Zephyr development environment is ready. This book targets **Zephyr 4.x** with **SDK 1.0.1** on Ubuntu 22.04.

## Requirements
- Ubuntu 22.04 (or WSL2 on Windows)
- Nordic nRF52840 DK + USB cable (data, not charge-only)
- Python 3.12 (`/usr/bin/python3.12`)
- ~5 GB free disk space

## Quick setup (copy-paste)

```bash
# 1. Create workspace and virtual environment
mkdir -p ~/workspace/myworkspace
cd ~/workspace/myworkspace
python3.12 -m venv .venv
source .venv/bin/activate
pip install west

# 2. Initialize Zephyr
unset ZEPHYR_BASE
west init .
west update
pip install -r zephyr/scripts/requirements.txt

# 3. Download Zephyr SDK 1.0.1
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/zephyr-sdk-1.0.1_linux-x86_64.tar.xz
tar xf zephyr-sdk-1.0.1_linux-x86_64.tar.xz
cd zephyr-sdk-1.0.1
./setup.sh
```

## Session setup (run at the start of every terminal session)

```bash
source ~/workspace/myworkspace/.venv/bin/activate
export ZEPHYR_BASE=~/workspace/myworkspace/zephyr
```

## Verify the setup

```bash
cd ~/workspace/myworkspace/zephyr/samples/basic/blinky
west build -b nrf52840dk/nrf52840
# Expected: "ninja: build stopped: subcommand failed." only if board not connected
# Expected: build/zephyr/zephyr.elf exists on success
```

> **Troubleshooting:** If you see `Could NOT find Python3 (version >= 3.12)`, your venv was created with the wrong Python. Delete `.venv/` and re-run `python3.12 -m venv .venv`. If you see `Could not find Zephyr-sdk compatible with version 1.0`, you have an old SDK — download SDK 1.0.1 above.

For full details see the [Workspace Setup Guide](https://github.com/jovin555/My-Zephyr-project/blob/master/Docs/Zephyr_Workspace_Setup.md).

---

# Day 1 — Introduction to Zephyr RTOS

## Goal
Learn what Zephyr is, why it exists, and how it differs from other embedded RTOS options.

## What you will learn
- What Zephyr RTOS is and its core use cases
- How Zephyr supports multiple architectures and boards
- The difference between Zephyr application code and Zephyr kernel/SDK
- The role of West, CMake, Kconfig, and Device Tree in Zephyr

## Overview
Zephyr is a small, scalable real-time operating system for embedded devices. It is designed for constrained hardware and supports a wide range of microcontrollers, sensors, networking stacks, and boards.

### Key concepts
- **Kernel**: task scheduling, threads, synchronization, and timers
- **Subsystems**: networking, file systems, Bluetooth, sensor frameworks
- **Build system**: Zephyr uses CMake and West to manage projects and dependencies
- **Configuration**: Kconfig controls compile-time features and board-specific settings
- **Device Tree**: describes hardware peripherals and pin mappings

## Why this matters
Understanding these concepts helps you read Zephyr examples, choose the right board and SDK, and avoid common mistakes when starting a new project.

## Practice tasks
1. Inspect the Zephyr repository or samples to identify core directories: `zephyr/kernel`, `zephyr/subsys`, `zephyr/samples`.
2. Compare Zephyr to a bare-metal project: note what Zephyr provides out of the box.
3. Make a list of one board and one feature you want to explore next.

## Example folder
View the complete source code on GitHub: [src/2026-05-16_Introduction_to_Zephyr_RTOS](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-16_Introduction_to_Zephyr_RTOS)

## Next topic
Tomorrow, we will explore Zephyr project structure and the build system in depth.
---


# Day 2 — Zephyr Project Structure and Build System

## Goal
Understand how a Zephyr application is organized and how the build system compiles it.

## What you will learn
- The role of `CMakeLists.txt` and `prj.conf`
- How West manages Zephyr projects and sample repositories
- The layout of a typical Zephyr application
- How to build and run a sample application

## Overview
A Zephyr application consists of source files, build metadata, and configuration files. The Zephyr build system uses CMake to generate build targets and West as a project manager.

### Typical files
- `CMakeLists.txt`: defines the application build, sources, and dependencies
- `prj.conf`: selects Zephyr features with Kconfig symbols
- `src/main.c`: application entry point
- `boards/` or board-specific overlays: optional hardware configuration

### Build flow
1. `west init` / `west update` to fetch Zephyr and modules
2. `west build -b <board> <app-dir>` to configure and compile
3. `west flash` or `west debug` to deploy and run

## Why this matters
Knowing the build structure helps you create valid Zephyr apps and troubleshoot build failures.

## Practice tasks
1. Create a minimal Zephyr app directory with `CMakeLists.txt`, `prj.conf`, and `src/main.c`.
2. Build a sample for a supported board such as `native_posix` or your target board.
3. Open the generated `build/zephyr/zephyr.elf` or build log to inspect compile steps.

## Example folder
View the complete source code on GitHub: [src/2026-05-17_Zephyr_Project_Structure](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-17_Zephyr_Project_Structure)

## Next topic
Tomorrow, we will cover Device Tree basics and how Zephyr maps hardware in software.
---


# Day 3 — Device Tree Basics in Zephyr

## Goal
Learn the fundamentals of Zephyr Device Tree and how hardware is described for your board.

## What you will learn
- What a Device Tree is and why Zephyr uses it
- The structure of `.dts` and overlay files
- How Zephyr uses `devicetree.h` macros to access hardware
- Common node properties and bindings

## Overview
Device Tree is a declarative hardware description language. In Zephyr, it describes peripherals, pin assignments, and board-specific configuration without hard-coding them in C.

### Key concepts
- **Node**: a hardware block such as `i2c0`, `gpio0`, or `uart1`
- **Property**: settings like `reg`, `status`, `label`, `pinctrl-0`
- **Overlay**: custom modifications on top of the base board tree
- **Bindings**: JSON files that define allowed properties for a node type

### Common macros
- `DT_NODELABEL(...)`
- `DT_ANY_INST_ON_BUS(...)`
- `DEVICE_DT_GET(...)`
- `DT_PROP(...)`

## Why this matters
Device Tree allows portable hardware code, so the same application can run on different boards with only config changes.

## Practice tasks
1. Open a board’s `.dts` file and locate an I2C or GPIO node.
2. Create an `app.overlay` that enables a peripheral and changes its `status` to `okay`.
3. Write a tiny `main.c` that fetches the device with `DEVICE_DT_GET`.

## Example folder
View the complete source code on GitHub: [src/2026-05-18_Device_Tree_Basics](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-18_Device_Tree_Basics)

## Next topic
Tomorrow, we will study Kconfig and `prj.conf` configuration in Zephyr.
---


# Day 4 — Kconfig and prj.conf Configuration

## Goal
Understand how Zephyr features are enabled and configured using Kconfig and `prj.conf`.

## What you will learn
- The purpose of Kconfig in Zephyr
- How to enable features with `CONFIG_*` symbols
- How to search for configuration options
- How `prj.conf` affects the build

## Overview
Zephyr uses Kconfig to manage thousands of compile-time options. `prj.conf` is the application-specific file where you select the features your project needs.

### Examples
- `CONFIG_LOG=y` enables the logging subsystem
- `CONFIG_UART_CONSOLE=y` enables console output over UART
- `CONFIG_I2C=y` enables the I2C API
- `CONFIG_MAIN_STACK_SIZE=1024` sets thread stack size

## Why this matters
Correct configuration helps reduce firmware size, avoid conflicts, and enable the right drivers and services.

## Practice tasks
1. Open `zephyr/Kconfig` or use `west build -t menuconfig` if available.
2. Add logging and console output to `prj.conf`.
3. Try enabling and disabling a feature to see how it affects compile output.

## Example folder
View the complete source code on GitHub: [src/2026-05-19_Kconfig_prj_conf](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-19_Kconfig_prj_conf)

## Next topic
Tomorrow, we will write a basic Zephyr application with main loop and device initialization.
---


# Day 5 — Writing a Basic Zephyr Application

## Goal
Build a simple Zephyr application from scratch and understand the entry point and runtime model.

## What you will learn
- How `main()` works in Zephyr
- Using `printk()` and `LOG_*` for output
- Basic thread usage and delays
- Initializing devices at runtime

## Overview
A Zephyr app typically starts in `main()`, which runs as a thread under the Zephyr kernel. From there, you can initialize peripherals, configure timers, and start application logic.

### Typical code
- `void main(void)` is the app entry point
- `printk()` writes to the system console
- `k_sleep(K_SECONDS(x))` pauses the current thread
- `device_get_binding()` or `DEVICE_DT_GET` obtains peripheral handles

## Why this matters
This is the core of application development: writing code that interacts with the kernel and hardware.

## Practice tasks
1. Create `src/main.c` with a blinking LED or periodic console message.
2. Add a thread using `K_THREAD_DEFINE` or `k_thread_create`.
3. Use `k_sleep()` and `printk()` to observe behavior.

## Example folder
View the complete source code on GitHub: [src/2026-05-20_Basic_Zephyr_Application](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-20_Basic_Zephyr_Application)

## Next topic
Tomorrow, we will learn how to use Zephyr drivers and device binding properly.
---


# Day 6 — Using Zephyr Drivers and Device Binding

## Goal
Learn how Zephyr drivers are accessed in code and how to bind to hardware devices.

## What you will learn
- The difference between `device_get_binding()` and `DEVICE_DT_GET()`
- How to use Zephyr device driver APIs
- How to check device readiness
- How to work with common peripherals like GPIO, I2C, and UART

## Overview
Zephyr provides a device model where peripherals are exposed as `struct device` objects. Your application can request these devices by name or via Device Tree macros.

### Driver access patterns
- `device_get_binding("UART_0")` for named devices
- `DEVICE_DT_GET(DT_NODELABEL(uart0))` for Device Tree-based access
- `device_is_ready(dev)` to verify the device

### Example workflow
1. Enable the peripheral in `prj.conf` and overlay
2. Get the device binding in `main()`
3. Use the driver API for operations like GPIO set/reset or I2C read/write

### Error handling — always check return values

Every Zephyr driver API returns an integer: `0` on success, negative errno on failure. Always check it:

```c
int ret;

ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
if (ret < 0) {
    LOG_ERR("Failed to configure LED GPIO: %d", ret);
    return;
}

ret = gpio_pin_set_dt(&led, 1);
if (ret < 0) {
    LOG_ERR("Failed to set LED: %d", ret);
}
```

Common error codes:
| Code | Meaning |
|------|---------|
| `-ENODEV` | Device not found or not ready |
| `-EIO` | I2C/SPI communication failure |
| `-EINVAL` | Invalid argument passed |
| `-EBUSY` | Bus or resource busy |
| `-ENOTSUP` | Operation not supported by this driver |

Use `LOG_ERR("msg: %d", ret)` rather than `printk` so log levels can silence it in production builds. Never silently ignore a negative return — it masks hardware faults.


## Why this matters
Correct device binding ensures your code works with the actual hardware and is portable across boards.

## Practice tasks
1. Bind to a GPIO device and toggle an LED.
2. Bind to a UART device and send a test string.
3. Explore a sample driver API such as sensor, PWM, or I2C.

## Example folder
View the complete source code on GitHub: [src/2026-05-21_Zephyr_Drivers_and_Binding](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-21_Zephyr_Drivers_and_Binding)

## Next topic
Tomorrow, we will cover debugging and testing Zephyr applications.
---


# Day 7 — Debugging and Testing Zephyr Applications

## Goal
Understand the Zephyr debugging and testing workflow for finding and fixing issues.

## What you will learn
- How to use logging and console output for debugging
- How to run Zephyr tests and sample validation
- The role of `west test`, `west debug`, and `west flash`
- How to interpret build and runtime errors

## Overview
Debugging Zephyr often involves enabling log output, using debugger tools, and running built-in test suites. These tools help you verify that your code and configuration are correct.

### Debug techniques
- `printk()` and `LOG_INF()` for runtime messages
- `west debug` to attach GDB to the target
- `west test` for Zephyr test cases
- `west build -t run` on simulation boards such as `native_posix`

## Why this matters
Good debugging practices accelerate learning, reduce frustration, and help you understand how your application interacts with Zephyr internals.

## Practice tasks
1. Add logging to an existing Zephyr app and watch the console output.
2. Run a sample on `native_posix` or a simulator with `west build -b native_posix`.
3. Review build warnings and fix one configuration or code issue.

## Example folder
View the complete source code on GitHub: [src/2026-05-22_Debugging_and_Testing](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-22_Debugging_and_Testing)

## Next topic
After this, you can continue with advanced Zephyr concepts such as Bluetooth, networking, and power management.
---


# Day 8 — Threads and Synchronization

## Goal
Learn how to create and manage multiple threads in Zephyr and synchronize them using semaphores, mutexes, and message queues.

## What you will learn
- How to define and start threads using `K_THREAD_DEFINE`
- How to use `k_sem`, `k_mutex`, and `k_msgq` for synchronization
- Thread priorities and cooperative vs preemptive scheduling
- How to pass data between threads safely

## Overview
Zephyr is a preemptive RTOS. Each thread has a priority and a stack. Threads run concurrently and share resources, so synchronization primitives are needed to avoid race conditions.

### Key concepts

**Defining a thread:**
```c
K_THREAD_DEFINE(my_thread, 1024, my_thread_fn, NULL, NULL, NULL, 5, 0, 0);
```
Arguments: name, stack size, entry function, arg1, arg2, arg3, priority, options, delay.

**Semaphore — signal between threads:**
```c
K_SEM_DEFINE(my_sem, 0, 1);
k_sem_give(&my_sem);   // signal
k_sem_take(&my_sem, K_FOREVER);  // wait
```

**Mutex — protect shared data:**
```c
K_MUTEX_DEFINE(my_mutex);
k_mutex_lock(&my_mutex, K_FOREVER);
// critical section
k_mutex_unlock(&my_mutex);
```

**Message queue — pass data between threads:**
```c
K_MSGQ_DEFINE(my_queue, sizeof(int), 10, 4);
k_msgq_put(&my_queue, &value, K_NO_WAIT);
k_msgq_get(&my_queue, &value, K_FOREVER);
```

### Thread priorities
- Lower number = higher priority
- Negative priorities are cooperative (must yield explicitly)
- Priorities 0–14 are preemptive by default

### Stack sizing guide

The stack size passed to `K_THREAD_DEFINE` is one of the most common sources of crashes. Too small → silent stack overflow → corrupted memory.

**Rules of thumb:**

| Thread workload | Minimum stack |
|----------------|--------------|
| Simple loop, no I/O | 512 bytes |
| Uses `printk` or `LOG_*` | 1024 bytes |
| Calls I2C, SPI, or ADC drivers | 2048 bytes |
| Uses USB or Shell subsystem | 4096 bytes |
| Uses BLE stack | 4096–8192 bytes |

Always add 25% headroom. Check actual usage at runtime:

```c
#include <zephyr/kernel.h>

extern struct k_thread my_thread_data;

void print_stack_usage(void)
{
    size_t unused;
    k_thread_stack_space_get(&my_thread_data, &unused);
    printk("Stack unused: %zu bytes\n", unused);
}
```

Enable `CONFIG_THREAD_ANALYZER=y` and `CONFIG_THREAD_ANALYZER_AUTO=y` in `prj.conf` to automatically log all thread stack usage at boot.

**Priority inversion and deadlock:**
- If thread A (low priority) holds a mutex and thread B (high priority) waits on it, B is blocked indefinitely — this is priority inversion. Use `K_MUTEX_DEFINE` with `CONFIG_MUTEX_ENABLE_PRIORITY_INHERITANCE=y` to mitigate it.
- Deadlock occurs when thread A waits on thread B and thread B waits on thread A. Always lock mutexes in the same order across all threads.


## Why this matters
Almost every real embedded application uses multiple threads: one for sensors, one for communication, one for UI. Understanding Zephyr threading is essential before building any non-trivial application.

## Practice tasks
1. Create two threads at different priorities and observe which runs first.
2. Use a semaphore to have thread A trigger thread B.
3. Pass an integer counter from a producer thread to a consumer thread via `k_msgq`.

## Example folder
View the complete source code on GitHub: [src/2026-05-23_Threads_and_Synchronization](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-23_Threads_and_Synchronization)

## Next topic
Tomorrow we explore GPIO configuration and interrupt-driven input handling.

---


# Day 9 — GPIO and Interrupts

## Goal
Learn how to configure GPIO pins in Zephyr for input and output, and how to respond to pin state changes using hardware interrupts.

## What you will learn
- How to get a GPIO device using the Device Tree
- How to configure a pin as output (LED) and input (button)
- How to register a GPIO interrupt callback
- How to debounce a button in software

## Overview
Zephyr's GPIO API abstracts hardware pin control across all supported boards. Pin configuration is done via the Device Tree, and the C API calls reference DT node labels.

### Key concepts

**Controlling an LED:**
```c
const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
if (ret < 0) {
    LOG_ERR("LED config failed: %d", ret);
    return;
}
gpio_pin_set_dt(&led, 1);   // on
gpio_pin_set_dt(&led, 0);   // off
gpio_pin_toggle_dt(&led);   // toggle
```

**Reading a button:**
```c
const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
gpio_pin_configure_dt(&btn, GPIO_INPUT);
int val = gpio_pin_get_dt(&btn);
```

**Interrupt callback:**
```c
static struct gpio_callback btn_cb;

void btn_handler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    printk("Button pressed!\n");
}

gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);
gpio_init_callback(&btn_cb, btn_handler, BIT(btn.pin));
gpio_add_callback(btn.port, &btn_cb);
```

### nRF52840 DK aliases
- `led0` through `led3` — onboard LEDs
- `sw0` through `sw3` — onboard buttons

## Why this matters
GPIO is the foundation of hardware interaction. LEDs, buttons, relays, and many sensors are wired directly to GPIO pins. Interrupts let you respond instantly without polling.

## Practice tasks
1. Toggle `led0` every 500 ms using `gpio_pin_toggle_dt`.
2. Read `sw0` in a loop and mirror its state to `led1`.
3. Use an interrupt on `sw0` to toggle `led0` on each press.

## Example folder
View the complete source code on GitHub: [src/2026-05-24_GPIO_and_Interrupts](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-24_GPIO_and_Interrupts)

## Next topic
Tomorrow we learn how to define a custom PCB as a Zephyr board target.

---


# Day 10 — Custom PCB Board Definition

## Goal
Learn how to define a custom PCB as a named Zephyr board target so that your hardware is fully described by Device Tree and Kconfig rather than relying on a stock evaluation kit.

## What you will learn
- The file structure required for a custom board
- How to write a board-level `.dts` and `_defconfig`
- How to register GPIO, UART, I2C, and SPI peripherals for your PCB
- How to build and flash targeting your custom board name

## Overview
When you design your own PCB around a chip like the nRF52840, you create a board definition that tells Zephyr exactly which peripherals are wired up and how. This replaces the DK-specific assumptions in `nrf52840dk/nrf52840`.

### Required file structure

```
boards/
└── arm/
    └── my_custom_board/
        ├── Kconfig.board          ← board name declaration
        ├── Kconfig.defconfig      ← default Kconfig symbols
        ├── board.cmake            ← flash runner (JLink, OpenOCD, etc.)
        ├── my_custom_board.dts    ← hardware description
        ├── my_custom_board.yaml   ← board metadata
        └── my_custom_board_defconfig  ← minimal kernel config
```

### my_custom_board.dts (skeleton)
```dts
/dts-v1/;
#include <nordic/nrf52840_qiaa.dtsi>

/ {
    model = "My Custom Board";
    compatible = "my-company,my-custom-board";

    chosen {
        zephyr,console = &uart0;
        zephyr,shell-uart = &uart0;
        zephyr,sram = &sram0;
        zephyr,flash = &flash0;
    };

    leds {
        compatible = "gpio-leds";
        led0: led_0 {
            gpios = <&gpio0 13 GPIO_ACTIVE_LOW>;
            label = "Status LED";
        };
    };

    aliases {
        led0 = &led0;
    };
};

&uart0 {
    status = "okay";
    compatible = "nordic,nrf-uarte";
    current-speed = <115200>;
    pinctrl-0 = <&uart0_default>;
    pinctrl-names = "default";
};

&i2c0 {
    status = "okay";
    pinctrl-0 = <&i2c0_default>;
    pinctrl-names = "default";
    clock-frequency = <I2C_BITRATE_FAST>;
};
```

### Kconfig.board
```kconfig
config BOARD_MY_CUSTOM_BOARD
    bool "My Custom Board"
    select SOC_NRF52840_QIAA
```

### board.cmake
```cmake
board_runner_args(jlink "--device=nRF52840_xxAA" "--speed=4000")
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
```

### my_custom_board_defconfig
```kconfig
CONFIG_SOC_SERIES_NRF52X=y
CONFIG_SOC_NRF52840_QIAA=y
CONFIG_BOARD_MY_CUSTOM_BOARD=y
CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC=64000000
CONFIG_GPIO=y
CONFIG_SERIAL=y
```

### Building for your custom board
Place the `boards/` directory either:
- Inside your application directory, or
- In a separate module registered via `west.yml`

Then build with:
```bash
west build -b my_custom_board
```

### Pin control (nRF52840 specific)
nRF52840 uses `pinctrl` to assign peripheral functions to physical pins. Add a `pinctrl` block to your `.dts`:
```dts
&pinctrl {
    uart0_default: uart0_default {
        group1 {
            psels = <NRF_PSEL(UART_TX, 0, 6)>,
                    <NRF_PSEL(UART_RX, 0, 8)>;
        };
    };
    i2c0_default: i2c0_default {
        group1 {
            psels = <NRF_PSEL(TWIM_SDA, 0, 26)>,
                    <NRF_PSEL(TWIM_SCL, 0, 27)>;
        };
    };
};
```
Replace pin numbers with those from your PCB schematic.

## Why this matters
Every production product needs a custom board definition. Keeping hardware description in DTS and Kconfig means your application code stays portable and board-agnostic.

## Practice tasks
1. Copy the `nrf52840dk` board directory from the Zephyr tree and rename it to `my_custom_board`.
2. Change the LED GPIO pin to match a pin on your schematic.
3. Build the example targeting `my_custom_board` — see [src/2026-05-25_Custom_PCB_Board_Definition](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-25_Custom_PCB_Board_Definition).

## Example folder
View the complete source code on GitHub: [src/2026-05-25_Custom_PCB_Board_Definition](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-25_Custom_PCB_Board_Definition)

## Next topic
Tomorrow we enable USB CDC-ACM so the board can communicate with a PC over USB.

---


# Day 11 — USB CDC-ACM Communication

## Goal
Enable USB communication on the nRF52840 so the board appears as a virtual serial port (COM port) on a connected PC.

## What you will learn
- What USB CDC-ACM is and why it is the simplest USB serial option
- How to enable the USB subsystem and CDC-ACM class in Kconfig
- How to redirect the Zephyr console to the USB virtual serial port
- How to write and read raw bytes over USB from application code

## Overview
CDC-ACM (Communications Device Class – Abstract Control Model) makes your microcontroller appear as a standard USB serial port. No custom driver is needed on Windows, Linux, or macOS. Data flows through the same `uart_poll_out` / `uart_fifo_read` API you already know.

### prj.conf settings
```kconfig
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_DEVICE_PRODUCT="My Zephyr USB Device"
CONFIG_USB_DEVICE_VID=0x2FE3
CONFIG_USB_DEVICE_PID=0x0100
CONFIG_USB_CDC_ACM=y

# Route the Zephyr console through USB CDC-ACM
CONFIG_UART_LINE_CTRL=y
CONFIG_USB_UART_CONSOLE=y
CONFIG_UART_CONSOLE=y
CONFIG_LOG=y
CONFIG_LOG_BACKEND_UART=y
```

### app.overlay — bind CDC-ACM to the console
```dts
/ {
    chosen {
        zephyr,console = &cdc_acm_uart0;
        zephyr,shell-uart = &cdc_acm_uart0;
    };
};

&zephyr_udc0 {
    cdc_acm_uart0: cdc_acm_uart0 {
        compatible = "zephyr,cdc-acm-uart";
        label = "CDC_ACM_0";
    };
};
```

### Initializing USB in application code
```c
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>

const struct device *usb_uart = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

void main(void)
{
    int ret = usb_enable(NULL);
    if (ret < 0) {
        LOG_ERR("USB enable failed: %d", ret);
        return;
    }

    // Wait for DTR — the PC has opened the COM port
    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_uart, UART_LINE_CTRL_DTR, &dtr);
        k_msleep(100);
    }

    printk("USB serial ready!\n");
}
```

### Reading incoming bytes
```c
uint8_t buf[64];
int len = uart_fifo_read(usb_uart, buf, sizeof(buf));
```

### Connecting from a PC
- **Linux**: appears as `/dev/ttyACM0`
- **macOS**: appears as `/dev/tty.usbmodem*`
- **Windows**: appears as `COMx` in Device Manager

Use any serial terminal (minicom, PuTTY, screen) at any baud rate (CDC-ACM ignores baud).

## Why this matters
USB serial is the most practical way to exchange data between an embedded device and a PC without dedicated RF hardware. It is the foundation for the CBOR-over-USB example in the next session.

## Practice tasks
1. Enable USB CDC-ACM and redirect `printk` output to the USB port.
2. Open a terminal on your PC and confirm you receive Zephyr log output.
3. Send a byte from the PC and echo it back from the firmware.

## Example folder
View the complete source code on GitHub: [src/2026-05-26_USB_CDC_ACM](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-26_USB_CDC_ACM)

## Next topic
Tomorrow we encode structured data using CBOR and send it over this USB connection.

---


# Day 12 — CBOR Encoding over USB

## Goal
Use the CBOR (Concise Binary Object Representation) format to encode structured sensor data on the nRF52840 and transmit it over the USB CDC-ACM virtual serial port established in Day 11.

## What you will learn
- What CBOR is and why it is used in embedded systems
- How to enable the `zcbor` library in Zephyr
- How to encode a map (key-value structure) with `zcbor`
- How to send the encoded binary frame over USB and decode it on a PC

## Overview
CBOR (RFC 8949) is a binary data format similar to JSON but much more compact — ideal for bandwidth-constrained links like USB serial or BLE. Zephyr ships with `zcbor`, a code-generated CBOR library with a streaming encoder/decoder.

### Enable zcbor in prj.conf
```kconfig
CONFIG_ZCBOR=y
CONFIG_ZCBOR_STOP_ON_ERROR=y
```

### Encoding a simple map
```c
#include <zcbor_encode.h>

uint8_t buf[64];
struct zcbor_state_t states[2];
zcbor_new_encode_state(states, ARRAY_SIZE(states), buf, sizeof(buf), 1);

// Encode: {"temp": 2350, "hum": 65}
zcbor_map_start_encode(states, 2);
zcbor_tstr_put_lit(states, "temp");
zcbor_int32_put(states, 2350);       // 23.50 °C * 100
zcbor_tstr_put_lit(states, "hum");
zcbor_int32_put(states, 65);
zcbor_map_end_encode(states, 2);

size_t len = states[0].payload - buf;
// buf[0..len-1] contains the CBOR frame
```

### Sending over USB
```c
for (size_t i = 0; i < len; i++) {
    uart_poll_out(usb_uart, buf[i]);
}
uart_poll_out(usb_uart, '\n');  // optional newline as frame delimiter
```

### Framing strategy
Raw CBOR is binary — a newline delimiter works for simple cases but is not reliable if the data contains `0x0A`. For robust framing, prefix each packet with a 2-byte length:
```c
uint8_t header[2] = { (len >> 8) & 0xFF, len & 0xFF };
uart_poll_out(usb_uart, header[0]);
uart_poll_out(usb_uart, header[1]);
for (size_t i = 0; i < len; i++) {
    uart_poll_out(usb_uart, buf[i]);
}
```

### Decoding on the PC (Python)
```python
import serial, cbor2

port = serial.Serial('/dev/ttyACM0', timeout=1)
raw = port.read(64)          # read up to 64 bytes
data = cbor2.loads(raw)      # {'temp': 2350, 'hum': 65}
print(data)
```
Install: `pip install pyserial cbor2`

### zcbor types reference
| Type | Encode call |
|------|-------------|
| Integer | `zcbor_int32_put(state, val)` |
| Text string | `zcbor_tstr_put_lit(state, "key")` |
| Byte string | `zcbor_bstr_put_arr(state, buf, len)` |
| Boolean | `zcbor_bool_put(state, true)` |
| Array start | `zcbor_list_start_encode(state, max)` |
| Map start | `zcbor_map_start_encode(state, max)` |

## Why this matters
CBOR is the encoding of choice for CoAP, SenML, SUIT firmware updates, and many IoT protocols. Learning it now prepares you for Bluetooth, LwM2M, and cloud integrations later.

## Practice tasks
1. Encode a map with three fields: `temp`, `hum`, and a `ts` (timestamp as uint32).
2. Send it over USB every second and confirm receipt with the Python snippet above.
3. Extend to an array of 5 historical readings inside the same CBOR packet.

## Example folder
View the complete source code on GitHub: [src/2026-05-27_CBOR_over_USB](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-27_CBOR_over_USB)

## Next topic
Tomorrow we explore the Zephyr Shell subsystem to build an interactive command-line interface over UART or USB.

---


# Day 13 — Zephyr Shell Subsystem

## Goal
Use Zephyr's built-in Shell subsystem to add an interactive command-line interface to your firmware, accessible over UART or USB.

## What you will learn
- How to enable and configure the Zephyr Shell
- How to define custom shell commands with arguments
- How to nest commands into subcommand trees
- How to use the shell over USB CDC-ACM

## Overview
The Zephyr Shell gives you a fully featured CLI — command history, tab completion, help text — with minimal effort. It runs as a dedicated kernel thread and dispatches commands to your registered handlers.

### prj.conf settings
```kconfig
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_UART=y
CONFIG_UART_CONSOLE=y
```

To use the shell over USB instead:
```kconfig
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_CDC_ACM=y
CONFIG_UART_LINE_CTRL=y
```

### Registering a top-level command
```c
#include <zephyr/shell/shell.h>

static int cmd_hello(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "Hello from shell!");
    return 0;
}

SHELL_CMD_REGISTER(hello, NULL, "Print a greeting", cmd_hello);
```

### Subcommands
```c
SHELL_STATIC_SUBCMD_SET_CREATE(led_cmds,
    SHELL_CMD(on,  NULL, "Turn LED on",  cmd_led_on),
    SHELL_CMD(off, NULL, "Turn LED off", cmd_led_off),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(led, &led_cmds, "LED control commands", NULL);
```
Usage: `led on`, `led off`

### Reading arguments
```c
static int cmd_set(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_error(sh, "Usage: set <value>");
        return -EINVAL;
    }
    int val = atoi(argv[1]);
    shell_print(sh, "Value set to %d", val);
    return 0;
}
```

### Printing from outside the shell
Use `shell_fprintf` with a saved shell pointer, or use `printk` — both reach the console.

### Built-in shell commands
Zephyr's shell comes with built-in commands you get for free:
- `help` — list all registered commands
- `kernel uptime` — system uptime
- `kernel reboot cold` — restart the device
- `log` — control log levels at runtime

## Why this matters
A shell interface is invaluable for calibration, diagnostics, and field testing. It lets you interact with a running device without rebuilding firmware.

## Practice tasks
1. Register a `ping` command that replies `pong`.
2. Add a `led` command with `on` and `off` subcommands that control `led0`.
3. Add a `read temp` subcommand that prints a simulated temperature value.

## Example folder
View the complete source code on GitHub: [src/2026-05-28_Zephyr_Shell](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-28_Zephyr_Shell)

## Next topic
Tomorrow we read data from a real I2C sensor and log it using everything learned so far.

---


# Day 14 — I2C Sensor Integration

## Goal
Read data from a real I2C sensor using Zephyr's sensor API, combining Device Tree configuration, driver binding, and the logging skills from previous days.

## What you will learn
- How to declare an I2C sensor in the Device Tree overlay
- How to fetch and use a sensor device via `DEVICE_DT_GET`
- How to read channels using `sensor_sample_fetch` and `sensor_channel_get`
- How to use the BME280 (temperature/humidity/pressure) as a worked example

## Overview
Zephyr provides a unified sensor API across all supported drivers. Once the sensor is declared in the Device Tree and the correct Kconfig driver is enabled, the application code is the same regardless of the specific chip.

### prj.conf
```kconfig
CONFIG_I2C=y
CONFIG_SENSOR=y
CONFIG_BME280=y        # replace with your sensor's Kconfig symbol
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
```

### app.overlay — add sensor to I2C bus
```dts
&i2c0 {
    bme280@76 {
        compatible = "bosch,bme280";
        reg = <0x76>;           /* I2C address — check your sensor datasheet */
        label = "BME280";
    };
};
```
The `@76` suffix and `reg` value must match the sensor's 7-bit I2C address.

### Reading sensor data
```c
#include <zephyr/drivers/sensor.h>

const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme280);

void read_sensor(void)
{
    struct sensor_value temp, press, hum;

    int ret = sensor_sample_fetch(bme);
    if (ret < 0) {
        LOG_ERR("Sensor fetch failed: %d", ret);
        return;
    }
    sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    sensor_channel_get(bme, SENSOR_CHAN_PRESS,        &press);
    sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY,     &hum);

    printk("Temp: %d.%02d C  Press: %d.%02d kPa  Hum: %d.%02d %%\n",
           temp.val1,  abs(temp.val2)  / 10000,
           press.val1, abs(press.val2) / 10000,
           hum.val1,   abs(hum.val2)   / 10000);
}
```

### sensor_value explained
`sensor_value` is a fixed-point struct:
- `val1` — integer part
- `val2` — fractional part in millionths (µ)

So `{23, 500000}` = 23.5

### Common sensor Kconfig symbols
| Sensor | Symbol |
|--------|--------|
| BME280 temp/hum/press | `CONFIG_BME280=y` |
| SHT3xD temp/hum | `CONFIG_SHT3XD=y` |
| LIS2DH accelerometer | `CONFIG_LIS2DH=y` |
| TMP116 temperature | `CONFIG_TMP116=y` |
| ICM-42688 IMU | `CONFIG_ICM42688P=y` |

### I2C bus readiness check
Always verify the bus and device are ready before reading:
```c
if (!device_is_ready(bme)) {
    LOG_ERR("BME280 not ready — check wiring and I2C address");
    return;
}
```

## Why this matters
Almost every IoT product reads from sensors. The Zephyr sensor API means you can swap sensor chips by changing the overlay and Kconfig, with zero changes to your application logic.

## Practice tasks
1. Wire a BME280 to I2C pins on the nRF52840 DK (P0.26 = SDA, P0.27 = SCL).
2. Build and flash; confirm temperature readings in the log.
3. Combine with Day 12: encode the sensor reading as CBOR and send it over USB.

## Example folder
View the complete source code on GitHub: [src/2026-05-29_I2C_Sensor_Integration](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-29_I2C_Sensor_Integration)

## Next topic
You now have the core building blocks. Next steps include BLE, power management, firmware updates via SUIT/MCUboot, and production hardening.

---


# Day 15 — KXTJ3-1057 Accelerometer with USB Shell

## Goal
Implement a custom I2C driver for the KXTJ3-1057 3-axis accelerometer and expose it through an interactive Zephyr Shell over USB CDC-ACM.

## What you will learn
- How to write a minimal custom I2C driver when no Zephyr driver exists
- How to read raw accelerometer data and convert it to g-force
- How to combine USB CDC-ACM and the Zephyr Shell in one application
- How to build shell commands that read and configure hardware

## KXTJ3-1057 Overview
The KXTJ3-1057 (Kionix) is a low-power 3-axis MEMS accelerometer:
- Interface: I2C (up to 400 kHz)
- I2C address: **0x0E** (ADDR/SDO pin low) or **0x0F** (ADDR/SDO pin high)
- Measurement ranges: ±2g, ±4g, ±8g, ±16g
- Output: 12-bit or 8-bit resolution
- WHO_AM_I register: `0x0F` → value `0x35`

### Key registers
| Register | Address | Description |
|----------|---------|-------------|
| XOUT_L   | 0x06    | X-axis low byte |
| XOUT_H   | 0x07    | X-axis high byte |
| YOUT_L   | 0x08    | Y-axis low byte |
| YOUT_H   | 0x09    | Y-axis high byte |
| ZOUT_L   | 0x0A    | Z-axis low byte |
| ZOUT_H   | 0x0B    | Z-axis high byte |
| WHO_AM_I | 0x0F    | Device ID (0x35) |
| CTRL_REG1| 0x1B    | Power/range/resolution control |

### CTRL_REG1 bit map
```
Bit 7: PC1   — 1 = operating mode, 0 = standby
Bit 6: RES   — 1 = 12-bit, 0 = 8-bit
Bit 4: GSEL1 — range select MSB
Bit 3: GSEL0 — range select LSB
  00 = ±2g, 01 = ±4g, 10 = ±8g (14-bit), 11 = ±8g
```

## Hardware wiring (nRF52840 DK)
| KXTJ3-1057 pin | nRF52840 DK pin |
|----------------|-----------------|
| VDD            | 3.3V            |
| GND            | GND             |
| SDA            | P0.26           |
| SCL            | P0.27           |
| ADDR           | GND (→ 0x0E)    |

## Project structure
```
src/2026-05-30_KXTJ3_Accel_USB_Shell/
├── CMakeLists.txt
├── prj.conf
├── app.overlay
├── drivers/
│   └── kxtj3/
│       ├── kxtj3.h        ← register map and API
│       └── kxtj3.c        ← I2C read/write implementation
└── src/
    └── main.c             ← USB init + shell commands
```

## prj.conf
```kconfig
CONFIG_I2C=y
CONFIG_USB_DEVICE_STACK=y
CONFIG_USB_CDC_ACM=y
CONFIG_UART_LINE_CTRL=y
CONFIG_SHELL=y
CONFIG_SHELL_BACKEND_SERIAL=y
CONFIG_LOG=y
CONFIG_LOG_DEFAULT_LEVEL=3
```

## Shell commands implemented
| Command | Description |
|---------|-------------|
| `accel whoami` | Read WHO_AM_I register (expect 0x35) |
| `accel read` | Print X, Y, Z in mg |
| `accel stream <n>` | Print n readings at 200 ms intervals |
| `accel range <2\|4\|8\|16>` | Set measurement range |

## Converting raw counts to mg
The 12-bit result is left-justified in a 16-bit register pair.
Shift right by 4 to get the signed 12-bit value, then scale:
```c
int16_t raw = (int16_t)((high << 8) | low) >> 4;
int32_t mg  = (raw * 1000) / counts_per_g;
// counts_per_g = 1024 for ±2g, 512 for ±4g, 256 for ±8g
```

## Why this matters
Many sensors used in production products are not supported by Zephyr's upstream drivers. Writing a thin I2C wrapper is a core embedded skill. The shell interface lets you validate sensor output during bring-up without rebuilding firmware.

## Practice tasks
1. Verify WHO_AM_I returns `0x35` before reading data.
2. Hold the board still and confirm Z reads close to 1000 mg (1g).
3. Try `accel range 4` and confirm the readings change scale.
4. Extend `accel stream` to also encode data as CBOR (combine with Day 12).

## Example folder
View the complete source code on GitHub: [src/2026-05-30_KXTJ3_Accel_USB_Shell](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-30_KXTJ3_Accel_USB_Shell)

## Next topic
Future topics: BLE peripheral advertising, MCUboot firmware updates, low-power sleep modes.

---


# Day 16 — KXTJ3-1057 Accelerometer with CBOR over USB

## Goal
Extend the KXTJ3-1057 driver from Day 15 to encode accelerometer readings as CBOR
and transmit them over USB CDC-ACM. Add shell commands to control the output format
and a Python script to receive, decode, and display the data on a PC.

## What you will learn
- How to combine zcbor encoding with real sensor data
- How to design a simple binary protocol using CBOR maps
- How to trigger CBOR output from an interactive shell
- How to write a Python receiver to decode and display the sensor stream

## CBOR frame format
Each transmission is a self-contained CBOR map:
```
{
  "seq":  <uint32>,   sequence number
  "x":    <int32>,    X-axis in mg
  "y":    <int32>,    Y-axis in mg
  "z":    <int32>,    Z-axis in mg
  "rng":  <uint8>     current range in g (2, 4, 8, or 16)
}
```

The frame is followed by a newline (`0x0A`) as a simple delimiter so the Python
receiver can use `readline()` to receive complete packets.

### Encoded size
A typical frame encodes to ~25–30 bytes — well within a single USB packet.

## Shell commands
| Command | Description |
|---------|-------------|
| `accel whoami` | Verify WHO_AM_I register |
| `accel read` | Print one reading as plain text |
| `accel cbor` | Send one CBOR frame over USB |
| `accel stream <n>` | Send n CBOR frames at 200 ms intervals |
| `accel range <g>` | Set measurement range (2/4/8/16) |

## Python decoder (`tools/decode_accel.py`)
The script connects to the USB serial port, reads CBOR frames, and prints
a formatted table. Optional `--plot` flag renders a live scrolling graph
using matplotlib.

```bash
# Install dependencies
pip install pyserial cbor2 matplotlib

# Basic usage
python3 tools/decode_accel.py --port /dev/ttyACM0

# With live plot
python3 tools/decode_accel.py --port /dev/ttyACM0 --plot
```

### Example output
```
SEQ    X (mg)   Y (mg)   Z (mg)   Range
----  --------  -------  -------  -----
   0      -12       34      998    ±2g
   1      -13       33      997    ±2g
   2      -11       35      999    ±2g
```

## Why this matters
Sending structured binary data (CBOR) instead of plain text strings is essential
for production firmware — it is compact, self-describing, and easy to parse on
any platform. This pattern is used in CoAP, SenML, LwM2M, and Bluetooth GATT
descriptors.

## Practice tasks
1. Flash and run `accel cbor` — confirm the Python script decodes correctly.
2. Run `accel stream 50` and observe the live plot.
3. Shake the board and confirm all three axes respond.
4. Switch to `accel range 4` mid-stream and confirm the mg values change scale.

## Example folder
View the complete source code on GitHub: [src/2026-05-31_KXTJ3_CBOR_USB](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-05-31_KXTJ3_CBOR_USB)

## Next topic
Future topics: BLE peripheral advertising sensor data, MCUboot OTA updates,
low-power sleep with sensor wake-on-motion.

---


# Day 17 — BLE GATT Services

## Goal
Expose sensor data over Bluetooth Low Energy using a custom GATT service with
readable and notifiable characteristics, so a phone can subscribe to live updates.

## What you will learn
- The difference between advertising (Day 18) and GATT services
- How to define a custom GATT service and characteristics in Zephyr
- How to send notifications when data changes
- How to read a characteristic value from nRF Connect on a phone

## Overview
GATT (Generic Attribute Profile) is the data layer of BLE. A **service** groups
related **characteristics**. Each characteristic has a value, permissions (read/write/notify),
and optional descriptors. Zephyr's `BT_GATT_SERVICE_DEFINE` macro registers a
static service at compile time.

### Custom service UUIDs
Generate your own 128-bit UUIDs (use uuidgenerator.net):
```c
/* Service UUID */
#define BT_UUID_SENSOR_VAL     BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)
#define BT_UUID_SENSOR      BT_UUID_DECLARE_128(BT_UUID_SENSOR_VAL)

/* Accelerometer characteristic UUID */
#define BT_UUID_ACCEL_VAL     BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1)
#define BT_UUID_ACCEL       BT_UUID_DECLARE_128(BT_UUID_ACCEL_VAL)
```

### Defining the GATT service
```c
#include <zephyr/bluetooth/gatt.h>

static int16_t accel_data[3] = {0, 0, 1000}; /* X, Y, Z in mg */

/* Read handler — called when central reads the characteristic */
static ssize_t read_accel(struct bt_conn *conn,
                          const struct bt_gatt_attr *attr,
                          void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             accel_data, sizeof(accel_data));
}

BT_GATT_SERVICE_DEFINE(sensor_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_SENSOR),
    BT_GATT_CHARACTERISTIC(BT_UUID_ACCEL,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ,
        read_accel, NULL, accel_data),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);
```

### Sending notifications
```c
void notify_accel(int16_t x, int16_t y, int16_t z)
{
    accel_data[0] = x;
    accel_data[1] = y;
    accel_data[2] = z;

    /* Notify all connected centrals that have enabled notifications */
    bt_gatt_notify(NULL, &sensor_svc.attrs[1], accel_data, sizeof(accel_data));
}
```

### prj.conf
```kconfig
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="ZephyrSensor"
CONFIG_BT_GATT_DYNAMIC_DB=n
CONFIG_LOG=y
```

### Full application flow
```c
void main(void)
{
    int ret = bt_enable(NULL);
    if (ret < 0) {
        LOG_ERR("BT enable failed: %d", ret);
        return;
    }

    /* Start advertising so centrals can find and connect */
    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));

    while (1) {
        /* Update sensor data and notify connected clients */
        notify_accel(read_x(), read_y(), read_z());
        k_msleep(200);
    }
}
```

### Reading from a phone
1. Open **nRF Connect** (Android/iOS) → scan → connect to "ZephyrSensor"
2. Expand the **Unknown Service** (your custom UUID)
3. Tap the **read** button on the characteristic → see raw bytes
4. Tap the **notify** button (bell icon) → live updates appear as data changes

### Decoding the characteristic value
The value is 6 bytes: three `int16_t` values in little-endian order.
In Python (over USB or file):
```python
import struct
raw = bytes.fromhex("F4FF2200E803")   # example
x, y, z = struct.unpack('<hhh', raw)
print(f"X={x} Y={y} Z={z} mg")       # X=-12 Y=34 Z=1000 mg
```

## Why this matters
GATT services are the foundation of every BLE product — heart rate monitors,
fitness trackers, environmental sensors, and industrial IoT all use this pattern.
Once you define a service, any BLE central (phone, PC, gateway) can read or
subscribe to your data without a custom app.

## Practice tasks
1. Flash and connect with nRF Connect — confirm the service appears and read returns data.
2. Enable notifications and shake the board — confirm live X/Y/Z updates on the phone.
3. Add a second characteristic for the measurement range (single byte, read-only).
4. Add a **write** characteristic that lets the phone set the accelerometer range.

## Example folder
View the complete source code on GitHub: [src/2026-06-01_BLE_GATT_Service](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-01_BLE_GATT_Service)

## Next topic
Tomorrow we cover BLE peripheral advertising in depth and learn how to handle
connect/disconnect events gracefully.

---

# Day 18 — BLE Peripheral Advertising

## Goal
Set up the nRF52840 as a Bluetooth Low Energy (BLE) peripheral that advertises
a device name and a custom manufacturer data payload, connectable by a phone or PC.

## What you will learn
- How to enable and initialize the Zephyr BLE stack
- How to construct advertising and scan response data
- How to start and stop advertising
- How to handle connect and disconnect events
- How to read the device address and advertised name with a phone

## Overview
BLE advertising is the mechanism by which a peripheral announces its presence.
The nRF52840 has a built-in radio that Zephyr drives through the `bt_` API.
No external hardware is required — just the SoC and a power supply.

### prj.conf
```kconfig
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="MyZephyrDevice"
CONFIG_BT_DEVICE_APPEARANCE=0
CONFIG_LOG=y
```

### Advertising data structure
```c
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* Manufacturer-specific data in scan response */
static const uint8_t manuf_data[] = { 0xFF, 0x07, 0x01, 0x02, 0x03 };
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, manuf_data, sizeof(manuf_data)),
};
```

### Starting advertising
```c
bt_enable(NULL);   /* initialise the BLE stack */

bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
```

`BT_LE_ADV_CONN` = connectable undirected advertising at 100 ms interval.

### Connection callbacks
```c
static void connected(struct bt_conn *conn, uint8_t err) {
    printk("Connected\n");
}
static void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("Disconnected (reason %u)\n", reason);
    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected    = connected,
    .disconnected = disconnected,
};
```

### Verifying on a phone
- **Android**: nRF Connect app → Scanner tab → find "MyZephyrDevice"
- **iOS**: LightBlue app → Peripherals tab → find "MyZephyrDevice"
- **Linux**: `bluetoothctl scan on` → look for the device name

## Why this matters
BLE advertising is the entry point to all BLE applications — sensor beacons,
wearables, and IoT devices all start here. Once advertising works, you can
add GATT services to expose data to connected clients.

## Practice tasks
1. Flash and scan with nRF Connect — confirm the device name appears.
2. Change `CONFIG_BT_DEVICE_NAME` and rebuild — confirm the new name.
3. Add a second byte to `manuf_data` containing a counter that increments
   each time the device boots (combine with NVS from Day 22).

## Example folder
View the complete source code on GitHub: [src/2026-06-02_BLE_Peripheral_Advertising](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-02_BLE_Peripheral_Advertising)

## Next topic
Tomorrow we look at SPI communication to interface with external peripherals.

---


# Day 19 — SPI Communication

## Goal
Use Zephyr's SPI API to communicate with an external SPI device — reading and
writing registers over a full-duplex bus, with a practical example using a
W25Q flash chip (widely available and cheap).

## What you will learn
- How to configure an SPI bus and chip-select in the Device Tree
- How to perform SPI read and write transfers using `spi_transceive`
- How to read a JEDEC ID from an SPI flash as a "hello world" for SPI
- How to write and read back a page of data

## Overview
SPI is a synchronous, full-duplex 4-wire bus: MOSI, MISO, SCK, and CS.
Zephyr's `spi_transceive` API sends and receives simultaneously.
The Device Tree declares which SPI controller and CS pin to use.

### app.overlay
```dts
&spi1 {
    status = "okay";
    cs-gpios = <&gpio0 17 GPIO_ACTIVE_LOW>;

    w25q: w25q@0 {
        compatible = "jedec,spi-nor";
        reg = <0>;
        spi-max-frequency = <8000000>;
        size = <0x800000>;   /* 8 Mbit */
        has-dpd;
        t-enter-dpd = <3000>;
        t-exit-dpd = <30000>;
    };
};
```

### prj.conf
```kconfig
CONFIG_SPI=y
CONFIG_FLASH=y
CONFIG_FLASH_MAP=y
CONFIG_LOG=y
```

### Reading the JEDEC ID (raw SPI transaction)
```c
#include <zephyr/drivers/spi.h>

static const struct device *spi = DEVICE_DT_GET(DT_NODELABEL(spi1));

static const struct spi_config cfg = {
    .frequency = 8000000,
    .operation = SPI_OP_MODE_MASTER | SPI_WORD_SET(8),
    .cs = SPI_CS_CONTROL_INIT(DT_NODELABEL(w25q), 2),
};

void read_jedec_id(void)
{
    uint8_t tx_buf[] = { 0x9F };   /* JEDEC Read ID command */
    uint8_t rx_buf[4] = { 0 };

    struct spi_buf tx[] = {{ .buf = tx_buf, .len = sizeof(tx_buf) }};
    struct spi_buf rx[] = {{ .buf = rx_buf, .len = sizeof(rx_buf) }};
    struct spi_buf_set tx_set = { .buffers = tx, .count = 1 };
    struct spi_buf_set rx_set = { .buffers = rx, .count = 1 };

    spi_transceive(spi, &cfg, &tx_set, &rx_set);
    /* rx_buf[1..3] = Manufacturer, Memory Type, Capacity */
    printk("JEDEC ID: %02X %02X %02X\n", rx_buf[1], rx_buf[2], rx_buf[3]);
}
```

### Using the Zephyr Flash API (higher level)
```c
#include <zephyr/drivers/flash.h>

const struct device *flash_dev = DEVICE_DT_GET(DT_NODELABEL(w25q));
uint8_t data[256];

flash_read(flash_dev, 0, data, sizeof(data));
flash_erase(flash_dev, 0, 4096);              /* erase one sector */
flash_write(flash_dev, 0, data, sizeof(data));
```

### nRF52840 DK SPI wiring (W25Q flash module)
| Signal | nRF52840 DK pin |
|--------|----------------|
| MOSI   | P0.20          |
| MISO   | P0.21          |
| SCK    | P0.19          |
| CS     | P0.17          |
| VCC    | 3.3V           |
| GND    | GND            |

## Why this matters
SPI is used for displays, flash memory, sensors (IMUs, barometers), DACs, and
ADCs. The same `spi_transceive` API works for all of them.

## Practice tasks
1. Wire a W25Q16 and confirm the JEDEC ID reads `EF 40 15`.
2. Erase sector 0, write your name as ASCII, read it back.
3. Try increasing the SPI clock frequency and confirm communication stays stable.

## Example folder
View the complete source code on GitHub: [src/2026-06-03_SPI_Communication](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-03_SPI_Communication)

## Next topic
Tomorrow we read analog signals using the nRF52840's built-in ADC.

---


# Day 20 — ADC Reading

## Goal
Use the nRF52840's built-in 12-bit SAR ADC (SAADC) to read an analog voltage,
convert it to millivolts, and log the result periodically.

## What you will learn
- How to declare an ADC channel in the Device Tree
- How to configure gain, reference, and acquisition time
- How to perform a single-shot ADC reading with `adc_read`
- How to convert raw counts to millivolts using `adc_raw_to_millivolts`
- Practical uses: battery monitoring, potentiometer, analog sensor

## Overview
The nRF52840 SAADC has 8 channels, 8/10/12-bit resolution, configurable
gain and reference. Zephyr abstracts this through the `adc_` API.

### app.overlay
```dts
/ {
    zephyr,user {
        /* AIN0 maps to P0.02 on nRF52840 */
        io-channels = <&adc 0>;
    };
};

&adc {
    status = "okay";
    #address-cells = <1>;
    #size-cells = <0>;

    channel@0 {
        reg = <0>;
        zephyr,gain           = "ADC_GAIN_1_6";
        zephyr,reference      = "ADC_REF_INTERNAL";
        zephyr,acquisition-time = <ADC_ACQ_TIME_DEFAULT>;
        zephyr,input-positive = <NRF_SAADC_AIN0>;   /* P0.02 */
    };
};
```

### prj.conf
```kconfig
CONFIG_ADC=y
CONFIG_LOG=y
```

### Reading the ADC
```c
#include <zephyr/drivers/adc.h>

#define ADC_NODE    DT_PATH(zephyr_user)
#define ADC_CHANNEL DT_IO_CHANNELS_INPUT_BY_IDX(ADC_NODE, 0)

static const struct adc_dt_spec adc_ch = ADC_DT_SPEC_GET(ADC_NODE);

void read_adc(void)
{
    int16_t raw;
    struct adc_sequence seq = {
        .buffer      = &raw,
        .buffer_size = sizeof(raw),
    };

    adc_sequence_init_dt(&adc_ch, &seq);
    adc_read(adc_ch.dev, &seq);

    int32_t mv = raw;
    adc_raw_to_millivolts_dt(&adc_ch, &mv);
    printk("ADC: raw=%d  voltage=%d mV\n", raw, mv);
}
```

### nRF52840 SAADC AIN mapping
| AIN  | GPIO  | Notes |
|------|-------|-------|
| AIN0 | P0.02 | Also used for UART RTS on DK — use with care |
| AIN1 | P0.03 | Also used for UART CTS |
| AIN2 | P0.04 | Free on most DK configs |
| AIN3 | P0.05 | Free on most DK configs |
| AIN4 | P0.28 | |
| AIN5 | P0.29 | |
| AIN6 | P0.30 | |
| AIN7 | P0.31 | |

### Measuring battery voltage (resistor divider)
Wire a 10 MΩ / 4.7 MΩ divider from VBAT to AIN, GND.
Scale factor = (10 + 4.7) / 4.7 ≈ 3.13.
```c
int32_t vbat_mv = mv * 147 / 47;  /* fixed-point scale */
```

## Why this matters
ADC is essential for battery monitoring, analog sensors (light, temperature via
NTC, pressure), and potentiometer-based UIs. Understanding gain and reference
settings prevents measurement errors.

## Practice tasks
1. Wire a 10 kΩ potentiometer (wiper to AIN2/P0.04) and confirm readings sweep
   0–3600 mV as you turn it.
2. Add a second ADC channel on AIN3 and log both simultaneously.
3. Average 8 consecutive readings to reduce noise.

## Example folder
View the complete source code on GitHub: [src/2026-06-04_ADC_Reading](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-04_ADC_Reading)

## Next topic
Tomorrow we use PWM to smoothly dim an LED and control servo position.

---


# Day 21 — PWM LED Dimming

## Goal
Use Zephyr's PWM API to smoothly dim and brighten an LED using a breathing
animation, and understand how to map PWM channels in the Device Tree.

## What you will learn
- How to declare a PWM channel in the Device Tree using `pwm-leds`
- How to set duty cycle with `pwm_set_dt`
- How to implement a smooth breathing effect
- How PWM period and pulse width relate to duty cycle and frequency

## Overview
PWM (Pulse Width Modulation) rapidly switches a pin on and off. The ratio of
on-time (pulse) to total period (period) is the duty cycle. At 1 kHz, the
eye and motor cannot follow individual pulses — they perceive an average.

### app.overlay
```dts
/ {
    pwmleds {
        compatible = "pwm-leds";
        pwm_led0: pwm_led_0 {
            pwms = <&pwm0 0 PWM_MSEC(20) PWM_POLARITY_NORMAL>;
            label = "PWM LED";
        };
    };

    aliases {
        pwm-led0 = &pwm_led0;
    };
};

&pwm0 {
    status = "okay";
    pinctrl-0 = <&pwm0_default>;
    pinctrl-names = "default";
};

&pinctrl {
    pwm0_default: pwm0_default {
        group1 {
            /* LED1 on nRF52840 DK = P0.13 */
            psels = <NRF_PSEL(PWM_OUT0, 0, 13)>;
        };
    };
};
```

### prj.conf
```kconfig
CONFIG_PWM=y
CONFIG_LOG=y
```

### Setting duty cycle
```c
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm_led =
    PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/* 50% duty cycle */
pwm_set_dt(&pwm_led, pwm_led.period, pwm_led.period / 2);

/* 0% — fully off */
pwm_set_dt(&pwm_led, pwm_led.period, 0);

/* 100% — fully on */
pwm_set_dt(&pwm_led, pwm_led.period, pwm_led.period);
```

### Breathing effect
```c
/* Ramp pulse from 0 to period in 50 steps, then back down */
uint32_t period = pwm_led.period;
int step = period / 50;

while (1) {
    for (uint32_t pulse = 0; pulse <= period; pulse += step) {
        pwm_set_dt(&pwm_led, period, pulse);
        k_msleep(20);
    }
    for (uint32_t pulse = period; pulse > 0; pulse -= step) {
        pwm_set_dt(&pwm_led, period, pulse);
        k_msleep(20);
    }
}
```

### Servo control
PWM is also used for hobby servos (50 Hz, 1–2 ms pulse):
```c
/* Neutral position: 1.5 ms pulse at 50 Hz (20 ms period) */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(1500));
/* Full left: 1 ms */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(1000));
/* Full right: 2 ms */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(2000));
```

## Why this matters
PWM is used everywhere: LED brightness control, motor speed, servo position,
buzzer tones, and switching power supplies. Zephyr's Device Tree mapping
means you change a pin number in the overlay, not in the application code.

## Practice tasks
1. Flash and observe the breathing LED on LED1 (P0.13).
2. Change the step size and sleep delay to speed up or slow down the effect.
3. Wire a buzzer to a PWM pin and play a 1 kHz tone.

## Example folder
View the complete source code on GitHub: [src/2026-06-05_PWM_LED_Dimming](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-05_PWM_LED_Dimming)

## Next topic
Tomorrow we store configuration data in internal flash using Zephyr's NVS.

---


# Day 22 — NVS Flash Storage

## Goal
Use Zephyr's NVS (Non-Volatile Storage) subsystem to persist configuration
and state data across power cycles in the nRF52840's internal flash.

## What you will learn
- What NVS is and how it differs from raw flash writes
- How to define an NVS partition in the flash map
- How to write, read, and delete key-value pairs with `nvs_write` / `nvs_read`
- A practical example: counting boots and storing a device config struct

## Overview
NVS stores arbitrary binary data as key-value pairs in a flash sector.
It handles wear leveling automatically — writes spread across the sector
so no single location wears out. Data survives resets and power loss.

### Flash partition (app.overlay)
```dts
/ {
    fstab {
        compatible = "zephyr,fstab";
    };
};

&flash0 {
    partitions {
        compatible = "fixed-partitions";
        #address-cells = <1>;
        #size-cells = <1>;

        boot_partition: partition@0 {
            label = "mcuboot";
            reg = <0x00000000 0x0000C000>;
        };
        slot0_partition: partition@c000 {
            label = "image-0";
            reg = <0x0000C000 0x00067000>;
        };
        storage_partition: partition@73000 {
            label = "storage";
            reg = <0x00073000 0x0000D000>;
        };
    };
};
```

### prj.conf
```kconfig
CONFIG_NVS=y
CONFIG_FLASH=y
CONFIG_FLASH_PAGE_LAYOUT=y
CONFIG_FLASH_MAP=y
CONFIG_LOG=y
```

### NVS key IDs (define your own)
```c
#define NVS_ID_BOOT_COUNT  1
#define NVS_ID_DEVICE_CFG  2
```

### Initialising the NVS filesystem
```c
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>

static struct nvs_fs fs;

void nvs_init_storage(void)
{
    const struct flash_area *fa;
    flash_area_open(FIXED_PARTITION_ID(storage_partition), &fa);

    fs.flash_device = flash_area_get_device(fa);
    fs.offset       = fa->fa_off;
    fs.sector_size  = 4096;
    fs.sector_count = 4;

    nvs_mount(&fs);
}
```

### Writing and reading a boot counter
```c
uint32_t boot_count = 0;

/* Read existing value (returns -ENOENT on first boot) */
nvs_read(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));
boot_count++;
nvs_write(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));
printk("Boot count: %u\n", boot_count);
```

### Storing a config struct
```c
struct device_cfg {
    uint8_t  range_g;
    uint16_t sample_rate_ms;
    char     name[16];
};

struct device_cfg cfg = { .range_g = 4, .sample_rate_ms = 200, .name = "sensor1" };
nvs_write(&fs, NVS_ID_DEVICE_CFG, &cfg, sizeof(cfg));

/* Later, on next boot */
struct device_cfg loaded = { 0 };
if (nvs_read(&fs, NVS_ID_DEVICE_CFG, &loaded, sizeof(loaded)) > 0) {
    printk("Loaded config: range=%dg rate=%dms name=%s\n",
           loaded.range_g, loaded.sample_rate_ms, loaded.name);
}
```

### Deleting an entry
```c
nvs_delete(&fs, NVS_ID_DEVICE_CFG);
```

## Why this matters
Any real product needs to remember settings across power cycles: calibration
values, user preferences, device identity, error logs, and usage counters.
NVS is the simplest way to do this in Zephyr without a file system.

## Practice tasks
1. Flash and open a serial terminal — confirm boot count increments each reset.
2. Store and reload a config struct; verify all fields survive a power cycle.
3. Combine with the Shell (Day 13): add `config save` and `config load` commands.

## Example folder
View the complete source code on GitHub: [src/2026-06-06_NVS_Flash_Storage](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-06_NVS_Flash_Storage)

## Next topic
Tomorrow we explore work queues and kernel timers for deferred and periodic tasks.

---


# Day 23 — Work Queues and Kernel Timers

## Goal
Learn how to schedule deferred and periodic work in Zephyr using kernel timers
and work queues, without blocking threads or polling in a tight loop.

## What you will learn
- How `k_timer` fires a callback at a set interval
- How `k_work` defers processing out of ISR / timer context
- How `k_work_delayable` schedules work after a delay
- How to create a dedicated work queue thread for heavy background tasks

## Overview
Timers and work queues are Zephyr's core tools for time-driven logic.
A timer fires in interrupt context — keep the callback short. Hand off
real work to a `k_work` item that runs in a thread.

### One-shot and periodic timers
```c
static void timer_cb(struct k_timer *timer) {
    /* runs in ISR context — keep it short */
    printk("tick\n");
}

K_TIMER_DEFINE(my_timer, timer_cb, NULL);

/* Start: fire once after 2 s */
k_timer_start(&my_timer, K_SECONDS(2), K_NO_WAIT);

/* Start: fire every 500 ms after an initial 1 s delay */
k_timer_start(&my_timer, K_SECONDS(1), K_MSEC(500));

/* Stop */
k_timer_stop(&my_timer);
```

### Submitting work from a timer
```c
static struct k_work my_work;

static void work_handler(struct k_work *work) {
    /* runs in system work queue thread — safe to call kernel APIs */
    LOG_INF("Work executed at uptime %lld ms", k_uptime_get());
}

static void timer_cb(struct k_timer *timer) {
    k_work_submit(&my_work);  /* safe to call from ISR */
}

K_TIMER_DEFINE(my_timer, timer_cb, NULL);

void main(void) {
    k_work_init(&my_work, work_handler);
    k_timer_start(&my_timer, K_NO_WAIT, K_SECONDS(1));
}
```

### Delayable work (self-rescheduling)
```c
static struct k_work_delayable dwork;

static void dwork_handler(struct k_work *work) {
    LOG_INF("Deferred work");
    /* reschedule itself */
    k_work_reschedule(&dwork, K_MSEC(500));
}

void main(void) {
    k_work_init_delayable(&dwork, dwork_handler);
    k_work_schedule(&dwork, K_NO_WAIT);
}
```

### Custom work queue (dedicated thread)
```c
K_THREAD_STACK_DEFINE(my_wq_stack, 1024);
static struct k_work_q my_wq;

k_work_queue_start(&my_wq, my_wq_stack,
                   K_THREAD_STACK_SIZEOF(my_wq_stack),
                   K_PRIO_PREEMPT(5), NULL);

/* Submit to your queue instead of the system queue */
k_work_submit_to_queue(&my_wq, &my_work);
```

Use a dedicated queue when work is heavy, slow, or must not block
the system work queue that other Zephyr subsystems rely on.

### Checking timer remaining time
```c
k_ticks_t remaining = k_timer_remaining_ticks(&my_timer);
int64_t   ms        = k_ticks_to_ms_floor64(remaining);
```

## Why this matters
Work queues are used throughout Zephyr internals — the BLE stack, USB stack,
and sensor trigger system all post work items. Understanding them lets you
build responsive, non-blocking firmware without complex thread management.

## Practice tasks
1. Create a 1 Hz timer that submits work to toggle `led0`.
2. Add a `k_work_delayable` that prints uptime every 3 s without a timer.
3. Create a dedicated work queue and move heavy logging there.

## Example folder
View the complete source code on GitHub: [src/2026-06-07_Work_Queues_and_Timers](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-07_Work_Queues_and_Timers)

## Next topic
Tomorrow we configure Zephyr's power management to put the nRF52840 into
low-power sleep between work items.

---


# Day 24 — Power Management and Sleep

## Goal
Configure Zephyr's power management subsystem to put the nRF52840 into
low-power sleep states between tasks, dramatically reducing current consumption.

## What you will learn
- Zephyr's power management states and how the PM subsystem works
- How to enable automatic idle sleep with `CONFIG_PM`
- How to use `k_sleep` and `k_msleep` to hint the kernel to sleep
- How to measure current draw with and without PM enabled
- How to use the nRF52840's System OFF mode for ultra-low power

## Overview
The nRF52840 has several power states:
| State | Current | Wake source |
|-------|---------|-------------|
| Active (CPU running) | ~4 mA | — |
| WFI (Wait For Interrupt) | ~1.5 mA | Any IRQ |
| System ON sleep (radio off) | ~2–4 µA | GPIO, timer, RTC |
| System OFF | ~0.4 µA | GPIO DETECT, RESETN |

Zephyr's PM subsystem selects the deepest safe sleep state automatically
whenever all threads are blocked or sleeping.

### prj.conf — enable automatic power management
```kconfig
CONFIG_PM=y
CONFIG_PM_DEVICE=y
CONFIG_SERIAL=n          # disable UART to allow deeper sleep
CONFIG_LOG=n             # logging keeps UART active
```

> **Note:** Disabling UART removes the console. Use RTT (Real-Time Transfer)
> for debug output in low-power builds:
> ```kconfig
> CONFIG_USE_SEGGER_RTT=y
> CONFIG_RTT_CONSOLE=y
> ```

### How automatic sleep works
When all Zephyr threads call `k_msleep()` or are blocked on semaphores,
the kernel calculates how long until the next wake event, then calls
`pm_state_force()` or lets the idle thread select a sleep state.
No application code change is needed — just enable `CONFIG_PM=y`.

### Entering System OFF (deepest sleep)
System OFF is a full power-down. The CPU does not resume — it resets on wake.
Use this for ultra-low power when long intervals (minutes/hours) are acceptable.

```c
#include <zephyr/pm/pm.h>
#include <hal/nrf_gpio.h>

void enter_system_off(void)
{
    /* Configure a GPIO pin as wake source */
    nrf_gpio_cfg_sense_input(NRF_GPIO_PIN_MAP(0, 11),
                             NRF_GPIO_PIN_PULLUP,
                             NRF_GPIO_PIN_SENSE_LOW);

    /* Enter System OFF — code here does not return */
    pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});
    k_sleep(K_FOREVER);
}
```

### Wake-on-motion pattern (with KXTJ3 INT pin)
```
1. Read + process accelerometer data
2. Configure KXTJ3 wake-on-motion interrupt
3. Enter System OFF sleep
4. KXTJ3 INT pin goes high → nRF52840 resets
5. main() runs again → read + process → sleep
```

### Measuring power savings
Use a Nordic PPK2 (Power Profiler Kit 2) or a µCurrent meter in series
with the VDD supply. Compare:
- No sleep: ~4 mA continuous
- `CONFIG_PM=y` with 1 s sleep: ~8 µA average
- System OFF with GPIO wake: ~0.5 µA average

### Keeping peripherals active during sleep
Some drivers support `PM_DEVICE` — they can be suspended/resumed
automatically. Enable with:
```kconfig
CONFIG_PM_DEVICE=y
CONFIG_PM_DEVICE_RUNTIME=y
```

## Why this matters
Battery life is a critical spec for wearables and IoT sensors. A coin cell
(CR2032, 220 mAh) lasts hours at 4 mA but over a year at 8 µA average.

## Practice tasks
1. Enable `CONFIG_PM=y` and measure current during `k_msleep(5000)`.
2. Compare against the same build without `CONFIG_PM`.
3. Implement System OFF with button (sw0) as wake source — confirm it wakes
   and boot-counts (NVS from Day 22) increments on each wake.

## Example folder
View the complete source code on GitHub: [src/2026-06-08_Power_Management_Sleep](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-08_Power_Management_Sleep)

## Next topic
You now have all the core Zephyr building blocks. The capstone project in
Days 25–27 combines everything into a complete wireless sensor node.

---

# Capstone Project — Days 25–27: Wireless Sensor Node

This three-day capstone combines BME280 environmental sensing, BLE GATT
notifications, NVS persistence, and System OFF sleep into a single
production-ready firmware application. By the end you will have a
battery-powered node that wakes every 60 seconds, reads temperature and
humidity, broadcasts the values over BLE, stores a boot count and last
reading in NVS, then returns to sub-µA sleep.

**Hardware required:**
- Nordic nRF52840 DK
- BME280 breakout (I2C, 3.3 V)
- Coin cell holder or LiPo (optional, for real power measurements)

---

## Day 25 — Sensor + NVS Integration

**Goal:** Read BME280 and persist a boot counter plus last reading to NVS.

### prj.conf

```kconfig
CONFIG_I2C=y
CONFIG_SENSOR=y
CONFIG_BME280=y
CONFIG_NVS=y
CONFIG_FLASH=y
CONFIG_FLASH_PAGE_LAYOUT=y
CONFIG_FLASH_MAP=y
CONFIG_LOG=y
CONFIG_MAIN_STACK_SIZE=2048
```

### main.c

```c
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(capstone, LOG_LEVEL_INF);

#define NVS_PARTITION        storage_partition
#define NVS_PARTITION_DEVICE FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET FIXED_PARTITION_OFFSET(NVS_PARTITION)

#define NVS_ID_BOOT_COUNT  1
#define NVS_ID_LAST_TEMP   2
#define NVS_ID_LAST_HUM    3

static struct nvs_fs fs;

static int nvs_init_fs(void)
{
    struct flash_pages_info info;
    int ret;

    fs.flash_device = NVS_PARTITION_DEVICE;
    fs.offset = NVS_PARTITION_OFFSET;
    ret = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
    if (ret < 0) {
        return ret;
    }
    fs.sector_size = info.size;
    fs.sector_count = 4U;
    return nvs_mount(&fs);
}

int main(void)
{
    const struct device *bme = DEVICE_DT_GET_ANY(bosch_bme280);
    struct sensor_value temp, hum;
    uint32_t boot_count = 0;
    int32_t last_temp_mdeg = 0, last_hum_m = 0;
    int ret;

    if (!device_is_ready(bme)) {
        LOG_ERR("BME280 not ready");
        return -ENODEV;
    }

    ret = nvs_init_fs();
    if (ret < 0) {
        LOG_ERR("NVS mount failed: %d", ret);
        return ret;
    }

    /* Read persistent boot count */
    nvs_read(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));
    nvs_read(&fs, NVS_ID_LAST_TEMP, &last_temp_mdeg, sizeof(last_temp_mdeg));
    nvs_read(&fs, NVS_ID_LAST_HUM,  &last_hum_m,    sizeof(last_hum_m));

    LOG_INF("Boot #%u, last T=%d.%03d C, H=%d.%03d %%",
            boot_count,
            last_temp_mdeg / 1000, abs(last_temp_mdeg % 1000),
            last_hum_m / 1000,     abs(last_hum_m % 1000));

    boot_count++;
    nvs_write(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));

    ret = sensor_sample_fetch(bme);
    if (ret < 0) {
        LOG_ERR("Fetch failed: %d", ret);
        return ret;
    }
    sensor_channel_get(bme, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    sensor_channel_get(bme, SENSOR_CHAN_HUMIDITY,     &hum);

    last_temp_mdeg = temp.val1 * 1000 + temp.val2 / 1000;
    last_hum_m     = hum.val1  * 1000 + hum.val2  / 1000;

    nvs_write(&fs, NVS_ID_LAST_TEMP, &last_temp_mdeg, sizeof(last_temp_mdeg));
    nvs_write(&fs, NVS_ID_LAST_HUM,  &last_hum_m,    sizeof(last_hum_m));

    LOG_INF("T=%.2f C  H=%.2f %%",
            sensor_value_to_double(&temp),
            sensor_value_to_double(&hum));

    return 0;
}
```

**What to observe:** Open a serial terminal. Each power cycle (or reset)
should increment the boot counter and log the previous reading.

---

## Day 26 — BLE GATT Environmental Service

**Goal:** Expose temperature and humidity as BLE GATT characteristics so a
phone can subscribe to notifications.

### prj.conf additions

Add to the Day 25 `prj.conf`:

```kconfig
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="SensorNode"
CONFIG_BT_GATT_DYNAMIC_DB=n
```

### ble_sensor.h

```c
#pragma once
#include <stdint.h>

void ble_sensor_init(void);
void ble_sensor_notify(int32_t temp_mdeg, int32_t hum_m);
```

### ble_sensor.c

```c
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>
#include "ble_sensor.h"

LOG_MODULE_REGISTER(ble_sensor, LOG_LEVEL_INF);

/* Custom 128-bit UUID for the sensor service */
#define BT_UUID_SENSOR_SERVICE_VAL \
    BT_UUID_128_ENCODE(0xaabbccdd, 0x1234, 0x5678, 0xabcd, 0x1234567890ab)
#define BT_UUID_TEMP_CHAR_VAL \
    BT_UUID_128_ENCODE(0xaabbccdd, 0x1234, 0x5678, 0xabcd, 0x1234567890ac)
#define BT_UUID_HUM_CHAR_VAL \
    BT_UUID_128_ENCODE(0xaabbccdd, 0x1234, 0x5678, 0xabcd, 0x1234567890ad)

#define BT_UUID_SENSOR_SERVICE BT_UUID_DECLARE_128(BT_UUID_SENSOR_SERVICE_VAL)
#define BT_UUID_TEMP_CHAR      BT_UUID_DECLARE_128(BT_UUID_TEMP_CHAR_VAL)
#define BT_UUID_HUM_CHAR       BT_UUID_DECLARE_128(BT_UUID_HUM_CHAR_VAL)

static int32_t temp_val;
static int32_t hum_val;

static ssize_t read_temp(struct bt_conn *conn,
                         const struct bt_gatt_attr *attr,
                         void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             &temp_val, sizeof(temp_val));
}

static ssize_t read_hum(struct bt_conn *conn,
                        const struct bt_gatt_attr *attr,
                        void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             &hum_val, sizeof(hum_val));
}

BT_GATT_SERVICE_DEFINE(sensor_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_SENSOR_SERVICE),
    BT_GATT_CHARACTERISTIC(BT_UUID_TEMP_CHAR,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ, read_temp, NULL, &temp_val),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
    BT_GATT_CHARACTERISTIC(BT_UUID_HUM_CHAR,
        BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
        BT_GATT_PERM_READ, read_hum, NULL, &hum_val),
    BT_GATT_CCC(NULL, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

void ble_sensor_init(void)
{
    int ret = bt_enable(NULL);
    if (ret < 0) {
        LOG_ERR("bt_enable: %d", ret);
        return;
    }
    ret = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (ret < 0) {
        LOG_ERR("adv_start: %d", ret);
    }
    LOG_INF("BLE advertising as \"%s\"", CONFIG_BT_DEVICE_NAME);
}

void ble_sensor_notify(int32_t temp_mdeg, int32_t hum_m)
{
    temp_val = temp_mdeg;
    hum_val  = hum_m;
    bt_gatt_notify(NULL, &sensor_svc.attrs[1], &temp_val, sizeof(temp_val));
    bt_gatt_notify(NULL, &sensor_svc.attrs[3], &hum_val,  sizeof(hum_val));
}
```

Call `ble_sensor_init()` and `ble_sensor_notify(last_temp_mdeg, last_hum_m)`
from `main.c` after reading the sensor.

**What to observe:** Use the nRF Connect app on your phone. Scan for
"SensorNode", connect, and subscribe to both characteristics. You should
see temperature (in milli-degrees C) and humidity (in milli-%) update on
each notification.

---

## Day 27 — Sleep and Wake-Up Loop

**Goal:** Add a 60-second sleep cycle. The node wakes, reads the sensor,
sends a BLE notification burst, then enters System OFF (or deep sleep).

### Strategy

For a real battery-powered product you want the shortest possible active
window. The pattern is:

1. Boot (from reset or GPIO wake)
2. Read NVS boot count, increment, write back
3. Read BME280
4. Initialize BLE, advertise, send notifications
5. Wait 2 seconds for a central to connect and read
6. Disable BLE
7. Enter System OFF — wake via RTC alarm or external GPIO

For the DK (no external RTC) we use `k_sleep` with `CONFIG_PM=y` to
demonstrate the current savings without requiring external hardware.

### Timed sleep version (DK-friendly)

```c
#include <zephyr/pm/pm.h>

/* At the end of main(), after BLE notify: */
LOG_INF("Sleeping 60 s...");
k_sleep(K_SECONDS(60));
/* Loop back: in a real product, trigger System OFF here */
```

### System OFF version (production pattern)

```c
#include <zephyr/pm/pm.h>

static void enter_system_off(void)
{
    LOG_INF("Entering System OFF");
    /* Flush log backend before power-off */
    k_sleep(K_MSEC(100));
    pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});
    /* Enable GPIO wakeup source (button sw0) */
    const struct device *gpio = DEVICE_DT_GET(DT_NODELABEL(gpio0));
    gpio_pin_interrupt_configure(gpio, 11, GPIO_INT_LEVEL_LOW);
    /* The next line does not return */
    k_sleep(K_FOREVER);
}
```

Call `enter_system_off()` at the end of `main()`.

### Measuring current

| State                  | Typical current  |
|------------------------|-----------------|
| Active + BLE TX        | 8–12 mA         |
| `k_sleep` with PM      | 5–15 µA         |
| System OFF             | 0.4–0.7 µA      |

Use a Nordic PPK2 or a bench supply with current measurement to verify.

### Practice tasks

1. Flash the full capstone firmware (Days 25–27 combined).
2. Confirm boot count increments on each reset.
3. Connect with nRF Connect — verify temperature and humidity notifications.
4. Enable `CONFIG_PM=y` and measure current during the 60-second sleep.
5. Switch to System OFF — measure the sub-µA quiescent current.
6. Add a third NVS key for "last wakeup reason" (reset vs GPIO).

## Example folder
View the complete source code on GitHub: [src/capstone_wireless_sensor_node](https://github.com/jovin555/My-Zephyr-project/tree/master/src/capstone_wireless_sensor_node)

---

# Appendix A — Troubleshooting

Common build and runtime errors with solutions.

## Build errors

### `Could NOT find Zephyr-sdk compatible with version 1.0`
**Cause:** SDK 0.16.x or 0.17.x is installed instead of SDK 1.0.1.
**Fix:**
```bash
ls ~/zephyr-sdk-*    # check what's installed
# Download SDK 1.0.1 from:
# https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v1.0.1
cd ~ && tar xf zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz
cd zephyr-sdk-1.0.1 && ./setup.sh
```

### `Could NOT find Python3 (found version "3.10")`
**Cause:** Zephyr 4.x requires Python 3.12.
**Fix:**
```bash
python3.12 -m venv .venv
source .venv/bin/activate
pip install west
```

### `west: command not found`
**Cause:** Virtual environment not activated.
**Fix:**
```bash
source /home/jovin/workspace/My-Zephyr-project/myworkspace/.venv/bin/activate
```

### `ZEPHYR_BASE is not set`
**Cause:** `ZEPHYR_BASE` environment variable missing.
**Fix:**
```bash
export ZEPHYR_BASE=/home/jovin/workspace/My-Zephyr-project/myworkspace/zephyr
```

### `DT_NODELABEL(...) node does not exist`
**Cause:** The device tree label doesn't match the board's `.dts` file.
**Fix:** Check `zephyr/boards/nordic/nrf52840dk/nrf52840dk_nrf52840.dts` for the
correct node label. Use `west build -- -DDTC_OVERLAY_FILE=app.overlay` to
add or override nodes in your app overlay.

### `undefined reference to 'bt_enable'`
**Cause:** `CONFIG_BT=y` not set in `prj.conf`.
**Fix:** Add the required Kconfig options for the feature you are using.
BLE minimum: `CONFIG_BT=y`, `CONFIG_BT_PERIPHERAL=y` (or `CONFIG_BT_CENTRAL=y`).

---

## Runtime / flashing errors

### `LIBUSB_ERROR_ACCESS` when running `west flash`
**Cause:** User lacks permissions to access the J-Link USB device.
**Fix:**
```bash
sudo usermod -aG plugdev $USER
# Log out and back in, or run:
newgrp plugdev
```

### Serial terminal shows garbage / no output
**Cause:** Wrong baud rate (Zephyr default is 115200) or wrong port.
**Fix:**
```bash
ls /dev/ttyACM*          # find the port
screen /dev/ttyACM0 115200
# or
minicom -D /dev/ttyACM0 -b 115200
```

### Application crashes with `*** USAGE FAULT ***`
**Cause:** Stack overflow, null pointer dereference, or misaligned access.
**Fix:** Enable stack sentinel in `prj.conf`:
```kconfig
CONFIG_STACK_SENTINEL=y
CONFIG_THREAD_ANALYZER=y
```
Run, check logs for "Stack sentinel violated" or per-thread stack usage.

### `sensor_sample_fetch` returns `-5` (EIO)
**Cause:** I2C address mismatch or SDA/SCL wiring error.
**Fix:**
1. Check the sensor's I2C address against the overlay (e.g., `reg = <0x0e>`).
2. Verify wiring: SDA → P0.26, SCL → P0.27 on the nRF52840 DK.
3. Use a logic analyser or `i2cdetect` (if shell enabled) to scan the bus.

### BLE not advertising (no device found in nRF Connect)
**Cause:** `bt_enable` or `bt_le_adv_start` returned an error, or
`CONFIG_BT_PERIPHERAL=y` is missing.
**Fix:** Check the log for `bt_enable:` or `adv_start:` error codes.
Common: `-22` (EINVAL) means the advertising parameters are invalid.

### NVS write returns `-28` (ENOSPC)
**Cause:** NVS partition is full or `sector_count` is too small.
**Fix:** Increase `sector_count` in your `nvs_fs` struct, or call
`nvs_clear(&fs)` to erase all entries and start fresh.

---

# Appendix B — Glossary

**ADC** — Analog-to-Digital Converter. Converts a voltage on a GPIO pin to
a digital value. Used in Day 20 to read battery voltage.

**BLE** — Bluetooth Low Energy. A short-range wireless protocol optimised
for battery-powered devices. Used in Days 17–18 and 26.

**CBOR** — Concise Binary Object Representation (RFC 7049). A binary
encoding format used in Day 12 and 16 to serialise sensor data over USB.

**CDC-ACM** — USB Communications Device Class – Abstract Control Model.
The USB class that makes the nRF52840 appear as a virtual serial port
(`/dev/ttyACM0`) on the host. Used in Days 11–13, 15–16.

**DTS / Device Tree Source** — A hardware description file (`.dts` /
`.overlay`) that tells Zephyr which peripherals exist, where they are
mapped, and how they are connected. Used throughout the course.

**GATT** — Generic Attribute Profile. The BLE protocol layer that defines
services and characteristics. Used in Days 17 and 26.

**GPIO** — General Purpose Input/Output. Digital pins that can be driven
high/low or read as input. Used in Days 5, 9, and throughout.

**I2C** — Inter-Integrated Circuit. A two-wire serial bus (SDA + SCL) used
to communicate with sensors. Used in Days 14–17 and 25–27.

**ISR** — Interrupt Service Routine. A function called directly by
hardware when an interrupt fires. In Zephyr, ISRs must not sleep or
allocate memory.

**Kconfig** — The configuration system inherited from Linux. Boolean and
integer options that enable/disable subsystems at build time. Configured
via `prj.conf`.

**NVS** — Non-Volatile Storage. A simple key-value store that uses a
dedicated flash partition to persist data across resets. Used in Days 22
and 25–27.

**PPK2** — Nordic Power Profiler Kit 2. A USB device for measuring current
consumption down to ~100 nA. Referenced in the power management days.

**PWM** — Pulse Width Modulation. A technique for controlling average
power to an output (e.g., LED brightness, motor speed) by varying the
duty cycle of a digital signal. Used in Day 21.

**SDK** — Software Development Kit. Here refers to the Zephyr SDK
(version 1.0.1) which bundles the ARM GCC toolchain, OpenOCD, and J-Link
tools used to compile and flash firmware.

**SPI** — Serial Peripheral Interface. A four-wire serial bus (MOSI, MISO,
SCK, CS) used for high-speed peripherals such as flash memory. Used in
Day 19 with the W25Q SPI flash.

**System OFF** — The deepest power state on the nRF52840. The CPU is fully
stopped and RAM is not retained. Current is typically 0.4–0.7 µA. Wake is
via GPIO or reset. Used in Day 24 and the capstone.

**West** — Zephyr's meta-build tool. Manages the workspace, fetches
modules with `west update`, and wraps CMake/Ninja with `west build` and
`west flash`.

**Work Queue** — A Zephyr kernel object that executes `k_work` items in a
dedicated thread. Used in Day 23 to defer processing from ISR context to
a thread.

---

