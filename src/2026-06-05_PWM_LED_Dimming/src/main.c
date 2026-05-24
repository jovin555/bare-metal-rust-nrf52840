#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day21, LOG_LEVEL_INF);

static const struct pwm_dt_spec pwm_led =
    PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

#define STEPS      50
#define STEP_MS    20

void main(void)
{
    if (!pwm_is_ready_dt(&pwm_led)) {
        LOG_ERR("Day 21: PWM device not ready");
        return;
    }

    LOG_INF("Day 21: PWM breathing on LED1 (period=%u ns)", pwm_led.period);

    uint32_t period = pwm_led.period;
    uint32_t step   = period / STEPS;

    while (1) {
        /* Fade in */
        for (uint32_t pulse = 0; pulse <= period; pulse += step) {
            pwm_set_dt(&pwm_led, period, pulse);
            k_msleep(STEP_MS);
        }
        /* Fade out */
        for (uint32_t pulse = period; pulse > 0; pulse -= step) {
            pwm_set_dt(&pwm_led, period, pulse);
            k_msleep(STEP_MS);
        }
    }
}
