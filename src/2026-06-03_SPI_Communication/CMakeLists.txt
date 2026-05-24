cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-06-03_SPI_Communication)

target_sources(app PRIVATE src/main.c)
