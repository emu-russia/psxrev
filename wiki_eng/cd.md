# CD-ROM Subsystem

The CD-ROM control subsystem has undergone the greatest number of changes during the lifetime of PSX.

Let's take a look at the main components that make up this subsystem.

- CD Driver
- CD RF 
- CDROM Decoder
- CD DSP
- The drive itself ("optical device")

Note that at one time, each component was a single chip, but later, as a result of integration, several components were put into a single [common chip](subic.md).

Also keep in mind that the CD-ROM subsystem is in close communication with [SPU](spu.md) and [SUB-CPU](subcpu.md). In general, all together, these systems are part of the so-called `PSX Subsystem`.

## CD Driver

The task of the [CD Driver](cddriver.md) is to control the CD carriage (sled), spindel, focus and laser movement.

Recall that the CD driver can rotate the disk, move the carriage with the head back and forth, and the head itself can move in the carriage in two mutually perpendicular directions (up and down and left and right, relative to the carriage movement).

## CD RF

[CD RF](cdrf.md) controls the laser emission as well as receiving the signal from it and detecting errors.

The signal from the laser comes to a matrix of 4 detectors A,B,C,D and two side E-F, which come to CD-RF input.

The laser diode (LD) power control is performed by means of feedback: the signal from the photo detector (PD) goes to the CD-RF chip, which houses the automatic power control system (APC), which corrects the voltage on the laser diode.

## CDROM Decoder

The [CDROM Decoder](cddec.md) is the link between the central processing unit (CPU) and the entire PSX subsystem. It has its own memory for storing the loaded data from the CD-ROM and feeds the data into [SPU](spu.md) for CD Audio playback.

The CD decoder includes control registers that are controlled by the SUB-CPU with the help of the microcode that is embedded in the SUB-CPU ROM.

The CD decoder also gives the programmer control by programming the four "external" registers cdreg0-cdreg3, which are accessible via the sub bus.

## CD DSP

[CD DSP](cddsp.md) is engaged in decoding the signal coming from the CD-RF into a digital form, detecting tracking errors (TE) and focus errors (FE), as well as controlling the CD Driver. The CD DSP operates according to a program stored in [SUB-CPU](subcpu.md) ROM. The CD-DSP is controlled by sending special command packets.

Decoded data goes to the CDROM Decoder, which decides where to pass them - outward (to the CPU) or to the sound processor (SPU).

## Optical Device (Drive)

![Optical_device_back](/wiki/imgstore/Optical_device_back.jpg)

![Optical_device_back_noted](/wiki/imgstore/Optical_device_back_noted.jpg)

![Optical_device_back_hand](/wiki/imgstore/Optical_device_back_hand.jpg)

Optical device revisions:

|Revision|Drive|Drive cable|PCB|Console version|
|---|---|---|---|---|
|KSM-440 AAM (short cable)|![Aam_f1](/wiki/imgstore/Aam_f1.jpg) ![Aam_b1](/wiki/imgstore/Aam_b1.jpg)|![Ps1-p-009](/wiki/imgstore/Ps1-p-009.jpg)|PU-7|SCPH-1000|
|KSM-440 ACM (short cable)|![Acm_f1](/wiki/imgstore/Acm_f1.jpg) ![Acm_b1](/wiki/imgstore/Acm_b1.jpg)|![Ps1-p-009](/wiki/imgstore/Ps1-p-009.jpg)|PU-8|SCPH-1001/2, SCPH-300x|
|KSM-440 ADM (long cable)|![Adm_f1](/wiki/imgstore/Adm_f1.jpg) ![Adm_b1](/wiki/imgstore/Adm_b1.jpg)|![Ps1-p-010-1](/wiki/imgstore/Ps1-p-010-1.jpg)|PU-18, PU-20|SCPH-500x, SCPH-700x|
|KSM-440 AEM (medium cable)|![Aem_f1](/wiki/imgstore/Aem_f1.jpg) ![Aem_b1](/wiki/imgstore/Aem_b1.jpg)|![Ps1-p-010](/wiki/imgstore/Ps1-p-010.jpg)|PU-22, PU-23|SCPH-750x, SCPH-900x|
|KSM-440 BAM (short cable)|![Bam_f1](/wiki/imgstore/Bam_f1.jpg) ![Bam_b](/wiki/imgstore/Bam_b.jpg)|![Ps1-p-009](/wiki/imgstore/Ps1-p-009.jpg)|PM-41, PM-41(2)|SCPH-10x (PSOne)|

At the hardware level (signal level) almost all drives are compatible with each other, differing only by the size and length of the cable. In rare cases, there may be incompatibilities: for some versions of PSone (KSM-440 BAM) and first PS1 models (KSM-440 ACM).

Source of drive version information: http://chip.tut.su/=ps1/ps1_heads.htm

## CN701 and CN702 Connectors

These connectors are used to connect the optical device to the chips:

![CN701_702](/wiki/imgstore/CN701_702.jpg)

The upper contact group is for the CD-RF chip and the lower contact group is for the CD Driver chip.

The LIMIT SWITCH is for the SUB-CPU, to detect the carriage parking.
