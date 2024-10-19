
cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(device)

# Verify that this samnple is built through sysbuild to ensure MCUboot is
# automatically included and that sample specific MCUboot configurations are
# used when building MCUboot.
test_sysbuild()

target_sources(
    app 
    PRIVATE 
    src/main.c
    src/commands.c
    src/utils.c
)

target_include_directories(
    app
    PUBLIC
    src/
)

add_subdirectory(src/communication)
if(${BOARD} STREQUAL "nrf52840dk")
    add_subdirectory(src/ui)
endif()

add_subdirectory(src/sensors)
