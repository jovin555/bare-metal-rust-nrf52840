#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day11, LOG_LEVEL_INF);

static const struct device *usb_uart = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

void main(void)
{
    if (!device_is_ready(usb_uart)) {
        return;
    }

    if (usb_enable(NULL) != 0) {
        return;
    }

    /* Wait until the host opens the COM port (DTR goes high) */
    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_uart, UART_LINE_CTRL_DTR, &dtr);
        k_msleep(100);
    }

    LOG_INF("Day 11: USB CDC-ACM ready");

    int count = 0;
    uint8_t rx_buf[64];

    while (1) {
        /* Send a counter line every second */
        printk("count: %d\n", count++);

        /* Echo any received bytes */
        int len = uart_fifo_read(usb_uart, rx_buf, sizeof(rx_buf) - 1);
        if (len > 0) {
            rx_buf[len] = '\0';
            printk("echo: %s\n", rx_buf);
        }

        k_msleep(1000);
    }
}
