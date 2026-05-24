#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day18, LOG_LEVEL_INF);

static const uint8_t manuf_data[] = { 0xFF, 0x07, 0xDE, 0xAD, 0xBE };

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE,
            CONFIG_BT_DEVICE_NAME, sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, manuf_data, sizeof(manuf_data)),
};

static void start_advertising(void)
{
    int rc = bt_le_adv_start(BT_LE_ADV_CONN,
                             ad, ARRAY_SIZE(ad),
                             sd, ARRAY_SIZE(sd));
    if (rc) {
        LOG_ERR("Advertising start failed (err %d)", rc);
    } else {
        LOG_INF("Advertising as \"%s\"", CONFIG_BT_DEVICE_NAME);
    }
}

static void connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("Connection failed (err %d)", err);
        return;
    }
    LOG_INF("Connected");
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    LOG_INF("Disconnected (reason 0x%02x)", reason);
    start_advertising();
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected    = connected,
    .disconnected = disconnected,
};

int main(void)
{
    int rc = bt_enable(NULL);
    if (rc) {
        LOG_ERR("BLE enable failed (err %d)", rc);
        return;
    }
    LOG_INF("Day 18: BLE stack ready");
    start_advertising();
    return 0;
}