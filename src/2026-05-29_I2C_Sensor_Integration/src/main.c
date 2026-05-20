#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

LOG_MODULE_REGISTER(day14, LOG_LEVEL_INF);

static const struct device *bme280 = DEVICE_DT_GET_ANY(bosch_bme280);

void main(void)
{
    if (!device_is_ready(bme280)) {
        LOG_ERR("Day 14: BME280 not ready — check wiring and I2C address in app.overlay");
        return;
    }

    LOG_INF("Day 14: BME280 ready, reading every 2 s");

    struct sensor_value temp, press, hum;

    while (1) {
        if (sensor_sample_fetch(bme280) < 0) {
            LOG_ERR("Failed to fetch sample");
            k_msleep(2000);
            continue;
        }

        sensor_channel_get(bme280, SENSOR_CHAN_AMBIENT_TEMP, &temp);
        sensor_channel_get(bme280, SENSOR_CHAN_PRESS,        &press);
        sensor_channel_get(bme280, SENSOR_CHAN_HUMIDITY,     &hum);

        LOG_INF("Temp:  %d.%02d C",   temp.val1,  abs(temp.val2)  / 10000);
        LOG_INF("Press: %d.%02d kPa", press.val1, abs(press.val2) / 10000);
        LOG_INF("Hum:   %d.%02d %%",  hum.val1,   abs(hum.val2)   / 10000);

        k_msleep(2000);
    }
}
