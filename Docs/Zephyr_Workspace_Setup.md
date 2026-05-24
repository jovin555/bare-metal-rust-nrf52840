# Zephyr Workspace Setup Guide

Setup steps for the latest Zephyr inside `myworkspace/` using Python 3.12.

> **Note:** Python 3.12 is required. Zephyr's CMake modules call `find_package(Python3 3.12)` — using 3.9 or 3.10 will cause a CMake error at build time. Use the system `python3.12` (`/usr/bin/python3.12`).

---

## Step 1 — Create a virtual environment with west

```bash
cd /home/jovin/workspace/My-Zephyr-project/myworkspace
python3.12 -m venv .venv
source .venv/bin/activate
pip install west
```

---

## Step 2 — Initialize the Zephyr workspace

```bash
unset ZEPHYR_BASE   # clear any existing ZEPHYR_BASE before init
west init .
west update
```

> Downloads Zephyr kernel + all HAL/module dependencies (~1–2 GB, takes a few minutes).

---

## Step 3 — Install Zephyr Python requirements

```bash
pip install -r zephyr/scripts/requirements.txt
```

---

## Step 4 — Install the Zephyr SDK (ARM toolchain for nRF52840)

Check if the SDK is already installed first:
```bash
ls ~/zephyr-sdk-* 2>/dev/null && echo "SDK found" || echo "SDK not installed"
```

If not installed:
```bash
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/zephyr-sdk-1.0.1_linux-x86_64.tar.xz
tar xf zephyr-sdk-1.0.1_linux-x86_64.tar.xz
cd zephyr-sdk-1.0.1
./setup.sh
```

> **Note:** SDK 1.0.1 is required for Zephyr 4.x. SDK 0.16.8 and 0.17.x are incompatible — they cause a CMake "could not find Zephyr-sdk compatible with version 1.0" error.

---

## Step 5 — Build a project

Run these commands at the start of every new terminal session:

```bash
source /home/jovin/workspace/My-Zephyr-project/myworkspace/.venv/bin/activate
export ZEPHYR_BASE=/home/jovin/workspace/My-Zephyr-project/myworkspace/zephyr
```

Then navigate to any project and build:

```bash
cd /home/jovin/workspace/My-Zephyr-project/src/2026-05-16_Introduction_to_Zephyr_RTOS
west build -b nrf52840dk/nrf52840
```

Flash to the board:
```bash
west flash
```

---

## Quick reference — session setup (copy-paste)

```bash
source /home/jovin/workspace/My-Zephyr-project/myworkspace/.venv/bin/activate
export ZEPHYR_BASE=/home/jovin/workspace/My-Zephyr-project/myworkspace/zephyr
```

---

## Notes

- **Use `python3.12`** (`/usr/bin/python3.12`) — Zephyr's CMake requires Python ≥ 3.12. Using 3.9 or 3.10 causes a CMake `Could NOT find Python3` error at build time.
- `ZEPHYR_BASE` must **not** be set before running `west init` — unset it first if it points to `~/ncs/v2.4.2`.
- The `.venv` folder is inside `myworkspace/` which is excluded from git (`.gitignore`).
- If `west` is not found after activating the venv, re-run `pip install west` inside the activated environment.
- **SDK version `0.17.0`** is required for Zephyr 4.x. SDK 0.16.8 only supports Zephyr up to 3.x.
