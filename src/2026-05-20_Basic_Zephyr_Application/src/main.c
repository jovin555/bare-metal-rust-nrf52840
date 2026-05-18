#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

void main(void)
{
    int count = 0;
    while (1) {
        printk("Day 5: Hello from Zephyr app loop (%d)!\n", count++);
        k_sleep(K_SECONDS(1));
    }
}
