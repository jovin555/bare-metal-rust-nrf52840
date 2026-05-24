#include <zephyr/drivers/i2c.h>
#include <zephyr/logging/log.h>
#include "kxtj3.h"

LOG_MODULE_REGISTER(kxtj3, LOG_LEVEL_INF);

static uint8_t current_range_g = 2;
static int32_t counts_per_g    = 1024;  /* default for ±2g 12-bit */

static int reg_read(const struct device *i2c, uint8_t addr,
                    uint8_t reg, uint8_t *val)
{
    return i2c_reg_read_byte(i2c, addr, reg, val);
}

static int reg_write(const struct device *i2c, uint8_t addr,
                     uint8_t reg, uint8_t val)
{
    return i2c_reg_write_byte(i2c, addr, reg, val);
}

int kxtj3_who_am_i(const struct device *i2c, uint8_t addr, uint8_t *id)
{
    return reg_read(i2c, addr, KXTJ3_REG_WHO_AM_I, id);
}

int kxtj3_set_range(const struct device *i2c, uint8_t addr, uint8_t range_g)
{
    uint8_t gsel;

    switch (range_g) {
    case 2:  gsel = KXTJ3_CTRL1_2G;  counts_per_g = 1024; break;
    case 4:  gsel = KXTJ3_CTRL1_4G;  counts_per_g = 512;  break;
    case 8:  gsel = KXTJ3_CTRL1_8G;  counts_per_g = 256;  break;
    case 16: gsel = KXTJ3_CTRL1_16G; counts_per_g = 128;  break;
    default:
        LOG_ERR("Invalid range %dg", range_g);
        return -EINVAL;
    }

    /* Standby first, then reconfigure, then re-enable */
    int rc = reg_write(i2c, addr, KXTJ3_REG_CTRL1, 0x00);
    if (rc) return rc;

    rc = reg_write(i2c, addr, KXTJ3_REG_CTRL1,
                   KXTJ3_CTRL1_PC1 | KXTJ3_CTRL1_RES | gsel);
    if (rc == 0) {
        current_range_g = range_g;
        LOG_INF("KXTJ3 range set to ±%dg", range_g);
    }
    return rc;
}

int kxtj3_init(const struct device *i2c, uint8_t addr)
{
    uint8_t id;
    int rc = kxtj3_who_am_i(i2c, addr, &id);
    if (rc) {
        LOG_ERR("I2C read failed (err %d) — check wiring", rc);
        return rc;
    }
    if (id != KXTJ3_WHO_AM_I_VAL) {
        LOG_ERR("Unexpected WHO_AM_I 0x%02X (expected 0x%02X)", id, KXTJ3_WHO_AM_I_VAL);
        return -ENODEV;
    }

    LOG_INF("KXTJ3 found (WHO_AM_I=0x%02X)", id);
    return kxtj3_set_range(i2c, addr, 2);
}

int kxtj3_read(const struct device *i2c, uint8_t addr, struct kxtj3_data *out)
{
    uint8_t buf[6];
    int rc = i2c_burst_read(i2c, addr, KXTJ3_REG_XOUT_L, buf, sizeof(buf));
    if (rc) {
        LOG_ERR("Burst read failed (err %d)", rc);
        return rc;
    }

    /* 12-bit result is left-justified: shift right 4 to get signed value */
    int16_t raw_x = (int16_t)((buf[1] << 8) | buf[0]) >> 4;
    int16_t raw_y = (int16_t)((buf[3] << 8) | buf[2]) >> 4;
    int16_t raw_z = (int16_t)((buf[5] << 8) | buf[4]) >> 4;

    out->x_mg = (int32_t)raw_x * 1000 / counts_per_g;
    out->y_mg = (int32_t)raw_y * 1000 / counts_per_g;
    out->z_mg = (int32_t)raw_z * 1000 / counts_per_g;

    return 0;
}
