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
#include "latch.pio.h"
#include "protothreads.h"
 
char ch;
int newchar = 1;

// ==================================================
// === VGA Graphics Thread (Core 0) ===
// ==================================================
static PT_THREAD(protothread_graphics(struct pt *pt)) {
    PT_BEGIN(pt);

    static short posx = 0;
    static short posy = 0;
    static char fg_color = WHITE;
    static char bg_color = BLACK;

    enum colors {BLACK, DARK_GREEN, MED_GREEN, GREEN,
        DARK_BLUE, BLUE, LIGHT_BLUE, CYAN,
        RED, DARK_ORANGE, ORANGE, YELLOW, 
        MAGENTA, PINK, LIGHT_PINK, WHITE} ;

    while (true) {
        // Draw new character if received
        if (newchar) {
            
            if(ch >= 0x80) {  // check for control bytes
                switch(ch) {
                    case 0x80:
                        fg_color = BLACK;
                    break;
                    case 0x81:
                        fg_color = DARK_GREEN;
                    break;
                    case 0x82:
                        fg_color = MED_GREEN;
                    break;
                    case 0x83:
                        fg_color = GREEN;
                    break;
                    case 0x84:
                        fg_color = DARK_BLUE;
                    break;
                    case 0x85:
                        fg_color = BLUE;
                    break;
                    case 0x86:
                        fg_color = LIGHT_BLUE;
                    break;
                    case 0x87:
                        fg_color = CYAN;
                    break;
                    case 0x88:
                        fg_color = RED;
                    break;
                    case 0x89:
                        fg_color = DARK_ORANGE;
                    break;
                    case 0x8A:
                        fg_color = ORANGE;
                    break;
                    case 0x8B:
                        fg_color = YELLOW;
                    break;
                    case 0x8C:
                        fg_color = MAGENTA;
                    break;
                    case 0x8D:
                        fg_color = PINK;
                    break;
                    case 0x8E:
                        fg_color = LIGHT_PINK;
                    break;
                    case 0x8F:
                        fg_color = WHITE;
                    break;
                    case 0x90:
                        bg_color = BLACK;
                    break;
                    case 0x91:
                        bg_color = DARK_GREEN;
                    break;
                    case 0x92:
                        bg_color = MED_GREEN;
                    break;
                    case 0x93:
                        bg_color = GREEN;
                    break;
                    case 0x94:
                        bg_color = DARK_BLUE;
                    break;
                    case 0x95:
                        bg_color = BLUE;
                    break;
                    case 0x96:
                        bg_color = LIGHT_BLUE;
                    break;
                    case 0x97:
                        bg_color = CYAN;
                    break;
                    case 0x98:
                        bg_color = RED;
                    break;
                    case 0x99:
                        bg_color = DARK_ORANGE;
                    break;
                    case 0x9A:
                        bg_color = ORANGE;
                    break;
                    case 0x9B:
                        bg_color = YELLOW;
                    break;
                    case 0x9C:
                        bg_color = MAGENTA;
                    break;
                    case 0x9D:
                        bg_color = PINK;
                    break;
                    case 0x9E:
                        bg_color = LIGHT_PINK;
                    break;
                    case 0x9F:
                        bg_color = WHITE;
                    break;
                    case 0xFF:
                        clear_screen();
                        posy = 0;
                        posx = 0;
                    break;
                }
            } else {
                if(posy + CHARHEIGHT >= SCREENHEIGHT) {
                    clear_screen();
                    posy = 0;
                    posx = 0;
                }
    
                if(ch == '\n') {
                    posy += CHARHEIGHT;
                    posx = 0;
                } else {
                    drawChar(posx, posy, ch, fg_color, bg_color);
    
                    posx += 8;
                    if (posx >= SCREENWIDTH-1) {
                        posx = 0;
                        posy += CHARHEIGHT;
                    }
                }
            }

            // only release when char is done
            newchar = false;
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
    while (true) {
        // Spawn a thread to handle non-blocking serial write
        serial_write;

        // Spawn a thread to handle non-blocking serial read
        serial_read;

        PT_YIELD(pt);
    }
    PT_END(pt);
}

// ==================================================
//            Latch Input Thread (Core 1) 
// ==================================================
static PT_THREAD(protothread_latch(struct pt *pt)) {
    PT_BEGIN(pt);

    // Choose PIO and state machine
    PIO pio = pio1;
    uint sm = 0;

    // Define latch pin (GPIO 2) and data pins (GPIO 8-15)
    const uint latch_pin = 2;
    const uint data_base_pin = 8;

    // Initialize PIO program
    uint offset = pio_add_program(pio, &latch_program);
    latch_program_init(pio, sm, offset, latch_pin, data_base_pin);

    // Main loop to continuously read and display captured data
    while (true) {
        if(!pio_sm_is_rx_fifo_empty(pio, sm) && !newchar) {
            ch = pio_sm_get(pio, sm);
            newchar = true;
        } else {
            PT_YIELD(pt);
        }
    }
    PT_END(pt);
}

// ==================================================
// === Core 1 Main Function ===
// ==================================================
void core1_main() {
    pt_add_thread(protothread_serial);
    pt_add_thread(protothread_latch);
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