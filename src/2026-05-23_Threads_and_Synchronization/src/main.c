#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day8, LOG_LEVEL_INF);

K_MSGQ_DEFINE(counter_queue, sizeof(int), 10, 4);

void producer_fn(void *a, void *b, void *c)
{
    int count = 0;

    while (1) {
        k_msgq_put(&counter_queue, &count, K_NO_WAIT);
        count++;
        k_msleep(1000);
    }
}

void consumer_fn(void *a, void *b, void *c)
{
    int value;

    while (1) {
        k_msgq_get(&counter_queue, &value, K_FOREVER);
        LOG_INF("Day 8: received %d", value);
    }
}

K_THREAD_DEFINE(producer, 1024, producer_fn, NULL, NULL, NULL, 5, 0, 0);
K_THREAD_DEFINE(consumer, 1024, consumer_fn, NULL, NULL, NULL, 5, 0, 0);

void main(void)
{
    LOG_INF("Day 8: threads and synchronization started");
}
