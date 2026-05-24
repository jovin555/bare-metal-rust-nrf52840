#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day4, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Day 4: Kconfig and prj.conf example running.");
    while (1) {
        k_msleep(1000);
    }
    return 0;
}