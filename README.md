# Raspberry Pi Pico 2 - Text-Based VGA Display

This project demonstrates how to drive a text-based VGA display using the
**Raspberry Pi Pico 2**. The display uses a 640x480 pixel resolution with 16
colors (4-bit color depth). The Raspberry Pi Pico communicates with the display
via a single 8-bit interface latch and provides debugging via UART.

## Features

- **Resolution**: 640 x 480 pixels
- **Font**: Original Philips P2000C 12x8 bitmap font
- **Color Depth**: 4-bit (16 colors)
- **Refresh Rate**: ~72 Hz (using a 150 MHz clock with a x5 clock divider)
- **Interface**: Single 8-bit latch for display control
- **UART**: For debugging and communication

> [!WARNING]
> **Do not connect this board to a CRT monitor unless you are absolutely sure
> that the monitor supports 72 Hz vertical sync at 640x480 resolution**.  
> Connecting this board to a CRT that does not support these specifications may
> cause **damage to the monitor** or result in **display instability**.  
> Always check your CRT monitor's specifications to confirm compatibility with
> 72 Hz VSync at 640x480 resolution before proceeding.

## How It Works

The Raspberry Pi Pico drives the display by sending pixel data through an 8-bit
latch interface, which updates the screen in real-time. The VGA signal is
generated at the specified resolution and color depth, with a refresh rate of
approximately 72 Hz. Debugging information can be sent via the UART interface
for easy monitoring and troubleshooting.