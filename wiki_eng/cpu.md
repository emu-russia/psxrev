## CPU

The CPU on all revisions of the motherboard is represented by one large chip with 208 pins, under the designation IC103.

|PCB|PU-7|older PU-8|newer PU-8|PU-18|PU-20|PU-22|PU-23|PM-41|PM-41(2)|
|---|---|---|---|---|---|---|---|---|---|
|IC103|![CXD8530BQ_package](/wiki/imgstore/CXD8530BQ_package.jpg)|![8530BQ_PU8_package](/wiki/imgstore/8530BQ_PU8_package.jpg)|![8530CQ_package](/wiki/imgstore/8530CQ_package.jpg)|![CXD8606AQ_package](/wiki/imgstore/CXD8606AQ_package.jpg)|![CXD8606BQ_package](/wiki/imgstore/CXD8606BQ_package.jpg)|![8606BQ_PU22_package](/wiki/imgstore/8606BQ_PU22_package.jpg)|![8606BQ_PU23_package](/wiki/imgstore/8606BQ_PU23_package.jpg)|![8606BQ_PM41_package](/wiki/imgstore/8606BQ_PM41_package.jpg)|![CXD8606CQ_package](/wiki/imgstore/CXD8606CQ_package.jpg)|
||8530BQ|8530BQ|8530CQ|8606AQ|8606BQ|8606BQ|8606BQ|8606BQ|8606CQ|
||L9A0025|L9A0025|L9A0048|L9A0082|L9B0082|L9B0082|L9B0082|L9B0082|L9A0182|

The CPU consists of the following components:

- The chip is based on the CoreWare chip development framework
- Slightly modified LSI Logic CW33300 core
- System coprocessor 2 (GTE)
- MDEC (JPEG-like video decoder)
- DMA controller (DMAC) (7 channels)
- Interrupt controller (INTC)
- DRAM controller (DRAMC)
- Bus controller (interface for ROM BIOS / GPU)
- SIO controller (RS-232-like serial interface), for two ports (SIO0 and SIO1)
- PIO controller (expansion port for additional devices)
- Root counters (3 hardware counters)
- Built-in instruction cache
- ScratchPad is used instead of the data cache
- Dedicated "mini-caches" (R-buffer and W-buffer)

Microphotograph of a chip:

![Cpu_chip](/wiki/imgstore/Cpu_chip.jpg)

As you can see most of the chip is taken up by the "mess" of synthesized HDL logic, and at the edges there are various memory and registers.

## CPU Revisions

- The very first Japanese consoles (SCPH-1000 / PU-7) and old versions of PU-8 came with revision 90025.
- Then they were quickly replaced by newer consoles (there was some bug in MDEC) which already had the revision 90048 chip.
- In consoles since SCPH-5500 (PU-18) the 90082 revision of the chips were added. These chips were present in all latest PSX models, also in the first version of PSOne motherboards (PM-41).
- The latest versions of PSOne with PM-41(2) motherboards contained 90182 revision of the chip.

You can find out the revision of the chip from the marking on the cover, removing unnecessary letters (e.g. L9A0048 means revision 90048). There must be some sense in the letters, but most likely it is related to the improvement of the technological process.

There is also a tattoo on the lower right corner of the chip. The revision of the chip is indicated in the first line:

![6f18eaaedc260890621c89afba5b0b46](/wiki/imgstore/6f18eaaedc260890621c89afba5b0b46.jpg)

## Related ICs

Related chips include [CPU DRAM](dram.md) and [ROM BIOS](bios.md).

In early versions of the motherboard, the DRAM was represented by four 8-bit memory packages. Later it was replaced by a single 32-bit package.

![Cpu_ram_nec_424805al-a60](/wiki/imgstore/Cpu_ram_nec_424805al-a60.jpg)

## Hardware Interface

We will be guided by the picture from the PU-22 (SCPH-7500) service manual when the CPU was most fully connected to the other parts.

