#include <zephyr/kernel.h>
#include <zephyr/usb/usb_device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>
#include "kxtj3.h"

LOG_MODULE_REGISTER(day15, LOG_LEVEL_INF);

static const struct device *i2c_dev   = DEVICE_DT_GET(DT_NODELABEL(i2c0));
static const struct device *usb_uart  = DEVICE_DT_GET(DT_NODELABEL(cdc_acm_uart0));

#define KXTJ3_ADDR KXTJ3_ADDR_LOW   /* Change to KXTJ3_ADDR_HIGH if ADDR pin is high */

/* accel whoami */
static int cmd_accel_whoami(const struct shell *sh, size_t argc, char **argv)
{
    uint8_t id;
    int rc = kxtj3_who_am_i(i2c_dev, KXTJ3_ADDR, &id);

    if (rc) {
        shell_error(sh, "I2C error: %d — check wiring", rc);
        return rc;
    }

    if (id == KXTJ3_WHO_AM_I_VAL) {
        shell_print(sh, "WHO_AM_I = 0x%02X  OK (KXTJ3-1057 detected)", id);
    } else {
        shell_error(sh, "WHO_AM_I = 0x%02X  UNEXPECTED (want 0x%02X)", id, KXTJ3_WHO_AM_I_VAL);
    }
    return 0;
}

/* accel read */
static int cmd_accel_read(const struct shell *sh, size_t argc, char **argv)
{
    struct kxtj3_data data;
    int rc = kxtj3_read(i2c_dev, KXTJ3_ADDR, &data);

    if (rc) {
        shell_error(sh, "Read failed: %d", rc);
        return rc;
    }

    shell_print(sh, "X: %5d mg    Y: %5d mg    Z: %5d mg",
                data.x_mg, data.y_mg, data.z_mg);
    return 0;
}

/* accel stream <count> */
static int cmd_accel_stream(const struct shell *sh, size_t argc, char **argv)
{
    int n = (argc >= 2) ? atoi(argv[1]) : 10;

    if (n <= 0 || n > 1000) {
        shell_error(sh, "count must be 1-1000");
        return -EINVAL;
    }

    shell_print(sh, "Streaming %d readings (200 ms interval)...", n);

    for (int i = 0; i < n; i++) {
        struct kxtj3_data data;
        if (kxtj3_read(i2c_dev, KXTJ3_ADDR, &data) == 0) {
            shell_print(sh, "[%3d]  X: %5d  Y: %5d  Z: %5d  (mg)",
                        i, data.x_mg, data.y_mg, data.z_mg);
        } else {
            shell_error(sh, "[%3d]  read error", i);
        }
        k_msleep(200);
    }

    shell_print(sh, "Stream done.");
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
        shell_print(sh, "Range set to ±%dg", g);
    } else {
        shell_error(sh, "Failed to set range: %d", rc);
    }
    return rc;
}

SHELL_STATIC_SUBCMD_SET_CREATE(accel_cmds,
    SHELL_CMD(whoami, NULL, "Read WHO_AM_I register",          cmd_accel_whoami),
    SHELL_CMD(read,   NULL, "Read X Y Z in mg",                cmd_accel_read),
    SHELL_CMD(stream, NULL, "Stream readings: stream <count>", cmd_accel_stream),
    SHELL_CMD(range,  NULL, "Set range: range <2|4|8|16>",     cmd_accel_range),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(accel, &accel_cmds, "KXTJ3-1057 accelerometer commands", NULL);

int main(void)
{
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    if (!device_is_ready(usb_uart) || usb_enable(NULL) != 0) {
        LOG_ERR("USB init failed");
        return;
    }

    /* Wait for PC to open the COM port */
    uint32_t dtr = 0;
    while (!dtr) {
        uart_line_ctrl_get(usb_uart, UART_LINE_CTRL_DTR, &dtr);
        k_msleep(100);
    }

    LOG_INF("Day 15: USB shell ready");

    int rc = kxtj3_init(i2c_dev, KXTJ3_ADDR);
    if (rc) {
        LOG_ERR("KXTJ3 init failed (%d) — check wiring and I2C address", rc);
    } else {
        LOG_INF("KXTJ3 ready — type 'accel help' for commands");
    }
    return 0;
}