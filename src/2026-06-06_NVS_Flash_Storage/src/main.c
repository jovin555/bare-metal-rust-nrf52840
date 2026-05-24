#include <zephyr/kernel.h>
#include <zephyr/fs/nvs.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(day22, LOG_LEVEL_INF);

#define NVS_ID_BOOT_COUNT  1
#define NVS_ID_DEVICE_CFG  2
#define NVS_SECTOR_SIZE    4096
#define NVS_SECTOR_COUNT   4

struct device_cfg {
    uint8_t  range_g;
    uint16_t sample_rate_ms;
    char     name[16];
};

static struct nvs_fs fs;

static int nvs_init(void)
{
    const struct flash_area *fa;
    int rc = flash_area_open(FIXED_PARTITION_ID(storage_partition), &fa);
    if (rc) {
        LOG_ERR("Cannot open storage partition: %d", rc);
        return rc;
    }

    fs.flash_device = flash_area_get_device(fa);
    fs.offset       = fa->fa_off;
    fs.sector_size  = NVS_SECTOR_SIZE;
    fs.sector_count = NVS_SECTOR_COUNT;

    rc = nvs_mount(&fs);
    if (rc) {
        LOG_ERR("NVS mount failed: %d", rc);
    }
    return rc;
}

void main(void)
{
    if (nvs_init() != 0) {
        return;
    }
    LOG_INF("Day 22: NVS ready");

    /* Boot counter */
    uint32_t boot_count = 0;
    nvs_read(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));
    boot_count++;
    nvs_write(&fs, NVS_ID_BOOT_COUNT, &boot_count, sizeof(boot_count));
    LOG_INF("Boot count: %u", boot_count);

    /* Config struct — write defaults on first boot, reload on subsequent */
    struct device_cfg cfg = { 0 };
    ssize_t rc = nvs_read(&fs, NVS_ID_DEVICE_CFG, &cfg, sizeof(cfg));

    if (rc <= 0) {
        LOG_INF("First boot — writing default config");
        cfg.range_g        = 2;
        cfg.sample_rate_ms = 200;
        strncpy(cfg.name, "sensor1", sizeof(cfg.name) - 1);
        nvs_write(&fs, NVS_ID_DEVICE_CFG, &cfg, sizeof(cfg));
    } else {
        LOG_INF("Loaded config: range=±%dg  rate=%dms  name=%s",
                cfg.range_g, cfg.sample_rate_ms, cfg.name);
    }
}