Starting with PU-23 (SCPH-9000) the parallel port (PIO) was taken away from it, and in PM-41 (PSOne) the serial port (SIO) was also taken away.

![CPU_Block](/wiki/imgstore/CPU_Block.jpg)

Before examining the control signals, it should be noted that the CPU operates with two buses which are controlled by the bus unit (B/U):
- Main bus: runs inside the CPU and also connects to [DRAM](dram.md) and [GPU](gpu.md)
- Sub bus: all the subsystem devices (ROM BIOS, [SPU](spu.md), [CD-ROM](cd.md)) and the parallel port (which is really a kind of external device as well) are on this bus
- /SWR0, /SWR1: Sub bus write enable. SWR0 is intended for both internal Sub bus devices and PIOs, but SWR1 for some reason is exclusively fed only to PIOs.
- /SRD: Sub bus read enable.

PIO:
- /CS0: Select PIO on the sub bus. Only one device can be connected to the bus at a time to avoid bus conflicts. This is controlled by the terminals in the "CS" (chip select) group.
- DACK5 / DREQ5: PIO DMA
- /INTIN10: terminal is combined with controllers/memory cards and PIO (common interrupt signal).

SIO:
- RXD1, TXD1, /DSR1, /DTR1, /CTS1, /RTS1: conventional serial interface

Controllers/memory cards: a variation of SIO
- /SCK0: timing for controllers (strobe?)
- RXD0, TXD0, /DSR0, /DTR0A, /DTR0B: serial interface. DTR has two pins because we have two ports for controllers/memory cards (Port A and Port B)

![Front_jack](/wiki/imgstore/Front_jack.jpg)

DRAM: in newer motherboards the RAM is a single chip (IC106), but previously there were more (four). We will focus on the newer version because it is convenient.
- DD: A 32-bit data bus.
- DA: a 10 bit address bus
- /DWE: write enable
- /DRAS0, /DCAS0, /DCAS1, /DCAS2, /DCAS3: to set Row/Column

ROM BIOS: ROM is connected to the sub bus.
- /CS2: Connect the ROM to the sub bus. Direct data exchange (OE) is enabled if the read enable signal (/SRD) is active. Well that's understandable, the ROM can only be read :smiley:

GPU:
- VD: 32-bit data bus, connected to Main bus
- VA2: address bus, 1bit :smiley: The thing is, the GPU has only two 32-bit registers (GP0/GP1), therefore, one address line is enough to address them.
- /VRD, /VWR: read/write enable
- /CS7: Select GPU
- DREQ2, DACK2: GPU DMA. When a direct memory access controller is used to transfer data, the GPU 
processor sends a bus-capture request signal by setting the logic low on the GPUDREQ pin. Upon receiving such a request, the processor releases the buses, signaling this by setting a low logic level on the GPUDACK output.
- /INTIN0: GPU VBLANK interrupt signal
- TCLK0: comes out of the GPU PCK (pixel clock) pin, can be used as Root Counter 0 ( dot counter)
- TCLK1: comes out from GPU HBLANK pin, can be used as Root Counter 1 (horizontal line counter)
- /INTIN1: general interrupt of the GPU (can be caused by sending a special command to the GPU, but is not used in games as far as I know)

CD-ROM: 
- /CS5: select CD-controller
- /INTIN2: interrupt from CD-Controller

SPU:
- /CS4: select SPU
- /INTIN9: interrupt from SPU
- DACK4, /DREQ4: SPU DMA

CPU timing and resetting:
- CRYSTALP: input CLK, 67.73 MHz
- SYSCLK0: CLK to GPU input (33.3 MHz)
- SYSCLK1: CLK on Sub bus devices (33.3 MHz)
- DSYSCLK: CLK * 2 on GPU input (66.67 MHz NTSC, 64.5 MHz PAL)
- /EXT RESET: reset (comes from common signal RES3.3 from power supply)

![CPU_clk](/wiki/imgstore/CPU_clk.jpg)
