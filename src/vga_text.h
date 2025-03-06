/**
 * Hunter Adams (vha3@cornell.edu)
 * modifed for 16 colors by BRL4
 * 
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


// Give the I/O pins that we're using some names that make sense - usable in main()
enum vga_pins {HSYNC=16, VSYNC, LO_GRN, HI_GRN, BLUE_PIN, RED_PIN} ;

// We can only produce 16 (4-bit) colors, so let's give them readable names - usable in main()
enum colors {BLACK, DARK_GREEN, MED_GREEN, GREEN,
             DARK_BLUE, BLUE, LIGHT_BLUE, CYAN,
             RED, DARK_ORANGE, ORANGE, YELLOW, 
             MAGENTA, PINK, LIGHT_PINK, WHITE} ;

// VGA primitives - usable in main
void initVGA(void) ;
void drawPixel(short x, short y, char color) ;
void drawChar(short x, short y, unsigned char c, char color, char bg) ;