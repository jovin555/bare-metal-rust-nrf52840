#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day10, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

void main(void)
{
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("Day 10: LED GPIO not ready");
        return;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    LOG_INF("Day 10: custom board running, blinking LED");

    while (1) {
        gpio_pin_toggle_dt(&led);
        k_msleep(500);
    }
}
