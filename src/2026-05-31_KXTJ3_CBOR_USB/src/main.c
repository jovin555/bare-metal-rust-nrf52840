#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <zcbor_encode.h>
#include <stdlib.h>
#include "kxtj3.h"

LOG_MODULE_REGISTER(day16, LOG_LEVEL_INF);

static const struct device *i2c_dev  = DEVICE_DT_GET(DT_NODELABEL(i2c0));
static const struct device *usb_uart = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

#define KXTJ3_ADDR   KXTJ3_ADDR_LOW
#define CBOR_BUF_LEN 64

static uint32_t seq_num;
static uint8_t  current_range_g = 2;

/* Send raw bytes followed by newline as frame delimiter */
static void usb_send(const uint8_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        uart_poll_out(usb_uart, data[i]);
    }
    uart_poll_out(usb_uart, '\n');
}

/*
 * Encode one accelerometer reading as a CBOR map:
 * { "seq": uint, "x": int, "y": int, "z": int, "rng": uint }
 */
static int encode_and_send(const struct shell *sh)
{
    struct kxtj3_data data;
    int rc = kxtj3_read(i2c_dev, KXTJ3_ADDR, &data);
    if (rc) {
        if (sh) shell_error(sh, "Sensor read failed: %d", rc);
        return rc;
    }

    uint8_t buf[CBOR_BUF_LEN];
    ZCBOR_STATE_E(states, 2, buf, sizeof(buf), 1);

    bool ok = zcbor_map_start_encode(states, 5)
           && zcbor_tstr_put_lit(states, "seq") && zcbor_uint32_put(states, seq_num)
           && zcbor_tstr_put_lit(states, "x")   && zcbor_int32_put(states, data.x_mg)
           && zcbor_tstr_put_lit(states, "y")   && zcbor_int32_put(states, data.y_mg)
           && zcbor_tstr_put_lit(states, "z")   && zcbor_int32_put(states, data.z_mg)
           && zcbor_tstr_put_lit(states, "rng") && zcbor_uint32_put(states, current_range_g)
           && zcbor_map_end_encode(states, 5);

    if (!ok) {
        if (sh) shell_error(sh, "CBOR encode failed");
        return -ENOMEM;
    }

    size_t len = (size_t)(states[0].payload - buf);
    usb_send(buf, len);
    seq_num++;
    return 0;
}

/* accel whoami */
static int cmd_accel_whoami(const struct shell *sh, size_t argc, char **argv)
{
    uint8_t id;
    int rc = kxtj3_who_am_i(i2c_dev, KXTJ3_ADDR, &id);
    if (rc) {
        shell_error(sh, "I2C error: %d", rc);
        return rc;
    }
    if (id == KXTJ3_WHO_AM_I_VAL) {
        shell_print(sh, "WHO_AM_I = 0x%02X  OK (KXTJ3-1057 detected)", id);
    } else {
        shell_error(sh, "WHO_AM_I = 0x%02X  UNEXPECTED (want 0x%02X)", id, KXTJ3_WHO_AM_I_VAL);
    }
    return 0;
}

/* accel read — plain text single reading */
static int cmd_accel_read(const struct shell *sh, size_t argc, char **argv)
{
    struct kxtj3_data data;
    int rc = kxtj3_read(i2c_dev, KXTJ3_ADDR, &data);
    if (rc) {
        shell_error(sh, "Read failed: %d", rc);
        return rc;
    }
    shell_print(sh, "X: %5d mg    Y: %5d mg    Z: %5d mg", data.x_mg, data.y_mg, data.z_mg);
    return 0;
}

/* accel cbor — single CBOR-encoded frame over USB */
static int cmd_accel_cbor(const struct shell *sh, size_t argc, char **argv)
{
    int rc = encode_and_send(sh);
    if (rc == 0) {
        shell_print(sh, "CBOR frame sent (seq=%u)", seq_num - 1);
    }
    return rc;
}

/* accel stream <count> — send n CBOR frames at 200 ms intervals */
static int cmd_accel_stream(const struct shell *sh, size_t argc, char **argv)
{
    int n = (argc >= 2) ? atoi(argv[1]) : 10;
    if (n <= 0 || n > 1000) {
        shell_error(sh, "count must be 1–1000");
        return -EINVAL;
    }

    shell_print(sh, "Streaming %d CBOR frames...", n);
    for (int i = 0; i < n; i++) {
        encode_and_send(sh);
        k_msleep(200);
    }
    shell_print(sh, "Stream done. %d frames sent.", n);
    return 0;
}

/* accel range <2|4|8|16> */
static int cmd_accel_range(const struct shell *sh, size_t argc, char **argv)
{
    if (argc < 2) {
        shell_error(sh, "Usage: accel range <2|4|8|16>");
        return -EINVAL;
    }
    int g = atoi(argv[1]);
    if (g != 2 && g != 4 && g != 8 && g != 16) {
        shell_error(sh, "Valid ranges: 2, 4, 8, 16");
        return -EINVAL;
    }
    int rc = kxtj3_set_range(i2c_dev, KXTJ3_ADDR, (uint8_t)g);
    if (rc == 0) {
        current_range_g = (uint8_t)g;
        shell_print(sh, "Range set to ±%dg", g);
    } else {
        shell_error(sh, "Failed: %d", rc);
    }
    return rc;
}

SHELL_STATIC_SUBCMD_SET_CREATE(accel_cmds,
    SHELL_CMD(whoami, NULL, "Read WHO_AM_I register",           cmd_accel_whoami),
    SHELL_CMD(read,   NULL, "Single reading as plain text",     cmd_accel_read),
    SHELL_CMD(cbor,   NULL, "Send one CBOR frame over USB",     cmd_accel_cbor),
    SHELL_CMD(stream, NULL, "Stream CBOR frames: stream <n>",   cmd_accel_stream),
    SHELL_CMD(range,  NULL, "Set range: range <2|4|8|16>",      cmd_accel_range),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(accel, &accel_cmds, "KXTJ3-1057 accelerometer commands", NULL);

int main(void)
{
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C not ready");
        return;
    }

    if (!device_is_ready(usb_uart) || usb_enable(NULL) != 0) {
        LOG_ERR("USB init failed");
        return;
    }

    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_uart, UART_LINE_CTRL_DTR, &dtr);
        k_msleep(100);
    }

    LOG_INF("Day 16: USB CBOR shell ready");

    if (kxtj3_init(i2c_dev, KXTJ3_ADDR) != 0) {
        LOG_ERR("KXTJ3 init failed — check wiring");
    } else {
        LOG_INF("KXTJ3 ready — type 'accel help'");
    }
    return 0;
}