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