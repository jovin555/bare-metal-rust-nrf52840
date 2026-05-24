#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day20, LOG_LEVEL_INF);

static const struct adc_dt_spec adc_ch =
    ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

int main(void)
{
    if (!adc_is_ready_dt(&adc_ch)) {
        LOG_ERR("Day 20: ADC not ready");
        return;
    }

    int rc = adc_channel_setup_dt(&adc_ch);
    if (rc) {
        LOG_ERR("ADC channel setup failed: %d", rc);
        return;
    }

    LOG_INF("Day 20: ADC ready on AIN2 (P0.04), reading every 1 s");

    int16_t raw;
    struct adc_sequence seq = {
        .buffer      = &raw,
        .buffer_size = sizeof(raw),
    };
    adc_sequence_init_dt(&adc_ch, &seq);

    while (1) {
        rc = adc_read(adc_ch.dev, &seq);
        if (rc) {
            LOG_ERR("ADC read failed: %d", rc);
        } else {
            int32_t mv = raw;
            adc_raw_to_millivolts_dt(&adc_ch, &mv);
            LOG_INF("raw=%5d   voltage=%4d mV", raw, mv);
        }
        k_msleep(1000);
    }
    return 0;
}