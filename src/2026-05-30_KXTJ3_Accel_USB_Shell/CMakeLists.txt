cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-05-30_KXTJ3_Accel_USB_Shell)

target_sources(app PRIVATE
    src/main.c
    drivers/kxtj3/kxtj3.c
)

target_include_directories(app PRIVATE drivers/kxtj3)
