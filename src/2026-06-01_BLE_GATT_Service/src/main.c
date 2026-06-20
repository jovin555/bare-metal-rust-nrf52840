#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(day17, LOG_LEVEL_INF);

/*
 * Custom 128-bit service UUID:  12345678-1234-5678-1234-56789abcdef0
 * Custom characteristic UUID:   12345678-1234-5678-1234-56789abcdef1
 */
#define BT_UUID_SENSOR_SVC  BT_UUID_DECLARE_128( \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0))
#define BT_UUID_SENSOR_VAL  BT_UUID_DECLARE_128( \
    BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef1))

static uint32_t sensor_counter;
static bool     notify_enabled;

static ssize_t read_sensor_val(struct bt_conn *conn,
                               const struct bt_gatt_attr *attr,
                               void *buf, uint16_t len, uint16_t offset)
{
    return bt_gatt_attr_read(conn, attr, buf, len, offset,
                             &sensor_counter, sizeof(sensor_counter));
}

static void ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
    notify_enabled = (value == BT_GATT_CCC_NOTIFY);
    LOG_INF("Notifications %s", notify_enabled ? "enabled" : "disabled");
}

BT_GATT_SERVICE_DEFINE(sensor_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_SENSOR_SVC),
    BT_GATT_CHARACTERISTIC(BT_UUID_SENSOR_VAL,
                           BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ,
                           read_sensor_val, NULL, &sensor_counter),
    BT_GATT_CCC(ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
);

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA_BYTES(BT_DATA_UUID128_ALL,
        BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x5678, 0x1234, 0x56789abcdef0)),
};

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
    notify_enabled = false;
    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
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
        return rc;
    }
    LOG_INF("Day 17: BLE GATT service ready");

    rc = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (rc) {
        LOG_ERR("Advertising failed (err %d)", rc);
        return rc;
    }
    LOG_INF("Advertising as \"%s\"", CONFIG_BT_DEVICE_NAME);

    while (1) {
        k_msleep(1000);
        sensor_counter++;

        if (notify_enabled) {
            bt_gatt_notify(NULL, &sensor_svc.attrs[2],
                           &sensor_counter, sizeof(sensor_counter));
            LOG_INF("Notified counter=%u", sensor_counter);
        }
    }
    return 0;
}
