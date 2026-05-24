#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/logging/log.h>
#include <string.h>

LOG_MODULE_REGISTER(day19, LOG_LEVEL_INF);

#define FLASH_NODE   DT_NODELABEL(w25q)
#define TEST_ADDR    0x000000
#define SECTOR_SIZE  4096
#define PAGE_SIZE    256

static const struct device *flash_dev = DEVICE_DT_GET(FLASH_NODE);

int main(void)
{
    if (!device_is_ready(flash_dev)) {
        LOG_ERR("Day 19: SPI flash not ready — check wiring");
        return;
    }
    LOG_INF("Day 19: SPI flash ready");

    /* Erase first sector */
    LOG_INF("Erasing sector at 0x%06X...", TEST_ADDR);
    int rc = flash_erase(flash_dev, TEST_ADDR, SECTOR_SIZE);
    if (rc) {
        LOG_ERR("Erase failed: %d", rc);
        return;
    }

    /* Write a test string */
    const char msg[] = "Hello from Zephyr SPI!";
    LOG_INF("Writing: \"%s\"", msg);
    rc = flash_write(flash_dev, TEST_ADDR, msg, strlen(msg));
    if (rc) {
        LOG_ERR("Write failed: %d", rc);
        return;
    }

    /* Read back and verify */
    char read_buf[64] = { 0 };
    rc = flash_read(flash_dev, TEST_ADDR, read_buf, strlen(msg));
    if (rc) {
        LOG_ERR("Read failed: %d", rc);
        return;
    }

    LOG_INF("Read back: \"%s\"", read_buf);

    if (memcmp(msg, read_buf, strlen(msg)) == 0) {
        LOG_INF("Verification PASSED");
    } else {
        LOG_ERR("Verification FAILED");
    }
    return 0;
}