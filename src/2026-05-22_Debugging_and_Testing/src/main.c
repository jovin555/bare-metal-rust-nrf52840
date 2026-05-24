#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day7, LOG_LEVEL_INF);

int main(void)
{
    int step = 0;
    while (1) {
        LOG_INF("Day 7: debug step %d", step++);
        k_msleep(K_SECONDS(1));
    }
    return 0;
}