# 2026-05-26 - USB CDC-ACM Communication

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
    usb_enable(NULL);

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
See `src/2026-05-26_USB_CDC_ACM` for the day's code example.

## Next topic
Tomorrow we encode structured data using CBOR and send it over this USB connection.
