#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    printk("Day 2: Zephyr project structure example is running.\n");
    while (1) {
        k_msleep(1000);
    }
}
