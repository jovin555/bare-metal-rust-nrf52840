#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

void main(void)
{
    const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

    if (!device_is_ready(uart)) {
        printk("Day 3: uart0 device is not ready.\n");
    } else {
        printk("Day 3: uart0 device is ready via Device Tree.\n");
    }
    while (1) {
        k_msleep(1000);
    }
}
