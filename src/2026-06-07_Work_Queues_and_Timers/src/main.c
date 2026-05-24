#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day23, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* --- 1 Hz toggle work --- */
static struct k_work toggle_work;

static void toggle_handler(struct k_work *work)
{
    gpio_pin_toggle_dt(&led);
    LOG_INF("LED toggled at %lld ms", k_uptime_get());
}

static void timer_cb(struct k_timer *timer)
{
    k_work_submit(&toggle_work);
}

K_TIMER_DEFINE(led_timer, timer_cb, NULL);

/* --- 3 s status delayable work --- */
static struct k_work_delayable status_work;

static void status_handler(struct k_work *work)
{
    LOG_INF("Status: uptime=%lld s  free_heap=%u B",
            k_uptime_get() / 1000,
            k_mem_slab_num_free_get(NULL));
    k_work_reschedule(&status_work, K_SECONDS(3));
}

void main(void)
{
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO not ready");
        return;
    }
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    k_work_init(&toggle_work, toggle_handler);
    k_timer_start(&led_timer, K_SECONDS(1), K_SECONDS(1));

    k_work_init_delayable(&status_work, status_handler);
    k_work_schedule(&status_work, K_SECONDS(3));

    LOG_INF("Day 23: timers and work queues running");
}
