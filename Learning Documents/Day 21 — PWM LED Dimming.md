# Day 21 — PWM LED Dimming

## Goal
Use Zephyr's PWM API to smoothly dim and brighten an LED using a breathing
animation, and understand how to map PWM channels in the Device Tree.

## What you will learn
- How to declare a PWM channel in the Device Tree using `pwm-leds`
- How to set duty cycle with `pwm_set_dt`
- How to implement a smooth breathing effect
- How PWM period and pulse width relate to duty cycle and frequency

## Overview
PWM (Pulse Width Modulation) rapidly switches a pin on and off. The ratio of
on-time (pulse) to total period (period) is the duty cycle. At 1 kHz, the
eye and motor cannot follow individual pulses — they perceive an average.

### app.overlay
```dts
/ {
    pwmleds {
        compatible = "pwm-leds";
        pwm_led0: pwm_led_0 {
            pwms = <&pwm0 0 PWM_MSEC(20) PWM_POLARITY_NORMAL>;
            label = "PWM LED";
        };
    };

    aliases {
        pwm-led0 = &pwm_led0;
    };
};

&pwm0 {
    status = "okay";
    pinctrl-0 = <&pwm0_default>;
    pinctrl-names = "default";
};

&pinctrl {
    pwm0_default: pwm0_default {
        group1 {
            /* LED1 on nRF52840 DK = P0.13 */
            psels = <NRF_PSEL(PWM_OUT0, 0, 13)>;
        };
    };
};
```

### prj.conf
```kconfig
CONFIG_PWM=y
CONFIG_LOG=y
```

### Setting duty cycle
```c
#include <zephyr/drivers/pwm.h>

static const struct pwm_dt_spec pwm_led =
    PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));

/* 50% duty cycle */
pwm_set_dt(&pwm_led, pwm_led.period, pwm_led.period / 2);

/* 0% — fully off */
pwm_set_dt(&pwm_led, pwm_led.period, 0);

/* 100% — fully on */
pwm_set_dt(&pwm_led, pwm_led.period, pwm_led.period);
```

### Breathing effect
```c
/* Ramp pulse from 0 to period in 50 steps, then back down */
uint32_t period = pwm_led.period;
int step = period / 50;

while (1) {
    for (uint32_t pulse = 0; pulse <= period; pulse += step) {
        pwm_set_dt(&pwm_led, period, pulse);
        k_msleep(20);
    }
    for (uint32_t pulse = period; pulse > 0; pulse -= step) {
        pwm_set_dt(&pwm_led, period, pulse);
        k_msleep(20);
    }
}
```

### Servo control
PWM is also used for hobby servos (50 Hz, 1–2 ms pulse):
```c
/* Neutral position: 1.5 ms pulse at 50 Hz (20 ms period) */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(1500));
/* Full left: 1 ms */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(1000));
/* Full right: 2 ms */
pwm_set_dt(&servo, PWM_MSEC(20), PWM_USEC(2000));
```

## Why this matters
PWM is used everywhere: LED brightness control, motor speed, servo position,
buzzer tones, and switching power supplies. Zephyr's Device Tree mapping
means you change a pin number in the overlay, not in the application code.

## Practice tasks
1. Flash and observe the breathing LED on LED1 (P0.13).
2. Change the step size and sleep delay to speed up or slow down the effect.
3. Wire a buzzer to a PWM pin and play a 1 kHz tone.

## Example folder
View the complete source code on GitHub: [src/2026-06-05_PWM_LED_Dimming](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-05_PWM_LED_Dimming)

## Next topic
Tomorrow we store configuration data in internal flash using Zephyr's NVS.
