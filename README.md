# PicoBeam: A Raspberry Pi Pico 2 based VGA Display

![GitHub tag (latest SemVer)](https://img.shields.io/github/v/tag/ifilot/pico-vga-text?label=version)
[![build](https://github.com/ifilot/pico-vga-text/actions/workflows/build.yml/badge.svg)](https://github.com/ifilot/pico-vga-text/actions/workflows/build.yml)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![License: CC BY-SA 4.0](https://img.shields.io/badge/License-CC%20BY--SA%204.0-blue.svg)](https://creativecommons.org/licenses/by-sa/4.0/)

This project demonstrates how to drive a VGA display using the **Raspberry Pi
Pico 2**. The display uses a 640x480 pixel resolution with 16 colors (4-bit
color depth). The Raspberry Pi Pico communicates with the display via a single
8-bit interface latch and provides debugging via UART.

## Features

- **Resolution**: 640 x 480 pixels
- **Builtin Font**: Original Philips P2000C 12x8 bitmap font
- **Bitmapped graphics**: Support for printing individual pixels to the screen
- **Color Depth**: 4-bit (16 colors)
- **Refresh Rate**: ~72 Hz (using a 150 MHz clock with a x5 clock divider)
- **Interface**: Single 8-bit latch for display control
- **UART**: For debugging and communication
- **Beeper**: 1-bit beeper for simple audio feedback

> [!WARNING]
> **Do not connect this board to a CRT monitor unless you are absolutely sure
> that the monitor supports 72 Hz vertical sync at 640x480 resolution**. 
> Connecting this board to a CRT that does not support these specifications may
> cause **damage to the monitor** or result in **display instability**. 
> Always check your CRT monitor's specifications to confirm compatibility with
> 72 Hz VSync at 640x480 resolution before proceeding.

## How It Works

The Raspberry Pi Pico drives the display by receiving pixel data through an
8-bit latch interface, which updates the screen in real-time. By connecting
the Pico to the data bus of a CPU or microcontroller, instructions can be sent
to the Pico.

## Compilation instructions

### Toolchain

```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi build-essential libstdc++-arm-none-eabi-newlib
sudo git clone https://github.com/raspberrypi/pico-sdk.git /opt/pico-sdk
sudo git -C /opt/pico-sdk submodule update --init
RUN 'export PICO_SDK_PATH=/opt/pico-sdk' | tee -a /etc/profile.d/pico-sdk.sh
```

### Compiling

```bash
mkdir build && cd build
cmake ../src
make -j
```

## Instruction delays

The Pico requires a bit of processing time and one should be cautious not to
sent the data too fast. The following timings were found to give stable results
when using a simple (slow) voltage divider circuit. When connecting the Pico to
your specific setup, it is recommended to perform your own calibration to find
which delays provide stable results.

| Operation                                         | Delay (μs) |
|---------------------------------------------------|------------|
| Single characters                                 | 150        |
| Large instructions (e.g., beeper or screen clear) | 2000       |
