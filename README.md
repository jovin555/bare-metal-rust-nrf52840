# Bare-Metal Rust on the nRF52840
### A 10-Day Hands-On Guide to Embedded Systems Without an OS

**Bare-Metal Rust on the nRF52840** is a hands-on guide for engineers who want to
understand what a microcontroller actually runs when there is no operating system —
no RTOS, no HAL abstractions hiding the reset vector, the linker script, or the
interrupt table.

Each day introduces one concept, one buildable example, and four practice tasks.
You start with the absolute minimum — a loop that does nothing — and finish with a
multi-task concurrent application using the RTIC framework.

**What's inside:**
- A compact Rust language primer (Part 0) for engineers coming from C or another systems language
- 10 structured lessons with working `#![no_std]` source code for every day
- Direct register access through the PAC, then the ergonomic HAL layer
- Logging and debugging over RTT with `defmt` — no UART or display required
- GPIO, interrupts (GPIOTE), UART, and hardware timers
- Compile-time data-race-free concurrency with RTIC

**Who this is for:** Embedded engineers with C experience who are new to Rust, and
Rust developers who are new to bare-metal / no-OS firmware.

**Hardware required:** Nordic nRF52840 DK (`PCA10056`) and a data-capable USB cable.

> **Target hardware:** Nordic nRF52840 DK (`nrf52840dk/nrf52840`)
> **Rust toolchain:** stable (`thumbv7em-none-eabihf`)
> **Key crates:** cortex-m-rt 0.7, nrf52840-hal 0.16, defmt 0.3, RTIC 1.1

---

## What you will build

By the end of this course you will have written firmware that:

- Blinks an LED via direct PAC register access, then via the HAL
- Uses SysTick for real, calibrated delays
- Logs over RTT with `defmt` at multiple severity levels
- Reads buttons and debounces GPIO input
- Handles GPIOTE pin-change interrupts safely
- Sends and receives data over UART
- Uses a hardware timer peripheral directly
- Runs button and timer tasks concurrently under RTIC, with compile-time-verified shared state

---

## Course outline

| | Topic | Folder |
|---|-------|--------|
| Part 0 | [Rust Language Foundations](manuscript/Rust_Book.md#part-zero) | [Learning_rust/content](Learning_rust/content/) |
| 1 | Toolchain Setup and First Flash | [rust_src/day01_toolchain_setup](rust_src/day01_toolchain_setup/) |
| 2 | Blinky with the PAC | [rust_src/day02_blinky_pac](rust_src/day02_blinky_pac/) |
| 3 | Blinky with the HAL | [rust_src/day03_blinky_hal](rust_src/day03_blinky_hal/) |
| 4 | SysTick and Real Delays | [rust_src/day04_systick_delays](rust_src/day04_systick_delays/) |
| 5 | Logging with defmt and RTT | [rust_src/day05_defmt_logging](rust_src/day05_defmt_logging/) |
| 6 | GPIO Input and Buttons | [rust_src/day06_gpio_input](rust_src/day06_gpio_input/) |
| 7 | GPIOTE Interrupts | [rust_src/day07_gpiote_interrupts](rust_src/day07_gpiote_interrupts/) |
| 8 | UART Serial Communication | [rust_src/day08_uart_serial](rust_src/day08_uart_serial/) |
| 9 | Hardware Timers | [rust_src/day09_hardware_timers](rust_src/day09_hardware_timers/) |
| 10 | RTIC: Safe Concurrency | [rust_src/day10_rtic_concurrency](rust_src/day10_rtic_concurrency/) |

| | Appendix A — Quick Reference | |
| | Appendix B — Glossary | |

---

## Prerequisites

- Nordic nRF52840 DK (or compatible nRF52840 board)
- Linux or macOS host
- USB cable (data, not charge-only)

## Quick start

```bash
# Rust toolchain
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
source ~/.cargo/env
rustup target add thumbv7em-none-eabihf

# probe-rs — flash and debug tool
cargo install probe-rs-tools --locked

# Build and flash Day 1
cd rust_src/day01_toolchain_setup
cargo run --release
```

`cargo run` triggers the `runner` defined in `.cargo/config.toml`, which calls
`probe-rs run` to flash and start the firmware. Logs (from Day 5 onward) stream
to your terminal automatically.

---

## Repository structure

```
.
├── manuscript/                  # The book itself (Markua source, Leanpub)
├── Learning_rust/content/       # Companion daily-log blog: Rust language foundations
└── rust_src/                    # One buildable Cargo project per day
    ├── day01_toolchain_setup/
    ├── ...
    └── day10_rtic_concurrency/
```

---

## License

Code: MIT
Book text: Creative Commons BY 4.0
