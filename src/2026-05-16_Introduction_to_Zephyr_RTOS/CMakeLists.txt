cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-05-16_Introduction_to_Zephyr_RTOS)

target_sources(app PRIVATE src/main.c)
