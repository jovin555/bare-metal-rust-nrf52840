#ifndef KXTJ3_H
#define KXTJ3_H

#include <zephyr/device.h>
#include <stdint.h>

/* I2C addresses */
#define KXTJ3_ADDR_LOW  0x0E   /* ADDR pin → GND */
#define KXTJ3_ADDR_HIGH 0x0F   /* ADDR pin → VDD */

/* Registers */
#define KXTJ3_REG_XOUT_L   0x06
#define KXTJ3_REG_XOUT_H   0x07
#define KXTJ3_REG_YOUT_L   0x08
#define KXTJ3_REG_YOUT_H   0x09
#define KXTJ3_REG_ZOUT_L   0x0A
#define KXTJ3_REG_ZOUT_H   0x0B
#define KXTJ3_REG_WHO_AM_I 0x0F
#define KXTJ3_REG_CTRL1    0x1B

#define KXTJ3_WHO_AM_I_VAL 0x35

/* CTRL_REG1 bits */
#define KXTJ3_CTRL1_PC1    BIT(7)   /* 1 = operating mode */
#define KXTJ3_CTRL1_RES    BIT(6)   /* 1 = 12-bit resolution */
#define KXTJ3_CTRL1_2G     0x00
#define KXTJ3_CTRL1_4G     BIT(3)
#define KXTJ3_CTRL1_8G     BIT(4)
#define KXTJ3_CTRL1_16G    (BIT(4) | BIT(3))

struct kxtj3_data {
    int32_t x_mg;
    int32_t y_mg;
    int32_t z_mg;
};

int kxtj3_init(const struct device *i2c_dev, uint8_t addr);
int kxtj3_who_am_i(const struct device *i2c_dev, uint8_t addr, uint8_t *id);
int kxtj3_set_range(const struct device *i2c_dev, uint8_t addr, uint8_t range_g);
int kxtj3_read(const struct device *i2c_dev, uint8_t addr, struct kxtj3_data *out);

#endif /* KXTJ3_H */
