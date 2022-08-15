## CPU

The CPU on all revisions of the motherboard is represented by one large chip with 208 pins, under the designation IC103.

|PCB|PU-7|older PU-8|newer PU-8|PU-18|PU-20|PU-22|PU-23|PM-41|PM-41(2)|
|---|---|---|---|---|---|---|---|---|---|
|IC103|![CXD8530BQ_package](/wiki/imgstore/CXD8530BQ_package.jpg)|![8530BQ_PU8_package](/wiki/imgstore/8530BQ_PU8_package.jpg)|![8530CQ_package](/wiki/imgstore/8530CQ_package.jpg)|![CXD8606AQ_package](/wiki/imgstore/CXD8606AQ_package.jpg)|![CXD8606BQ_package](/wiki/imgstore/CXD8606BQ_package.jpg)|![8606BQ_PU22_package](/wiki/imgstore/8606BQ_PU22_package.jpg)|![8606BQ_PU23_package](/wiki/imgstore/8606BQ_PU23_package.jpg)|![8606BQ_PM41_package](/wiki/imgstore/8606BQ_PM41_package.jpg)|![CXD8606CQ_package](/wiki/imgstore/CXD8606CQ_package.jpg)|
||8530BQ|8530BQ|8530CQ|8606AQ|8606BQ|8606BQ|8606BQ|8606BQ|8606CQ|
||L9A0025|L9A0025|L9A0048|L9A0082|L9B0082|L9B0082|L9B0082|L9B0082|L9A0182|

The CPU consists of the following components:

- Slightly modified LSI LR33300 core
- System coprocessor 2 (GTE)
- MDEC (JPEG-like video decoder)
- DMA controller (DMAC) (7 channels)
- Interrupt controller (INTC)
- DRAM controller (DRAMC)
- Bus controller (interface for ROM BIOS / GPU)
- SIO controller (RS-232-like serial interface), for two ports (SIO0 and SIO1)
- PIO controller (expansion port for additional devices)
- Root counters (3 hardware counters)
- Built-in instruction cache and data cache (data cache with direct access capability)
- Dedicated "mini-caches" (R-buffer and W-buffer)

Microphotograph of a chip:

![Cpu_chip](/wiki//imgstore/Cpu_chip.jpg)

As you can see most of the chip is taken up by the "mess" of synthesized HDL logic, and at the edges there are various memory and registers.

## CPU Revisions

- The very first Japanese consoles (SCPH-1000 / PU-7) and old versions of PU-8 came with revision 90025.
- Then they were quickly replaced by newer consoles (there was some bug in MDEC) which already had the revision 90048 chip.
- In consoles since SCPH-5500 (PU-18) the 90082 revision of the chips were added. These chips were present in all latest PSX models, also in the first version of PSOne motherboards (PM-41).
- The latest versions of PSOne with PM-41(2) motherboards contained 90182 revision of the chip.

On this site we are examining the `90048` revision (which was in SCPH-1001). It is likely that the new revisions differ significantly in M1/M2 wiring as the new revision chip is "reassembled" from Verilog/VHDL. So, to trace other revisions means to re-trace the whole processor %)

You can find out the revision of the chip from the marking on the cover, removing unnecessary letters (e.g. L9A0048 means revision 90048). There must be some sense in the letters, but most likely it is related to the improvement of the technological process.

There is also a tattoo on the lower right corner of the chip. The revision of the chip is indicated in the first line:

![6f18eaaedc260890621c89afba5b0b46](/wiki//imgstore/6f18eaaedc260890621c89afba5b0b46.jpg)

## Related ICs

Related chips include [CPU DRAM](dram.md) and [ROM BIOS](bios.md).

In early versions of the motherboard, the DRAM was represented by four 8-bit memory packages. Later it was replaced by a single 32-bit package.

![Cpu_ram_nec_424805al-a60](/wiki//imgstore/Cpu_ram_nec_424805al-a60.jpg)

## Hardware Interface

We will be guided by the picture from the PU-22 (SCPH-7500) service manual when the CPU was most fully connected to the other parts.

Starting with PU-23 (SCPH-9000) the parallel port (PIO) was taken away from it, and in PM-41 (PSOne) the serial port (SIO) was also taken away.

![CPU_Block](/wiki//imgstore/CPU_Block.jpg)
