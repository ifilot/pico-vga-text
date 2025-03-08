cmake_minimum_required(VERSION 3.13...3.27)

# Comment the line below in for the Pico W examples to build
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350-arm-s)

# initialize pico-sdk
set(PICO_SDK_PATH /opt/pico-sdk)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

project(buzzer_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# name anything you want
add_executable(buzzer_test)

# must match with executable name and source file names
target_sources(buzzer_test PRIVATE 
	main.c 
)

# must match with executable name
target_link_libraries(buzzer_test PRIVATE 
	pico_stdlib 
	pico_bootsel_via_double_reset 
	hardware_pio 
	hardware_dma 
	hardware_pwm
	hardware_adc 
	hardware_sync
	hardware_irq
	pico_multicore
)

# must match with executable name
pico_add_extra_outputs(buzzer_test)

add_compile_options(-Ofast)
