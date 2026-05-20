#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <stdlib.h>

LOG_MODULE_REGISTER(day13, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* ping command */
static int cmd_ping(const struct shell *sh, size_t argc, char **argv)
{
    shell_print(sh, "pong");
    return 0;
}

/* led on / led off subcommands */
static int cmd_led_on(const struct shell *sh, size_t argc, char **argv)
{
    gpio_pin_set_dt(&led, 1);
    shell_print(sh, "LED on");
    return 0;
}

static int cmd_led_off(const struct shell *sh, size_t argc, char **argv)
{
    gpio_pin_set_dt(&led, 0);
    shell_print(sh, "LED off");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(led_cmds,
    SHELL_CMD(on,  NULL, "Turn LED on",  cmd_led_on),
    SHELL_CMD(off, NULL, "Turn LED off", cmd_led_off),
    SHELL_SUBCMD_SET_END
);

/* read temp subcommand */
static int cmd_read_temp(const struct shell *sh, size_t argc, char **argv)
{
    /* Simulated reading: replace with real sensor call later */
    shell_print(sh, "temperature: 23.50 C");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(read_cmds,
    SHELL_CMD(temp, NULL, "Read temperature", cmd_read_temp),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(ping, NULL, "Reply with pong", cmd_ping);
SHELL_CMD_REGISTER(led,  &led_cmds,  "LED control", NULL);
SHELL_CMD_REGISTER(read, &read_cmds, "Read sensors", NULL);

void main(void)
{
    if (gpio_is_ready_dt(&led)) {
        gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    }
    LOG_INF("Day 13: shell ready — type 'help'");
}
