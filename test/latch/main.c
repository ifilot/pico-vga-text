#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include "latch.pio.h"

// Main function
int main() {
    // Initialize UART for debugging (output captured data to the terminal)
    stdio_init_all();
    printf("\nSTART\n");

    // Choose PIO and state machine
    PIO pio = pio0;
    uint sm = 0;

    // Define latch pin (GPIO 2) and data pins (GPIO 8-15)
    uint latch_pin = 2;
    uint data_base_pin = 8;

    // Initialize PIO program
    uint offset = pio_add_program(pio, &latch_program);
    latch_program_init(pio, sm, offset, latch_pin, data_base_pin);

    // Main loop to continuously read and display captured data
    while (true) {
        if (pio_sm_is_rx_fifo_empty(pio, sm) == false) {
            // If the FIFO has data, read and print it
            uint32_t captured_data = pio_sm_get_blocking(pio, sm);  // Get data from FIFO
            printf("Captured Data: 0x%08X\n", captured_data);  // Print data in hex
        }
        sleep_ms(100);  // Wait a bit before checking again
    }
}
