/**
 *
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 470 ohm resistor ---> VGA Green 
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
 *  - GPIO 21 ---> 330 ohm resistor ---> VGA Red
 *  - RP2040 GND ---> VGA GND
 *
 * RESOURCES USED
 *  - PIO state machines 0, 1, and 2 on PIO instance 0
 *  - DMA channels 0, 1, 2, and 3
 *  - 153.6 kBytes of RAM (for pixel color data)
 *
 */

#ifndef _VGA_TEXT_H
#define _VGA_TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/dma.h>

#include "hsync.pio.h"
#include "vsync.pio.h"
#include "rgb.pio.h"

// Screen width/height
#define SCREENWIDTH 640
#define SCREENHEIGHT 480
#define CHARHEIGHT 12

#define LATCH_PIN 2         // GPIO for latch signal
#define DATA_BASE_PIN 8     // First data pin

// Function to check if a specific state machine is enabled
static bool pio_sm_is_enabled(PIO pio, uint sm) {
    // Ensure the state machine number is valid (0 to 3)
    if (sm > 3) return false;

    // Check the SM enable bit in the CTRL register
    return (pio->ctrl & (1u << sm)) != 0;
}

// Give the I/O pins that we're using some names that make sense - usable in main()
enum vga_pins {HSYNC=16, VSYNC, LO_GRN, HI_GRN, BLUE_PIN, RED_PIN} ;

// We can only produce 16 (4-bit) colors, so let's give them readable names - usable in main()
enum colors {BLACK, DARK_GREEN, MED_GREEN, GREEN,
             DARK_BLUE, BLUE, LIGHT_BLUE, CYAN,
             RED, DARK_ORANGE, ORANGE, YELLOW, 
             MAGENTA, PINK, LIGHT_PINK, WHITE} ;

// VGA primitives - usable in main
void initVGA(void) ;
void clear_screen();
void drawPixel(short x, short y, char color) ;
void drawChar(short x, short y, unsigned char c, char color, char bg) ;

#endif // _VGA_TEXT_H