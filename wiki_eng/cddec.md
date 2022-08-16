# CD Decoder

:warning: In different sources this chip can be called either CD-decoder or CD-controller. We used to use the term "CD-Controller" on the wiki, but then we changed it to "CD-Decoder" according to the datasheet. If you come across CD-Controller somewhere else keep in mind that it is the same thing. Official service manuals, for example, use the term "CD-Controller".

The CD-Decoder is designed to distribute data from the disk between the central processing unit ([CPU](cpu.md)) and the sound processor ([SPU](spu.md)). The end result is data files written to disk sectors (for CPU) or streaming CD-Audio (for SPU). Functionally, the CD decoder is a kind of "bridge" of the PSX subsystem.

Like most other components of the CD/Audio system, the CD decoder was later incorporated into one [large chip](subic.md), so we will consider it as a separate chip - `IC305` (CXD1815Q).

![CXD1815Q_package](/wiki/imgstore/CXD1815Q_package.jpg)

Thanks to Martin Korth, he discovered that the PU-7 version of the motherboards used CXD1199, which has a [datasheet](/docs/CXD1199.pdf).

## Programmer Interface

The programmer can control the CD decoder through its 4 registers, usually called simply `CDREG0`-`CDREG3`.

The control is done by sending special commands.

A CD-DMA is also available to send data to the CPU memory.

## Hardware Interface

![IC305_overview](/wiki/imgstore/IC305_overview.jpg)

Schematic (from CXD1199, but I think that there are no fundamental differences):

![CXD1199_circuit](/wiki/imgstore/CXD1199_circuit.jpg)

The CD decoder communicates with the CPU, with the [SUB-CPU](subcpu.md) and also with the CDROM memory (IC303), so it has three sets of address/data buses at once:

- HA0-HA1: 2-bit host address bus, 2 bits are used because the CD decoder contains only 4 control registers.
- HD0-HD7: 8-bit host data bus. The CD-decoder communicates with the CPU with the values of its registers on these two buses.
- A0-A4: 5-bit address bus with SUB-CPU (addressed only 32 bytes - the internal registers of the CD-decoder)
- D0-D7: 8-bit data bus with SUB-CPU. The SUB-CPU is addressed to the decoder through these two buses.
- MA0-MA16: 17-bit address bus with CDROM RAM (only 15 bits of MA0-MA14 are used, addressing 32 KB RAM)
- MDB0-MDB7: 8 bit data bus with CDROM RAM. The decoder addresses its memory via these two buses.
- TD0-TD7: seems to be some kind of test bus (test data?), not connected.

Control pins:

- CPU side: XHWR (write), XHRD (read), HINT (interrupt), XHCS (chip select), XHRS (reset, not used)
- SUB-CPU side: XINT (interrupt), XWR (write), XRD (read), XCS (chip select), XRST (reset). In this case SUB-CPU controls the decoder. The decoder can only send an interrupt to the SUB-CPU.
- CDROM RAM side: XMOE (chip enable), XMWR (write)

As usual, the pins whose names start with "X" have inverse logic (active low).

CD data output stream for SPU: BCKO (bit clock), LRCO (left/right clock), DATO (data out) (serial interface)

Contacts for connection to [CD-DSP](cddsp.md): EMP, C2PO, BCLK, DATA, LRCK, RMCK. Purpose to be clarified :smiley:

Other unused pins: MUTE (not connected), WCKO (word clock, not connected), CKSL (clock select, grounded), HCLK (not connected), MDBP,HDP (not connected), HDRQ (DMA request, not connected), XHAC (grounded)

## How data transfer is organized

This description is mostly speculative, as there are no exact details about the inner workings of the CD-decoder yet.

However, based on the logic of the input and output terminals, as well as some knowledge of the inner workings of the CD system, it is possible to make some assumptions about the data transfer process.

- Firstly, it is absolutely certain that the read CD data is stored in the CDROM memory (also sometimes called FIFO). This data is intended for two consumers: the SPU and the CPU.
- In terms of the SPU, there are no questions: inside the decoder is a serial interface that streams audio to the SPU via the BCKO, LRCO and DATO pins.
- Input data is loaded into the CDROM memory via the serial interface with the CD-DSP
- Commands received on the CDREG0-CDREG3 is apparently "parsed" by SUB-CPU microcode. This is likely the CD-decoder uses SUB-CPU interrupt (signal XINT), to signal that it needs to handle the next portion of data coming to CDREG0-CDREG3. SUB-CPU can also set register output values. To be more precise it is necessary to disassemble the ROM SUB-CPU.
- The only unresolved issue is the CD DMA. It is not yet clear how the CPU organizes the DMA transfer of CD data. It is assumed that this uses a special combination of XHWR/XHRD, or special work with registers CDREG0-CDREG3. The only way to be sure is to send the data to the CPU is through the HD (SD) bus.

## Internal Registers

Inside the CD-decoder there is room for 32 8-bit registers. Data about the internal registers is obtained by examining the SUB-CPU microcode.

|Index(HEX)|Name|Description|
|---|---|---|
|0x17|CD_RESULT_0|Command result (byte 0)|
|0x18|CD_RESULT_1|Command result (byte 1)|
|0x19|CD_RESULT_2|Command result (byte 2)|
|0x1A|CD_RESULT_3|Command result (byte 3)|
|0x1B|CD_RESULT_4|Command result (byte 4)|
|0x1C|CD_RESULT_5|Command result (byte 5)|
|0x1D|CD_RESULT_6|Command result (byte 6)|
|0x1E|CD_RESULT_7|Command result (byte 7)|
|0x1F|||

![fixme](/wiki/imgstore/fixme.gif)
