#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/pm/pm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day24, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(DT_ALIAS(sw0),  gpios);

static void enter_system_off(void)
{
    LOG_INF("Entering System OFF — press RESET to wake");
    k_msleep(100);  /* flush log */

    /* Configure button pin as wake source */
    gpio_pin_configure_dt(&btn, GPIO_INPUT);
    gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_LEVEL_ACTIVE);

    /* Force System OFF — does not return */
    pm_state_force(0u, &(struct pm_state_info){PM_STATE_SOFT_OFF, 0, 0});
    k_sleep(K_FOREVER);
}

int main(void)
{
    if (!gpio_is_ready_dt(&led) || !gpio_is_ready_dt(&btn)) {
        LOG_ERR("GPIO not ready");
        return;
    }

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&btn, GPIO_INPUT);

    LOG_INF("Day 24: PM enabled — hold sw0 >2 s for System OFF");

    int cycle = 0;

    while (1) {
        /* Do work */
        gpio_pin_toggle_dt(&led);
        LOG_INF("Cycle %d — sleeping 5 s (PM will reduce current)", cycle++);

        /* Check for long button press → System OFF */
        int held_ms = 0;
        while (gpio_pin_get_dt(&btn) && held_ms < 2000) {
            k_msleep(50);
            held_ms += 50;
        }
        if (held_ms >= 2000) {
            enter_system_off();
        }

        /* Sleep — Zephyr PM subsystem enters low-power state automatically */
        k_msleep(5000);
    }
    return 0;
}