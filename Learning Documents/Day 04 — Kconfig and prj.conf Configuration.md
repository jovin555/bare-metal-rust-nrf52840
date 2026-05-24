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