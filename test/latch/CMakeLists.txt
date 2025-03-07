cmake_minimum_required(VERSION 3.13...3.27)

# Comment the line below in for the Pico W examples to build
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350-arm-s)

# initialize pico-sdk from GIT
set(PICO_SDK_PATH /opt/pico/pico-sdk)

# Initialize the SDK
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(latch_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# name anything you want
add_executable(latch_test)

# must match with pio filename and executable name from above
pico_generate_pio_header(latch_test ${CMAKE_CURRENT_LIST_DIR}/latch.pio)

# must match with executable name and source file names
target_sources(latch_test PRIVATE 
	main.c 
)

# must match with executable name
target_link_libraries(latch_test PRIVATE 
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
pico_add_extra_outputs(latch_test)

add_compile_options(-Ofast)
