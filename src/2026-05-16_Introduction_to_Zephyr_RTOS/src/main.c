#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    printk("Hello Zephyr! This is Day 1 of learning Zephyr.\n");
    while (1) {
        k_msleep(1000);
    }
}
