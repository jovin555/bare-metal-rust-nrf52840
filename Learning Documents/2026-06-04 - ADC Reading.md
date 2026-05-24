# 2026-06-04 - ADC Reading

## Goal
Use the nRF52840's built-in 12-bit SAR ADC (SAADC) to read an analog voltage,
convert it to millivolts, and log the result periodically.

## What you will learn
- How to declare an ADC channel in the Device Tree
- How to configure gain, reference, and acquisition time
- How to perform a single-shot ADC reading with `adc_read`
- How to convert raw counts to millivolts using `adc_raw_to_millivolts`
- Practical uses: battery monitoring, potentiometer, analog sensor

## Overview
The nRF52840 SAADC has 8 channels, 8/10/12-bit resolution, configurable
gain and reference. Zephyr abstracts this through the `adc_` API.

### app.overlay
```dts
/ {
    zephyr,user {
        /* AIN0 maps to P0.02 on nRF52840 */
        io-channels = <&adc 0>;
    };
};

&adc {
    status = "okay";
    #address-cells = <1>;
    #size-cells = <0>;

    channel@0 {
        reg = <0>;
        zephyr,gain           = "ADC_GAIN_1_6";
        zephyr,reference      = "ADC_REF_INTERNAL";
        zephyr,acquisition-time = <ADC_ACQ_TIME_DEFAULT>;
        zephyr,input-positive = <NRF_SAADC_AIN0>;   /* P0.02 */
    };
};
```

### prj.conf
```kconfig
CONFIG_ADC=y
CONFIG_LOG=y
```

### Reading the ADC
```c
#include <zephyr/drivers/adc.h>

#define ADC_NODE    DT_PATH(zephyr_user)
#define ADC_CHANNEL DT_IO_CHANNELS_INPUT_BY_IDX(ADC_NODE, 0)

static const struct adc_dt_spec adc_ch = ADC_DT_SPEC_GET(ADC_NODE);

void read_adc(void)
{
    int16_t raw;
    struct adc_sequence seq = {
        .buffer      = &raw,
        .buffer_size = sizeof(raw),
    };

    adc_sequence_init_dt(&adc_ch, &seq);
    adc_read(adc_ch.dev, &seq);

    int32_t mv = raw;
    adc_raw_to_millivolts_dt(&adc_ch, &mv);
    printk("ADC: raw=%d  voltage=%d mV\n", raw, mv);
}
```

### nRF52840 SAADC AIN mapping
| AIN  | GPIO  | Notes |
|------|-------|-------|
| AIN0 | P0.02 | Also used for UART RTS on DK — use with care |
| AIN1 | P0.03 | Also used for UART CTS |
| AIN2 | P0.04 | Free on most DK configs |
| AIN3 | P0.05 | Free on most DK configs |
| AIN4 | P0.28 | |
| AIN5 | P0.29 | |
| AIN6 | P0.30 | |
| AIN7 | P0.31 | |

### Measuring battery voltage (resistor divider)
Wire a 10 MΩ / 4.7 MΩ divider from VBAT to AIN, GND.
Scale factor = (10 + 4.7) / 4.7 ≈ 3.13.
```c
int32_t vbat_mv = mv * 147 / 47;  /* fixed-point scale */
```

## Why this matters
ADC is essential for battery monitoring, analog sensors (light, temperature via
NTC, pressure), and potentiometer-based UIs. Understanding gain and reference
settings prevents measurement errors.

## Practice tasks
1. Wire a 10 kΩ potentiometer (wiper to AIN2/P0.04) and confirm readings sweep
   0–3600 mV as you turn it.
2. Add a second ADC channel on AIN3 and log both simultaneously.
3. Average 8 consecutive readings to reduce noise.

## Example folder
See `src/2026-06-04_ADC_Reading` for the day's code.

## Next topic
Tomorrow we use PWM to smoothly dim an LED and control servo position.
