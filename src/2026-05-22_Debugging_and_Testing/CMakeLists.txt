cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(2026-05-22_Debugging_and_Testing)

target_sources(app PRIVATE src/main.c)
