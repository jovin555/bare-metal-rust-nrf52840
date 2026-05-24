#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <zcbor_encode.h>

LOG_MODULE_REGISTER(day12, LOG_LEVEL_INF);

static const struct device *usb_uart = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

static void send_bytes(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart_poll_out(usb_uart, data[i]);
    }
    /* newline as a simple frame delimiter */
    uart_poll_out(usb_uart, '\n');
}

static bool encode_sensor_frame(uint8_t *buf, size_t buf_size, size_t *out_len,
                                 int32_t temp_cdeg, int32_t hum_pct, uint32_t seq)
{
    ZCBOR_STATE_E(states, 2, buf, buf_size, 1);

    bool ok = zcbor_map_start_encode(states, 3)
           && zcbor_tstr_put_lit(states, "temp") && zcbor_int32_put(states, temp_cdeg)
           && zcbor_tstr_put_lit(states, "hum")  && zcbor_int32_put(states, hum_pct)
           && zcbor_tstr_put_lit(states, "seq")  && zcbor_uint32_put(states, seq)
           && zcbor_map_end_encode(states, 3);

    if (ok) {
        *out_len = (size_t)(states[0].payload - buf);
    }
    return ok;
}

int main(void)
{
    if (!device_is_ready(usb_uart) || usb_enable(NULL) != 0) {
        return;
    }

    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_uart, UART_LINE_CTRL_DTR, &dtr);
        k_msleep(100);
    }

    LOG_INF("Day 12: CBOR over USB ready");

    uint8_t cbor_buf[64];
    size_t cbor_len;
    uint32_t seq = 0;

    while (1) {
        /* Simulate sensor readings (multiply by 100 for fixed-point) */
        int32_t temp = 2350 + (int32_t)(seq % 10);  /* 23.50–23.59 °C */
        int32_t hum  = 60 + (int32_t)(seq % 20);    /* 60–79 % */

        if (encode_sensor_frame(cbor_buf, sizeof(cbor_buf), &cbor_len, temp, hum, seq)) {
            send_bytes(cbor_buf, cbor_len);
            LOG_INF("Sent CBOR frame seq=%u len=%u", seq, cbor_len);
        }

        seq++;
        k_msleep(1000);
    }
    return 0;
}