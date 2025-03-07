cmake_minimum_required(VERSION 3.13...3.27)

# Comment the line below in for the Pico W examples to build
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350-arm-s)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_firmware_vga C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# name anything you want
add_executable(pico_firmware_vga)

# must match with pio filename and executable name from above
pico_generate_pio_header(pico_firmware_vga ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
pico_generate_pio_header(pico_firmware_vga ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
pico_generate_pio_header(pico_firmware_vga ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)
pico_generate_pio_header(pico_firmware_vga ${CMAKE_CURRENT_LIST_DIR}/latch.pio)

# must match with executable name and source file names
target_sources(pico_firmware_vga PRIVATE 
	main.c 
	vga_text.c
)

# must match with executable name
target_link_libraries(pico_firmware_vga PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
	hardware_pio 
	hardware_dma 
	hardware_adc 
	hardware_sync
	hardware_irq
	pico_multicore
)

# must match with executable name
pico_add_extra_outputs(pico_firmware_vga)

add_compile_options(-Ofast)
