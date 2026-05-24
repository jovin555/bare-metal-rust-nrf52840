# Day 18 — BLE Peripheral Advertising

## Goal
Set up the nRF52840 as a Bluetooth Low Energy (BLE) peripheral that advertises
a device name and a custom manufacturer data payload, connectable by a phone or PC.

## What you will learn
- How to enable and initialize the Zephyr BLE stack
- How to construct advertising and scan response data
- How to start and stop advertising
- How to handle connect and disconnect events
- How to read the device address and advertised name with a phone

## Overview
BLE advertising is the mechanism by which a peripheral announces its presence.
The nRF52840 has a built-in radio that Zephyr drives through the `bt_` API.
No external hardware is required — just the SoC and a power supply.

### prj.conf
```kconfig
CONFIG_BT=y
CONFIG_BT_PERIPHERAL=y
CONFIG_BT_DEVICE_NAME="MyZephyrDevice"
CONFIG_BT_DEVICE_APPEARANCE=0
CONFIG_LOG=y
```

### Advertising data structure
```c
static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
    BT_DATA(BT_DATA_NAME_COMPLETE, CONFIG_BT_DEVICE_NAME,
            sizeof(CONFIG_BT_DEVICE_NAME) - 1),
};

/* Manufacturer-specific data in scan response */
static const uint8_t manuf_data[] = { 0xFF, 0x07, 0x01, 0x02, 0x03 };
static const struct bt_data sd[] = {
    BT_DATA(BT_DATA_MANUFACTURER_DATA, manuf_data, sizeof(manuf_data)),
};
```

### Starting advertising
```c
bt_enable(NULL);   /* initialise the BLE stack */

bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
```

`BT_LE_ADV_CONN` = connectable undirected advertising at 100 ms interval.

### Connection callbacks
```c
static void connected(struct bt_conn *conn, uint8_t err) {
    printk("Connected\n");
}
static void disconnected(struct bt_conn *conn, uint8_t reason) {
    printk("Disconnected (reason %u)\n", reason);
    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
    .connected    = connected,
    .disconnected = disconnected,
};
```

### Verifying on a phone
- **Android**: nRF Connect app → Scanner tab → find "MyZephyrDevice"
- **iOS**: LightBlue app → Peripherals tab → find "MyZephyrDevice"
- **Linux**: `bluetoothctl scan on` → look for the device name

## Why this matters
BLE advertising is the entry point to all BLE applications — sensor beacons,
wearables, and IoT devices all start here. Once advertising works, you can
add GATT services to expose data to connected clients.

## Practice tasks
1. Flash and scan with nRF Connect — confirm the device name appears.
2. Change `CONFIG_BT_DEVICE_NAME` and rebuild — confirm the new name.
3. Add a second byte to `manuf_data` containing a counter that increments
   each time the device boots (combine with NVS from Day 22).

## Example folder
View the complete source code on GitHub: [src/2026-06-02_BLE_Peripheral_Advertising](https://github.com/jovin555/My-Zephyr-project/tree/master/src/2026-06-02_BLE_Peripheral_Advertising)

## Next topic
Tomorrow we look at SPI communication to interface with external peripherals.
