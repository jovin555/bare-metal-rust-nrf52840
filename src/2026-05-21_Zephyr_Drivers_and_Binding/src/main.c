#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>

void main(void)
{
    const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

    if (!device_is_ready(uart)) {
        printk("Day 6: uart0 is not ready.\n");
        return;
    }

    printk("Day 6: uart0 is ready. Sending test output...\n");
    uart_poll_out(uart, 'Z');
    uart_poll_out(uart, '\n');
    while (1) {
        k_sleep(K_SECONDS(2));
    }
}
