# CD-DRIVER

CD Driver - special chip for controlling [CD-ROM](cd.md) motors and coils:

- Spindel - The main motor on which the disk rotates. Usually spins in one direction.
- Sledge - The carriage carries the laser head. The carriage is usually driven by a worm gear.
- Focusing coil - Moves the laser diode perpendicular to the disk surface
- Tracking coil - designed to move the laser beam precisely over the tracks of the CD

A very good summary of CD design is available here: http://www.radiofan.ru/faq/cd/part1.htm

Older revisions of motherboards had the CD Driver represented by a ROHM chip (IC704) and a bunch of third party chips. In later revisions the CD Driver was replaced by a single common chip `IC722`. That's what we will take it as a "reference".

## IC704

![Ba6392fp_sm](/wiki/imgstore/Ba6392fp_sm.jpg)

Full resolution: https://drive.google.com/file/d/1tPvfsoNfepg7JGQX0SbOree_kSWo4pOS/view

## Hardware Interface

![IC722_overview](/wiki/imgstore/IC722_overview.jpg)

The motors and coils are controlled by [CD-DSP](cddsp.md):

- TFDR/TRDR: Tracking coil control (F-forward, R-reverse)
- FFDR/FRDR: control of the focus coil
- SRDR/SFDR: motor carriage control
- MDP: motor dirve phase: control the speed of the disc, by giving special pulses

It is possible to set the speed of the disc (1x/2x), by giving the SPEED signal, which comes from [SUB-CPU](subcpu.md)

The MUTE signal is connected to the RESET3.3 signal (resetting the low voltage circuits of the PSX subsystem).

## Forward/Reverse

TBD.

## MDP

There is a description of the MDP signal in the CD-DSP documentation.

![MDP](/wiki/imgstore/MDP.jpg)

The motor can be controlled in two modes (set in the CD-DSP DCLV PWM MD settings): with and without the MDS.

In particular IC722 is controlled by a single MDP pin (without MDS involvement), and the positive polarity sets the motor acceleration and the negative polarity sets the motor deceleration. The acceleration value is coded by PWM and n varies from 0 to 31.
