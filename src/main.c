/**
 *
 * VGA Display Driver with Multicore Processing
 *
 * HARDWARE CONNECTIONS:
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 470-ohm resistor ---> VGA Green
 *  - GPIO 19 ---> 330-ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330-ohm resistor ---> VGA Blue
 *  - GPIO 21 ---> 330-ohm resistor ---> VGA Red
 *  - RP2040 GND ---> VGA GND
 *
 * RESOURCES USED:
 *  - PIO state machines 0, 1, and 2 on PIO instance 0
 *  - DMA channels obtained via claim mechanism
 *  - 153.6 kBytes of RAM (for pixel color data)
 *
 * Protothreads v1.1.3
 * Threads:
 * Core 0:
 *  - VGA display
 *  - Blink LED25
 * Core 1:
 *  - Toggle GPIO 4
 *  - Serial I/O
 */

#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/dma.h>
#include <hardware/sync.h>
#include <hardware/timer.h>
#include <pico/multicore.h>
#include <string.h>

#include "vga_text.h"
#include "protothreads.h"
 
char user_string;
int newchar = 1;

// ==================================================
// === VGA Graphics Thread (Core 0) ===
// ==================================================
static PT_THREAD(protothread_graphics(struct pt *pt)) {
    PT_BEGIN(pt);

    static short posx = 0;
    static short posy = 0;

    while (true) {
        // Draw new character if received
        if (newchar) {
            newchar = false;
            
            if(user_string == '\n') {
                posy += CHARHEIGHT;
                posx = 0;
            } else {
                drawChar(posx, posy, user_string, DARK_ORANGE, BLACK);

                posx += 8;
                if (posx >= SCREENWIDTH-1) {
                    posx = 0;
                    posy += CHARHEIGHT;
                }
            }
        }

        // Yield to allow other threads to run
        PT_YIELD_usec(100);
    }
    PT_END(pt);
}

// ==================================================
// === LED Blink Thread (Core 0) ===
// ==================================================
static PT_THREAD(protothread_toggle25(struct pt *pt)) {
    PT_BEGIN(pt);
    static bool LED_state = false;

    // Initialize onboard LED (GPIO 25)
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, true);

    PT_INTERVAL_INIT();

    while (1) {
        // Toggle LED every 100ms
        PT_YIELD_INTERVAL(100000);
        LED_state = !LED_state;
        gpio_put(25, LED_state);
    }
    PT_END(pt);
}

// ==================================================
//            Serial Input Thread (Core 1) 
// ==================================================
static PT_THREAD(protothread_serial(struct pt *pt)) {
    PT_BEGIN(pt);
    while (1) {
        // Spawn a thread to handle non-blocking serial write
        serial_write;

        // Spawn a thread to handle non-blocking serial read
        serial_read;

        // Process received character
        if (pt_serial_in_buffer != 0) {
            user_string = pt_serial_in_buffer;
            newchar = true;
        }
    }
    PT_END(pt);
}

// ==================================================
// === Core 1 Main Function ===
// ==================================================
void core1_main() {
    pt_add_thread(protothread_serial);
    pt_schedule_start;
    // Never exits
}

// ==================================================
// === Core 0 Main Function ===
// ==================================================
int main() {
    // Initialize standard I/O
    stdio_init_all();
    printf("\nVGA ready\n");

    // Initialize VGA display
    initVGA();

    // Start core 1 processing
    multicore_reset_core1();
    multicore_launch_core1(&core1_main);

    // Add threads for core 0
    pt_add_thread(protothread_graphics);
    pt_add_thread(protothread_toggle25);

    // Start thread scheduling (never exits)
    pt_schedule_start;
} 