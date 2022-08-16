# PSX Wiki

![PSX-Console-wController_small](/wiki/imgstore/PSX-Console-wController_small.png)

The PlayStation, PSX, was a TV console produced by SONY.

It is the first ever sane console with good games on CDs and three-dimensional graphics.

For [game development](sdk.md) a full-fledged development kit, the PsyQ SDK, based on a modified GCC-toolchain, was used.

In general, the appearance of PSX laid a solid foundation for SONY for many years to come. And then over the decades, SONY successfully used the same approaches that had been used in the PS1:
- Custom chip architecture.
- A more or less unified signal interface for interrupt handling (/INT) and DMA (Ack/Req).
- Each subsystem is controlled by its own smart chip (processor or controller)
- Use of custom toolchain from third-party developer (SN Systems) and in-house developed library in the SDK
- Standalone devkits (special versions of set-top boxes for developers)
- Operating system in C/POSIX direction, but hybrid nevertheless

:warning: The information on the wiki was formalized at different times and sometimes spontaneously, so while the overall information is more or less correct, there may be some minor inaccuracies, which will be corrected when found by the public.

## Architecture

Schematically, the PSX is divided into two parts: the Main system and the sub-system. The Main-system includes the CPU and GPU with the video system. The sub-system includes the CD subsystem and the SPU (sound processor).

Each system has its own bus (Main bus, Sub-bus), with the CPU connected to both buses.

In addition to the CPU, the PSX includes the so-called SUB-CPU, which, as you can easily guess, controls the CD/SPU subsystem.

## Main System

A picture from one "condidual" manual that shows the connections well:

![Psx_main_system](/wiki/imgstore/Psx_main_system.jpg)

The SIO interface (controllers, memory cards, and serial port) can also be referred to the main system. The SIO is simply not accessed through the buses, but through the registers of the CPU.

## Sub System

The sub-system includes:

- [SPU](spu.md)
- [SUB-CPU](subcpu.md)
- [CD-ROM subsystem](cd.md) (a bunch of chips)
- PIO
- [BIOS](bios.md)

![Subsystem](/wiki/imgstore/Subsystem.jpg)

## Peripherals

- [Controllers](controller.md)
- Memory Cards
- PlayStation Mouse
- Parallel port (PIO)
- Serial port (SIO)

## About the PSX

Nowadays it is not usual to use the abbreviation `PSX` for PlayStation 1 and the abbreviations `PS1` or `PSOne` are more common.

But we are supporters of the old school, so the good old `PSX` is used everywhere.
