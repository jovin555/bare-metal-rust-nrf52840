cmake_minimum_required(VERSION 3.13.1)

# Tell Zephyr to also search the local boards/ directory
set(BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-05-25_Custom_PCB_Board_Definition)

target_sources(app PRIVATE src/main.c)
