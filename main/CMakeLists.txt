cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

pico_sdk_init()

add_subdirectory("../mpu6050" "${CMAKE_BINARY_DIR}/mpu6050")


add_executable(main main.c)

# pull in common dependencies
target_link_libraries(main pico_stdlib pico_stdlib hardware_i2c mpu6050)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(main)


