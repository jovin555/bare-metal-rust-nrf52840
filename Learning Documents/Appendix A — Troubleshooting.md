# Appendix A — Troubleshooting

Common build and runtime errors encountered when working with Zephyr 4.x on
the nRF52840 DK, with their causes and fixes.

---

## Build errors

### `Could NOT find Zephyr-sdk compatible with version 1.0`
**Cause:** SDK 0.16.x or 0.17.x is installed instead of SDK 1.0.1.
**Fix:**
```bash
ls ~/zephyr-sdk-*   # check installed versions
cd ~
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v1.0.1/zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz
tar xf zephyr-sdk-1.0.1_linux-x86_64_gnu.tar.xz
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
**Cause:** `ZEPHYR_BASE` environment variable missing from the current shell.
**Fix:**
```bash
export ZEPHYR_BASE=/home/jovin/workspace/My-Zephyr-project/myworkspace/zephyr
```

### `DT_NODELABEL(...) node does not exist`
**Cause:** The device tree label does not match the board's `.dts` file.
**Fix:** Check `zephyr/boards/nordic/nrf52840dk/nrf52840dk_nrf52840.dts` for the
correct node label. Use a `.overlay` file to add or override nodes in your app.

### `undefined reference to 'bt_enable'`
**Cause:** `CONFIG_BT=y` not set in `prj.conf`.
**Fix:** Add the required Kconfig for the feature being used.
BLE minimum: `CONFIG_BT=y` plus `CONFIG_BT_PERIPHERAL=y` or `CONFIG_BT_CENTRAL=y`.

---

### `*** BUS FAULT *** Imprecise data bus error` on every boot

**Cause:** Two issues that affect all projects on Zephyr 4.x:

1. `CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=-1` (the default) tells Zephyr to use
   all remaining RAM for the libc malloc heap. On Zephyr 4.4.x the "remaining
   RAM" calculation overshoots the 256 KB boundary by a few bytes, causing
   `sys_heap_init` to write its tail chunk past `0x20040000` — a bus fault.

2. `void main(void)` is invalid in Zephyr 4.x — `main()` must return `int`.
   Using `void` can corrupt the stack on return.

**Fix:** Add to every `prj.conf`:
```kconfig
CONFIG_COMMON_LIBC_MALLOC_ARENA_SIZE=0
```
And change every `main.c` signature:
```c
/* wrong — Zephyr 4.x requires int */
void main(void) { ... }

/* correct */
int main(void) { ...; return 0; }
```
Also remove `CONFIG_STDOUT_CONSOLE=y` if present — it enables a printf
path that allocates from the same heap and is not needed when using `printk`.

**Diagnostic:** Open the `.map` file and check that `sys_heap_init` is at the
faulting PC address. Confirm with:
```
grep "sys_heap_init" build/zephyr/zephyr.map
```

---

## Runtime and flashing errors

### `LIBUSB_ERROR_ACCESS` when running `west flash`
**Cause:** User lacks permissions to access the J-Link USB device.
**Fix:**
```bash
sudo usermod -aG plugdev $USER
newgrp plugdev   # apply without logging out
```

### Serial terminal shows garbage or no output
**Cause:** Wrong baud rate (Zephyr default is 115200) or wrong port.
**Fix:**
```bash
ls /dev/ttyACM*
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
Check logs for "Stack sentinel violated" or per-thread stack usage output.

### `sensor_sample_fetch` returns `-5` (EIO)
**Cause:** I2C address mismatch or SDA/SCL wiring error.
**Fix:**
1. Verify the sensor's I2C address in the overlay (e.g., `reg = <0x0e>` for KXTJ3).
2. Check wiring: SDA → P0.26, SCL → P0.27 on the nRF52840 DK.
3. Use a logic analyser or the Zephyr shell `i2c scan i2c@40003000` to detect the sensor.

### BLE not advertising — no device in nRF Connect
**Cause:** `bt_enable` or `bt_le_adv_start` returned an error, or
`CONFIG_BT_PERIPHERAL=y` is missing.
**Fix:** Add logging and check for error codes:
```c
int ret = bt_enable(NULL);
if (ret < 0) { LOG_ERR("bt_enable: %d", ret); }
```
`-22` (EINVAL) means invalid advertising parameters.

### NVS write returns `-28` (ENOSPC)
**Cause:** NVS partition is full or `sector_count` is too small.
**Fix:** Increase `sector_count` in the `nvs_fs` struct, or call
`nvs_clear(&fs)` to erase all entries and start fresh.

---

## Quick reference: common errno values

| Code | Value | Meaning              |
|------|-------|----------------------|
| ENODEV  | -19 | Device not found or not ready |
| EINVAL  | -22 | Invalid argument     |
| ENOSPC  | -28 | No space left (NVS full) |
| EIO     | -5  | I/O error (sensor comms failure) |
| EBUSY   | -16 | Device busy          |
| ENOMEM  | -12 | Out of memory        |
| ETIMEDOUT | -110 | Operation timed out |
