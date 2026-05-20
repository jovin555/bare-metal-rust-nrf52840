cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-05-31_KXTJ3_CBOR_USB)

target_sources(app PRIVATE
    src/main.c
    drivers/kxtj3/kxtj3.c
)

target_include_directories(app PRIVATE drivers/kxtj3)
