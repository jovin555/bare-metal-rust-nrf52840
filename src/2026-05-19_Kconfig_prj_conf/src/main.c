#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day4, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Day 4: greeting = \"%s\"", CONFIG_DAY4_GREETING);
    LOG_INF("Day 4: sleep interval = %d ms  (change via prj.conf)",
            CONFIG_DAY4_SLEEP_INTERVAL_MS);

    while (1) {
        LOG_INF("tick — sleeping %d ms", CONFIG_DAY4_SLEEP_INTERVAL_MS);
        k_msleep(CONFIG_DAY4_SLEEP_INTERVAL_MS);
    }
    return 0;
}
